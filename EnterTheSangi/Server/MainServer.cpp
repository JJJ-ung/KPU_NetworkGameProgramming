#pragma once
#include "MainServer.h"

CMainServer::CMainServer() {};
CMainServer::~CMainServer() {};

void CMainServer::Init(const int server_port)
{
    int rt;

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //socket()
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listen_socket == INVALID_SOCKET) {
        //err_quit("socket()")
    };

    //bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(server_port);
    rt=bind(m_listen_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (rt == SOCKET_ERROR) {
        //err_quit("bind()")
    };


    for (int i = 0; i < MAX_CLIENTS; ++i)
        m_clients[i].SetID(i);

    for (auto& cl : m_clients)
    {
        cl.SetState(ST_FREE);
    }

    m_game_state = SCENE::ID::CUSTOMIZE;
    for(int i=0;i<MAX_CLIENTS;++i)
        m_client_event[i] = CreateEvent(NULL, true, true, NULL);
    m_server_event= CreateEvent(NULL, true, false, NULL);
};

void CMainServer::Activate()
{
    int rt;

    //listen()
    rt = listen(m_listen_socket, SOMAXCONN);
    if (rt == SOCKET_ERROR) {
        //err_quit("listen()")
    };

    //클라이언트별로 하나씩 할당되는 스레드.
    //for (int i = 0; i < MAX_CLIENTS; ++i)
    //    
    //for (auto& th : m_client_threads)
    //    th.join();

    //accept만을 위한 스레드. 주스레드에서 accpet 상관 없이 서버 연산을 돌리기 위함.
    for (int i = 0; i < 1; ++i)
        m_server_threads.emplace_back(&CMainServer::ServerThread, this);
    for (auto& th : m_server_threads)
        th.join();

    //accept()

};

void CMainServer::ClientThread(char id) 
{
    int ret;
    for (;;)
    {
        //In Robby
		m_state_lock.lock();
		while (m_game_state == SCENE::ID::CUSTOMIZE)
		{
			m_state_lock.unlock();

			ret = DoRecv(id);
            if (ret == SOCKET_ERROR)
            {
                m_state_lock.lock();
                continue;
            }
            else if (ret == 0)
            {
                m_state_lock.lock();
                continue;
            }
			ProcessPacket(id);

			m_state_lock.lock();
        }
        m_state_lock.unlock();


        //In Game
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::STAGE)
        {
            m_state_lock.unlock();
            //스레드 동기화
            //타이머 필요
            ret = DoRecv(id);
            if (ret == SOCKET_ERROR)
            {
                m_state_lock.lock();
                continue;
            }
            else if (ret == 0)
            {
                m_state_lock.lock();
                continue;
            }
            ProcessPacket(id);

            SetEvent(m_client_event[id]);
            WaitForSingleObject(m_server_event, INFINITE);//timeout 넣어야 하지 않을까 싶긴 한데 server_timer 동기화때문에 일단 둠
            //m_server_event 다시 죽여야하는데 어디서?
            // 전체 데이터 송신
            DoSend();

            m_state_lock.lock();
        }
        m_state_lock.unlock();
        //수신 성공시 suspend thread
        //모든 클라이언트로부터 recv 혹은 timeout시 서버 연산
        //이후 resume thread
       

    }
};

void CMainServer::ServerThread()
{
    int ret;
	for (;;)
	{  //In Robby
		m_state_lock.lock();
		while (m_game_state == SCENE::ID::CUSTOMIZE)
		{
			m_state_lock.unlock();
			DoAccept();
			m_state_lock.lock();
		}
		m_state_lock.unlock();
        
        m_server_timer = chrono::high_resolution_clock::now() + 1s / 60 * 2;
        //In Game
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::STAGE)
        {
            m_state_lock.unlock();
            WaitForMultipleObjects(MAX_CLIENTS, m_client_event, true, 1000 / 60 * 2);
            for (int i = 0; i < MAX_CLIENTS; ++i)
                ResetEvent(m_client_event[i]);
            ServerProcess();
            auto time_t = chrono::high_resolution_clock::now();
            if (time_t > m_server_timer)
                cout << "server process time over" << endl;
            else
                Sleep(chrono::duration_cast<chrono::milliseconds>(m_server_timer - time_t).count());
            SetEvent(m_server_event);

            m_server_timer += 1s / 60 * 2;

            m_state_lock.lock();
        }
        m_state_lock.unlock();
	}
};

void CMainServer::ProcessPacket(char client_id)
{
    char packet_type = m_clients[client_id].GetBuf()[1];

    if (packet_type == CS_PACKET_LOGIN)
    {
        cout << "Login Packet\n";
        cs_packet_login rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_login));

        m_clients[client_id].SetName(rp.name);

        cout << m_clients[client_id].GetName()<< endl;
        sc_packet_login_ok sp;
        sp.size = sizeof(sc_packet_login_ok);
        sp.type = SC_PACKET_LOGIN_OK;
        sp.id = client_id;
        sp.is_ready = false;
        

        send(m_clients[client_id].GetSocket(), (char*)&sp, sizeof(sc_packet_login_ok), 0);

        m_clients[client_id].StateLock();
        m_clients[client_id].SetState(ST_INROBBY);
        m_clients[client_id].StateUnlock();

        //다른 플레이어들에게 로그인했음을 알림.

        sc_packet_login_other_client packet;
        packet.size = sizeof(sc_packet_login_other_client);
        packet.type = SC_PACKET_LOGIN_OTHER_CLIENT;
        packet.id = client_id;
        strcpy_s(packet.name, m_clients[client_id].GetName());
        packet.body_color = m_clients[client_id].GetPlayer().GetBodyColor();
        packet.cloth_color = m_clients[client_id].GetPlayer().GetClothColor();
        packet.is_ready = false; //실은 스테이트 락 걸고 스테이트에서 받아와야함
                
        for (auto& other : m_clients)
        {
            if (other.GetID() == client_id)continue;
            other.StateLock();
            if (ST_INROBBY != other.GetState())
            {
                other.StateUnlock();
                continue;
            }
            other.StateUnlock();
                        
            send(m_clients[client_id].GetSocket(), (char*)&packet, sizeof(sc_packet_login_other_client), 0);
        }
    }

    else if (packet_type == CS_PACKET_CHANGE_COLOR)
    {
        cout << "CS_PACKET_CHANGE_COLOR Packet comein\n";

        cs_packet_change_color rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_change_color));

       // 서버에 id에 해당하는 플레이어 커마정보 저장
        m_clients[client_id].GetPlayer().SetBodyColor(rp.body_color);
        m_clients[client_id].GetPlayer().SetClothColor(rp.cloth_color);

        sc_packet_change_color sp;
        sp.type = SC_PACKET_CHANGE_COLOR;
        sp.id = client_id;
        sp.body_color = rp.body_color;
        sp.cloth_color = rp.cloth_color;
       
        for (auto& other : m_clients) {
            other.StateLock();
            if (ST_INROBBY != other.GetState())
            {
                other.StateUnlock();
                continue;
            }
            other.StateUnlock();

            send(other.GetSocket(), (char*)&sp, sizeof(sc_packet_change_color), 0);
        }
    }

    else if (packet_type == CS_PACKET_READY)
    {
        cout << "CS_PACKET_READY Packet comein\n";
      
        cs_packet_ready rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_ready));

        if (rp.is_ready == true)
        {
            m_clients[client_id].StateLock();
            if (m_clients[client_id].GetState() == ST_INROBBY)
                m_clients[client_id].SetState(ST_READY);
            m_clients[client_id].StateUnlock();
        }
        else
        {
            m_clients[client_id].StateLock();
            if (m_clients[client_id].GetState() == ST_READY)
                m_clients[client_id].SetState(ST_INROBBY);
            m_clients[client_id].StateUnlock();
        }
        
        sc_packet_ready sp;
        sp.type = SC_PACKET_READY;
        sp.size = sizeof(sc_packet_ready);
        sp.id = client_id;
        m_clients[client_id].StateLock();
        if (m_clients[client_id].GetState() == ST_READY)
            sp.is_ready = true;
        else if (m_clients[client_id].GetState() == ST_INROBBY)
            sp.is_ready = false;
        else
            std::cout << "state_error" << std::endl;
        m_clients[client_id].StateUnlock();

        for (auto& cl : m_clients) {
            send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_ready), 0);
        }

        //모든 클라이언트가 준비 상태이면 게임을 시작한다
        if (true == IsAllClientsReady())
        {
            m_state_lock.lock();
            m_game_state = SCENE::STAGE;
            m_state_lock.unlock();


        }
    }


    else
    {

    }

}

void CMainServer::DoSend()
{
    sc_packet_game_state sp;
    
    for (auto& cl : m_clients)
    {
        
    }

    // 여기서 플레이어들 정보 취합후 일괄 전송
    for (auto& cl : m_clients)
        send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_game_state), 0);
};

int CMainServer::DoRecv(char id)
{
    recv(m_clients[id].GetSocket(), m_clients[id].GetBuf(), BUF_SIZE, 0);
    ProcessPacket(id);

    return 0;
};

int CMainServer::DoAccept()
{
    //accpet()
    cout << "DoAccept() running\n";
    SOCKET client_socket;
    SOCKADDR_IN client_addr;
    int addr_len = sizeof(client_addr);
    client_socket = accept(m_listen_socket, (SOCKADDR*)&client_addr, &addr_len);
    if (client_socket == INVALID_SOCKET) return -1;
 
    char new_id = GetNewID();
    if (new_id != -1)
    {   
        m_clients[new_id].SetSocket(client_socket);
        m_client_threads.emplace_back(&CMainServer::ClientThread, this, new_id);
        cout << "[" << new_id  << "]  : client incoming \n";
        //플레이어 초기 정보 세팅
        //login_ok패킷 전송
    }
    else
    {
        // clients full
    }
};

void CMainServer::ServerProcess()
{

    //충돌체크 등 서버 로직 진행

};

char CMainServer::GetNewID()
{
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        m_clients[i].StateLock();
        if (m_clients[i].GetState() == ST_FREE)
        {
            m_clients[i].SetState(ST_ACCEPT);
            m_clients[i].StateUnlock();

            return i;
        }
        m_clients[i].StateUnlock();
    }
    cout << "Maximum Number of Clients Overflow!!\n";
   
    return -1;


};
bool CMainServer::IsAllClientsReady()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        m_clients[i].StateLock();
        if (m_clients[i].GetState() == ST_READY)
        {
            m_clients[i].StateUnlock();
            return false;
        }
        m_clients[i].StateUnlock();
    }
    return true;
}



void CMainServer::Disconnect(char id)
{
    m_clients[id].StateLock();
    m_clients[id].SetState(ST_FREE);
    m_clients[id].StateUnlock();
}
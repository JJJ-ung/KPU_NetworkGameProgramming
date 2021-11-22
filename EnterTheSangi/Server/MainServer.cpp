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

    SCENE::ID m_game_state = SCENE::ID::CUSTOMIZE;
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
        m_accpet_threads.emplace_back(&CMainServer::AccpetThread, this);
    for (auto& th : m_accpet_threads)
        th.join();

    //accept()

};

void CMainServer::ClientThread(char id) 
{
    for (;;)
    {
        //In Robby
        while (m_game_state==SCENE::ID::CUSTOMIZE)
        {
            DoRecv(id);
        }



        //In Game

        while (true)
        {
            DoRecv(id);
            //timeout 필요
        }
        //수신 성공시 suspend thread
        //모든 클라이언트로부터 recv 혹은 timeout시 서버 연산
        //이후 resume thread
        DoSend();

    }
};

void CMainServer::AccpetThread()
{
	for (;;)
	{
        DoAccept();
	}
};

void CMainServer::ProcessPacket(char id)
{
    if (m_clients[id].GetBuf()[0] == CS_PACKET_LOGIN)
    {
        cs_packet_login rp;
        memcpy(&rp, m_clients[id].GetBuf(), sizeof(cs_packet_login));

        m_clients[id].SetName(rp.name);

        sc_packet_login_ok sp;
        sp.type = SC_PACKET_LOGIN_OK;
        sp.id = id;
        sp.is_ready = false;
        sp.size = sizeof(sc_packet_login_ok);

        send(m_clients[id].GetSocket(), (char*)&sp, sizeof(sc_packet_login_ok), 0);
    }

    else if (m_clients[id].GetBuf()[0] == CS_PACKET_CHANGE_COLOR)
    {
        cs_packet_change_color rp;
        memcpy(&rp, m_clients[id].GetBuf(), sizeof(cs_packet_change_color));

       // 서버에 id에 해당하는 플레이어 커마정보 저장
        m_clients[id].GetPlayer().SetBodyColor(rp.body_color);
        m_clients[id].GetPlayer().SetClothColor(rp.cloth_color);

        sc_packet_change_color sp;
        sp.type = SC_PACKET_CHANGE_COLOR;
        sp.id = id;
        sp.body_color = rp.body_color;
        sp.cloth_color = rp.cloth_color;
       
        for (auto& cl : m_clients) {
            if (id == cl.GetID()) continue;
            send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_change_color), 0);
        }
    }

    else if (m_clients[id].GetBuf()[0] == CS_PACKET_READY)
    {
        cs_packet_ready rp;
        memcpy(&rp, m_clients[id].GetBuf(), sizeof(cs_packet_ready));

        m_clients[id].SetState(ST_READY);

        sc_packet_ready sp;
        sp.type = SC_PACKET_READY;
        sp.size = sizeof(sc_packet_ready);
        sp.id = id;
        sp.is_ready = true;

        for (auto& cl : m_clients) {
            send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_ready), 0);
        }
    }
}

void CMainServer::DoSend()
{
    sc_packet_game_state sp;
    
    for (auto& cl : m_clients)
    {
        ;
    }

    // 여기서 플레이어들 정보 취합후 일괄 전송
    for (auto& cl : m_clients)
        send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_game_state), 0);
};

void CMainServer::DoRecv(char id)
{
   
    recv(m_clients[id].GetSocket(), m_clients[id].GetBuf(), BUF_SIZE, 0);
    ProcessPacket(id);

};

int CMainServer::DoAccept()
{
    //accpet()
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
        //플레이어 초기 정보 세팅
        //login_ok패킷 전송
    }
    else
    {
        // clients full
    }
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

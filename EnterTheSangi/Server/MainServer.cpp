#pragma once
#include "MainServer.h"

CMainServer::CMainServer() {};
CMainServer::~CMainServer() {};
int ttt;
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
    rt = bind(m_listen_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (rt == SOCKET_ERROR) {
        //err_quit("bind()")
    };


    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        m_clients[i].SetID(i);
        m_clients[i].GetPlayer().SetBodyColor(D3DXVECTOR3(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f));
        m_clients[i].GetPlayer().SetClothColor(D3DXVECTOR3(rand() % 5 * 0.2f, rand() % 5 * 0.2f, rand() % 5 * 0.2f));
    }

    for (auto& cl : m_clients)
    {
        cl.SetState(ST_FREE);
    }

    InitBullets();
    InitChests();

    m_game_state = SCENE::CUSTOMIZE;
    for (int i = 0; i < MAX_CLIENTS; ++i)
        m_client_event[i] = CreateEvent(NULL, true, true, NULL);
    m_server_event = CreateEvent(NULL, true, false, NULL);
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

            if (m_clients[id].GetState() != ST_FREE) {
                ret = DoRecv(id);
                if (ret == SOCKET_ERROR)
                {
                    Disconnect(id);
                    m_state_lock.lock();
                    continue;
                }
                else if (ret == 0)
                {
                    m_state_lock.lock();
                    continue;
                }
                ProcessPacket(id);
            }
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
            cout << "ingame data recv!!! \n";

            //m_server_event 다시 죽여야하는데 어디서?
            // 전체 데이터 송신
            //DoSend();

            m_state_lock.lock();
        }
        m_state_lock.unlock();
        //수신 성공시 suspend thread
        //모든 클라이언트로부터 recv 혹은 timeout시 서버 연산
        //이후 resume thread


    }
}

void CMainServer::ServerThread()
{
    int ret;
    int client_count;
    for (;;)
    {  //In Robby
        client_count = 0;
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::CUSTOMIZE)
        {
            m_state_lock.unlock();
            for (auto& cl : m_clients)
            {
                cl.StateLock();
                if (cl.GetState() != ST_FREE)
                    client_count++;
                cl.StateUnlock();
            }

            if(client_count <= MAX_CLIENTS)
                DoAccept();
            m_state_lock.lock();
        }
        m_state_lock.unlock();
        cout << "Now InGame loop \n";
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
            DoSend();
            auto time_t = chrono::high_resolution_clock::now();
            if (time_t > m_server_timer)
            {
                // server process overflowed
            }
            else
                Sleep(chrono::duration_cast<chrono::milliseconds>(m_server_timer - time_t).count());
            SetEvent(m_server_event);
            cout << "server process!" << endl;
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
        cout << "Client [" << int(client_id) << "] name : " << m_clients[client_id].GetName() << endl;
        sc_packet_login_ok sp;
        sp.size = sizeof(sc_packet_login_ok);
        sp.type = SC_PACKET_LOGIN_OK;
        sp.id = client_id;
        sp.is_ready = false;
        sp.body_color = m_clients[client_id].GetPlayer().GetBodyColor();
        sp.cloth_color = m_clients[client_id].GetPlayer().GetClothColor();

        send(m_clients[client_id].GetSocket(), (char*)&sp, sizeof(sc_packet_login_ok), 0);
        cout << "Client [" << int(client_id) << "] : " << "login ok send!\n";
        m_clients[client_id].StateLock();
        m_clients[client_id].SetState(ST_INROBBY);
        m_clients[client_id].StateUnlock();

        //다른 플레이어들에게 로그인했음을 알림.
        //for ingame debug
        if (int(client_id) == 2)
            m_game_state = SCENE::ID::STAGE;

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
            if (other.GetID() == client_id)
                continue;

            other.StateLock();
            if (ST_INROBBY == other.GetState() || ST_READY == other.GetState())
            {
                other.StateUnlock();

                ttt = send(other.GetSocket(), (char*)&packet, sizeof(sc_packet_login_other_client), 0);
                cout << "me -> other : " << ttt << endl;
            }
            else
            {
                other.StateUnlock();
                continue;
            }


        }

        // 접속한 대상에게 다른 클라이언트 정보도 다 보냄!
        for (auto& cl : m_clients)
        {
            sc_packet_login_other_client o_packet;
            if (cl.GetID() == client_id)
                continue;

            cl.StateLock();
            if (ST_INROBBY == cl.GetState() || ST_READY == cl.GetState())
            {
                cl.StateUnlock();

                cl.StateLock();
                if (m_clients[cl.GetID()].GetState() == ST_READY)
                    o_packet.is_ready = true;
                else if (m_clients[cl.GetID()].GetState() == ST_INROBBY)
                    o_packet.is_ready = false;
                else cout << "STATE ERROR" << endl;
                cl.StateUnlock();

                o_packet.id = cl.GetID();
                strcpy_s(o_packet.name, cl.GetName());
                o_packet.size = sizeof(sc_packet_login_other_client);
                o_packet.body_color = cl.GetPlayer().GetBodyColor();
                o_packet.cloth_color = cl.GetPlayer().GetClothColor();
                o_packet.type = SC_PACKET_LOGIN_OTHER_CLIENT;

                ttt = send(m_clients[client_id].GetSocket(), (char*)&o_packet, sizeof(sc_packet_login_other_client), 0);
                cout << "other -> me : " << ttt << endl;
            }
            else
            {
                cl.StateUnlock();
                continue;
            }
        }

    }


    else if (packet_type == CS_PACKET_CHANGE_COLOR)
    {
        cout << "Client [" << int(client_id) << "] : " << "Change Color\n";

        cs_packet_change_color rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_change_color));

        // 서버에 id에 해당하는 플레이어 커마정보 저장
        m_clients[client_id].GetPlayer().SetBodyColor(rp.body_color);
        m_clients[client_id].GetPlayer().SetClothColor(rp.cloth_color);

        sc_packet_change_color sp;
        sp.size = sizeof(sc_packet_change_color);
        sp.type = SC_PACKET_CHANGE_COLOR;
        sp.id = client_id;
        sp.body_color = rp.body_color;
        sp.cloth_color = rp.cloth_color;

        for (auto& other : m_clients)
        {
            other.StateLock();
            CLIENT_STATE temp_state = other.GetState();
            other.StateUnlock();
            if ((ST_INROBBY == temp_state) || ((ST_READY == temp_state))) {
                send(other.GetSocket(), (char*)&sp, sizeof(sc_packet_change_color), 0);
                cout << "Client [" << int(client_id) << "] send change color ->  Client [" << int(other.GetID()) << "]\n";
            }
        }
    }

    else if (packet_type == CS_PACKET_READY)
    {
        cout << "Client [" << int(client_id) << "] : " << "Ready \n";

        cs_packet_ready rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_ready));

        if (rp.is_ready == true)
        {
            m_clients[client_id].StateLock();
            if (m_clients[client_id].GetState() == ST_INROBBY)
                m_clients[client_id].SetState(ST_READY);
            else
                cout << "ready_error_no1" << endl;
            m_clients[client_id].StateUnlock();
        }
        else
        {
            m_clients[client_id].StateLock();
            if (m_clients[client_id].GetState() == ST_READY)
                m_clients[client_id].SetState(ST_INROBBY);
            else
                cout << "ready_error_no2" << endl;
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
            sc_packet_all_ready sp;
            sp.size = sizeof(sc_packet_all_ready);
            sp.type = SC_PACKET_ALL_READY;     
            for (auto& cl : m_clients)
                send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_all_ready), 0);

            m_game_state = SCENE::STAGE;
            m_state_lock.unlock();


        }
    }
    else if (packet_type == CS_PACKET_PLAYER_INFO)
    {
        cs_packet_player_info rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_player_info));
        cout << "client [" << int(client_id) << "] recv : CS_PACKET_PLAYER_INFO \n";
        m_clients[client_id].GetPlayer().SetPosition(rp.m_position);
        m_clients[client_id].GetPlayer().SetLook(rp.m_look);
    }
    else if (packet_type == CS_PACKET_SHOOT_BULLET)
    {
    cs_packet_shoot_bullet rp;
    memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_shoot_bullet));
    cout << "client [" << int(client_id) << "] recv : CS_PACKET_SHOOT_BULLET \n";

    for (int i=0;i>MAX_BULLETS;++i)
    {
    
        m_bullets[i].StateLock();
        if (m_bullets[i].GetState() == OBJECT_STATE::ST_ALIVE)
        {
            m_bullets[i].StateUnlock();
            continue;
        }
        else if (m_bullets[i].GetState() == OBJECT_STATE::ST_FREE)
        {
            m_bullets[i].SetState(OBJECT_STATE::ST_ALIVE);
            m_bullets[i].StateUnlock();
            m_bullets[i].SetType(rp.bullet_type);
            m_bullets[i].SetID(i);
            m_bullets[i].SetLook(rp.look);
            m_bullets[i].SetPosition(rp.position);

            sc_packet_put_bullet sp;
            sp.bullet_id = i;
            sp.bullet_type = rp.bullet_type;
            sp.look = rp.look;
            sp.position = rp.position;
            for (auto& client : m_clients)
            {
                send(client.GetSocket(), (char*)&sp, sizeof(sc_packet_put_bullet), 0);
            }
            break;  
        }
        // bullet 최대개수가 꽉참
        cout << "BULLET_OVERFLOW" << endl;
    }

    }
    else
    {

    }
}

void CMainServer::DoSend()
{
    sc_packet_game_state sp;
    
    for(int i = 0; i < 3; ++i)
    {
        sp.player[i].look = m_clients[i].GetPlayer().GetLook();
        sp.player[i].id = m_clients[i].GetID();
        sp.player[i].position = m_clients[i].GetPlayer().GetPosition();
        sp.size = sizeof(sc_packet_game_state);
        sp.type = SC_PACKET_GAME_STATE;
    }
    for (auto& sc : m_clients)
    {
        send(sc.GetSocket(), (char*)&sp, sizeof(sc_packet_game_state), 0);
        cout << "ingame data send! \n";
    }
    // 여기서 플레이어들 정보 취합후 일괄 전송
    //for (auto& cl : m_clients)
    //    send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_game_state), 0);
};

int CMainServer::DoRecv(char id)
{
    char* buf = m_clients[id].GetBuf();
    int received;
    int left;
    bool first_recv = true;
    do
    {
        received = recv(m_clients[id].GetSocket(), buf, BUF_SIZE, 0);
        if (first_recv == true)
        {
            left = buf[0];
            first_recv = false;
        }
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        buf += received;
    } while (left > 0);
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

        if (m_client_threads.size() < 3) {
            if (int(new_id) >= m_client_threads.size())
                m_client_threads.emplace_back(&CMainServer::ClientThread, this, new_id);
        }
        cout << "[" << int(new_id) << "] : client incoming \n";
        //플레이어 초기 정보 세팅
        //login_ok패킷 전송
    }
    else
    {
        // clients full
    }
};

void CMainServer::ServerProcess() {

    CollisionCheckTerrainPlayer();
    CollisionCheckPlayerBullet();
    CollisionCheckTerrainBullet();
    CollisionCheckPlayerChest();
};

void CMainServer::CollisionCheckTerrainPlayer()
{

}

void CMainServer::CollisionCheckTerrainBullet()
{
    for (auto& bullet : m_bullets)
    {
        //벽 충돌시

        //총알 삭제
    }
}

void CMainServer::CollisionCheckPlayerBullet()
{
    for (auto& client : m_clients)
    {
        CPlayer& player = client.GetPlayer();
        for (auto& bullet : m_bullets)
        {
            if (CollisionCheck(bullet, player) == true)
            {
                //플레이어 체력 감소

                //플레이어 사망 판정

                //불릿 삭제
            }
            else
            {
                continue;
            }
        }
    }
}

void CMainServer::CollisionCheckPlayerChest()
{
    for (auto& client : m_clients)
    {
        CPlayer& player = client.GetPlayer();
        for (auto& chest : m_chests)
        {
            if (CollisionCheck(chest, player) == true)
            {
                //무기 선택 (랜덤? 사전 설정?)

                //플레이어 무기 변경

                //아이템 삭제
            }
            else
            {
                continue;
            }
        }
    }
}

template<class T1, class T2 >
bool CMainServer::CollisionCheck(T1& object_1, T2& object_2)
{
    if (abs(object_1.GetPosition().x - object_2.GetPosition().x) <=
        (object_1.vGetWidthHf() + object_2.vGetWidthHf()))
        return true;
    if (abs(object_1.GetPosition().y - object_2.GetPosition().y) <=
        (object_1.vGetHeightHf() + object_2.vGetHeightHf()))
        return true;

    return false;
}

void CMainServer::InitBullets()
{
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        m_bullets[i].SetID(i);
        m_bullets[i].SetState(OBJECT_STATE::ST_FREE);
    }
}

void CMainServer::InitChests()
{
    for (int i = 0; i < MAX_CHESTS; ++i)
    {
        // 초기 생성
    }
}


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
        if (m_clients[i].GetState() != ST_READY)
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
    cout << "Client [" << int(id) << "] : " << "Disconnect \n";
    m_clients[id].StateLock();
    closesocket(m_clients[id].GetSocket());
    m_clients[id].SetState(ST_FREE);
    for (auto& cl : m_clients)
    {
        if (cl.GetID() == id)
            continue;

        sc_packet_remove_object sp;
        sp.id = id;
        sp.size = sizeof(sc_packet_remove_object);
        sp.type = SC_PACKET_REMOVE_OBJECT;

        send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_remove_object), 0); // 연결을 종료하면 클라쪽에서도 안보여야 정상일듯?
    }
    m_clients[id].StateUnlock();
}
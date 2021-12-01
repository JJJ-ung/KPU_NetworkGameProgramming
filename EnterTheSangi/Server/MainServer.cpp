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
        svector2 temp;
        temp.x = 0;
        temp.y = 0;
        m_clients[i].GetPlayer().SetPosition(temp);
        m_clients[i].GetPlayer().SetLook(0);
        m_clients[i].GetPlayer().SetHealth(10);
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

    m_PerformanceCounter.reset();
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
                //cout << "Client [" << int(id) << "] recv data\n";
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
            float fDelta = m_PerformanceCounter.update();
            float fps = 1 / fDelta;
            cout << "fps : " << fps << endl;
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

            //m_server_event 다시 죽여야하는데 어디서?
            // 전체 데이터 송신
            DoSend(id);

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
        
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::CUSTOMIZE)
        {
            client_count = 0;
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

            

            ServerProcess();

            auto time_t = chrono::high_resolution_clock::now();
            if (time_t > m_server_timer)
            {
                // server process overflowed
            }
            else
                Sleep(chrono::duration_cast<chrono::milliseconds>(m_server_timer - time_t).count());
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

       
            
            // 초기 상자 정보를 전송한다.

            sc_packet_put_chest packet_put_chest;
            packet_put_chest.type = SC_PACKET_PUT_CHEST;
            packet_put_chest.size = sizeof(sc_packet_put_chest);
            for (auto& chest : m_chests)
            {
                packet_put_chest.chest_id = chest.GetID();
                packet_put_chest.position = chest.GetPosition();
                for (auto& client : m_clients)
                    send(client.GetSocket(), (char*)&packet_put_chest, sizeof(sc_packet_put_chest), 0);
            }
        }
    }
    else if (packet_type == CS_PACKET_PLAYER_INFO)
    {
        cs_packet_player_info rp;
        memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_player_info));

        m_clients[client_id].GetPlayer().SetPosition(rp.m_position);
        m_clients[client_id].GetPlayer().SetLook(rp.m_look);
        m_clients[client_id].SetPlayerState(rp.m_state);
    }
    else if (packet_type == CS_PACKET_SHOOT_BULLET)
    {
    cs_packet_shoot_bullet rp;
    memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_shoot_bullet));
    cout << "client [" << int(client_id) << "] recv : CS_PACKET_SHOOT_BULLET \n";

    for (int i=0;i<MAX_BULLETS;++i)
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
            //m_bullets[i].SetLook(rp.look);
            m_bullets[i].SetPosition(rp.position);
            m_bullets[i].SetLook(rp.angle);
           // 서버에서 가지는 총알포지션 값에 델타타임 적용 필요 (서버 충돌체크용)


            sc_packet_put_bullet sp;
            sp.size = sizeof(sc_packet_put_bullet);
            sp.type = SC_PACKET_PUT_BULLET;
            sp.bullet_type = rp.bullet_type;
            sp.angle = rp.angle;
            sp.direction = rp.direction;
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

void CMainServer::DoSend(char client_id)
{
    sc_packet_game_state sp;
    
    
    for(int i = 0; i < MAX_CLIENTS; ++i)
    {
        sp.player[i].look = m_clients[i].GetPlayer().GetLook();
        sp.player[i].position = m_clients[i].GetPlayer().GetPosition();
        sp.player[i].state = m_clients[i].GetPlayerState();
        sp.size = sizeof(sc_packet_game_state);
        sp.type = SC_PACKET_GAME_STATE;

        // 죽은건 체력 0이면 클라에서 처리하자!
    }
    send(m_clients[client_id].GetSocket(), (char*)&sp, sizeof(sc_packet_game_state), 0);
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

    
    cout << "client [" << int(id) << "] -> " << (void*)m_clients[id].GetBuf() << endl;
    return received;
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

        if (m_client_threads.size() < MAX_CLIENTS) {
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

void CMainServer::ServerProcess() 
{
    //CollisionCheckTerrainPlayer();
    CollisionCheckPlayerBullet();
    CollisionCheckTerrainBullet();
    CollisionCheckPlayerChest();
};

void CMainServer::CollisionCheckTerrainPlayer()
{
    //클라이언트에서 합니다
}

void CMainServer::CollisionCheckTerrainBullet()
{
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        if (TerrainCollisionCheck(m_bullets[i]))//벽 충돌시
        {
            //총알 삭제
            m_bullets[i].StateLock();
            m_bullets[i].SetState(OBJECT_STATE::ST_FREE);
            m_bullets[i].StateUnlock();


            sc_packet_remove_bullet sp;
            sp.type = SC_PACKET_REMOVE_BULLET;
            sp.size = sizeof(sc_packet_remove_bullet);
            sp.bullet_id = i;

            for (auto& client : m_clients)
            {
                send(client.GetSocket(), (char*)&sp, sizeof(sc_packet_remove_bullet), 0);
            }
        }
    }
}

void CMainServer::CollisionCheckPlayerBullet()
{
    for (auto& client : m_clients)
    {       
        CPlayer& player = client.GetPlayer();
        // 서버 쓰레드(싱글)에서 처리하므로 player 스테이트락 사용x    
        if (player.GetState() == STATE::TYPE::DEAD)
            continue;

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (CollisionCheck(m_bullets[i], player) == true )
            {            
                //플레이어 체력 감소
                // health를 여기서 감소시킬건데 총알 타입에 따른 데미지를 받아오는 친구가 있나..?

                char dmg;
                //dmg=m_bullets[i].~~~~~~~  // 총알 종류에 따른 데미지값
                player.ChangeHealth(-dmg);               

                //플레이어 사망 판정
                if (player.GetHealth()==0)
                    player.SetState(STATE::TYPE::DEAD);
                

                //불릿 삭제
                m_bullets[i].StateLock();
                m_bullets[i].SetState(OBJECT_STATE::ST_FREE);
                m_bullets[i].StateUnlock();

                sc_packet_remove_bullet sp;
                sp.type = SC_PACKET_REMOVE_BULLET;
                sp.size = sizeof(sc_packet_remove_bullet);
                sp.bullet_id = m_bullets[i].GetID();

                for (auto& client : m_clients)
                {
                    send(client.GetSocket(), (char*)&sp, sizeof(sc_packet_remove_bullet), 0);
                }
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
        // 서버 쓰레드(싱글)에서 처리하므로 player 스테이트락 사용x    
        if (player.GetState() == STATE::TYPE::DEAD)
            continue;
 
        for (auto& chest : m_chests)
        {
            // 서버 쓰레드(싱글)에서 처리하므로 chset 스테이트락 사용x            
            if (CollisionCheck(chest, player) == true)
            {
                //무기 선택 (랜덤? 사전 설정?)

                //플레이어 무기 변경

                //아이템 삭제 후 재생성 (실제로는 이동)
                //맵 좌표 받고 좌표 생성 로직 최신화 필요
                chest.SetPosition({ (short)(rand() % 1000), (short)(rand() % 1000) });

                sc_packet_move_chest sp;
                sp.type = SC_PACKET_MOVE_CHEST;
                sp.size = sizeof(sc_packet_move_chest);
                sp.chest_id = chest.GetID();
                sp.position = chest.GetPosition();
                for (auto& client : m_clients)
                    send(client.GetSocket(), (char*)&sp, sizeof(sc_packet_move_chest), 0);
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

template<class T1 >
bool CMainServer::TerrainCollisionCheck(T1& object_1) // 맵 안에 있으면 false, 벽이랑 닿거나 넘어가면 true
{
    for (auto& map_rect : m_map_rects)
    {
        //UP
        if ((object_1.GetPosition().y - object_1.vGetHeightHf()) <= map_rect.pos_1.y) return true;
        //DOWN
        if ((object_1.GetPosition().y + object_1.vGetHeightHf()) >= map_rect.pos_2.y) return true;
        //LEFT
        if ((object_1.GetPosition().x - object_1.vGetWidthHf()) <= map_rect.pos_1.x) return true;
        //RIGHT
        if ((object_1.GetPosition().x + object_1.vGetWidthHf()) >= map_rect.pos_2.x) return true;
    }
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
		m_chests[i].SetID(i);
        m_bullets[i].SetState(OBJECT_STATE::ST_ALIVE);
		m_chests[i].SetPosition(svector2{ (short)(i + 1) ,(short)(i + 1) });
	}
}

void CMainServer::InitPlayers()
{
    for (auto& cl : m_clients)
    {
        CPlayer& player = cl.GetPlayer();
            player.SetState(STATE::TYPE::IDLE);
            player.SetHealth(PLAYER_MAX_HP);  
    }
    //플레이어별 초기 좌표 설정 
}

void CMainServer::InitMapRects()
{
	// map_rect 값 넣어서 초기화
	m_map_rects[0] = { {},{} };
	m_map_rects[1] = { {},{} };
	m_map_rects[2] = { {},{} };
	m_map_rects[3] = { {},{} };
	m_map_rects[4] = { {},{} };

	//m_map_rects[MAX_MAP_RECT]
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
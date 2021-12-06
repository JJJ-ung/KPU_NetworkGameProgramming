#pragma once
#include "MainServer.h"

CMainServer::CMainServer() {};
CMainServer::~CMainServer() {};
void CMainServer::Init(const int server_port)
{
    // 무기 데이터 파일 로드
    wifstream fin;
    fin.open("../Binary/Data/info_Weapon.txt");
    if (fin.fail())
        cout << "file load fail \n";
    while (true)
    {
        WEAPON t;
        fin >> t.type >> t.damage >> t.shotspeed >> t.bulletspeed >> t.duration >> t.bulletoffset.x >> t.bulletoffset.y >> t.size;
        if (fin.eof()) break;
        m_weapon_info.emplace_back(t);
    }

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

    for (int i = 0; i < MAX_CLIENTS; ++i)
        hEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

    InitMapRects();
    InitRandomSpawner();
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

    //accept만을 위한 스레드. 주스레드에서 accpet 상관 없이 서버 연산을 돌리기 위함.
    for (int i = 0; i < 1; ++i)
        m_server_threads.emplace_back(&CMainServer::ServerThread, this);
    for (auto& th : m_server_threads)
        th.join();
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

        this_thread::sleep_for(100ms);

		sc_packet_put_chest packet_put_chest;
		packet_put_chest.type = SC_PACKET_PUT_CHEST;
		packet_put_chest.size = sizeof(sc_packet_put_chest);
		for (auto& chest : m_chests)
		{
			packet_put_chest.chest_id = chest.GetID();
			packet_put_chest.position = chest.GetPosition();
			packet_put_chest.weapon_id = chest.GetWeaponID();

			send(m_clients[id].GetSocket(), (char*)&packet_put_chest, sizeof(sc_packet_put_chest), 0);
		}

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

            //m_server_event 다시 죽여야하는데 어디서?
            // 전체 데이터 송신

            SetEvent(hEvent[id]);
            m_state_lock.lock();
        }
        m_state_lock.unlock();
        
        //게임 종료시 
        //어케?
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::END)
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
            //ProcessPacket(id);

            m_state_lock.lock();
        }
        m_state_lock.unlock();
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

            if(client_count < MAX_CLIENTS)
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
            if (m_PerformanceCounter.Frame_Limit(30.f)) {
                ServerProcess();
                WaitForMultipleObjects(3, hEvent, TRUE, INFINITE);
                for (int i = 0; i < MAX_CLIENTS; ++i)
                {
                    DoSend(i);
                    ResetEvent(hEvent[i]);
                }
            }
           m_state_lock.lock();
        }
        m_state_lock.unlock();

        //Game End      
        //게임 종료시 리셋 후 다시? 어캐?
        m_state_lock.lock();
        while (m_game_state == SCENE::ID::END)
        {
            m_state_lock.unlock();



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
                send(other.GetSocket(), (char*)&packet, sizeof(sc_packet_login_other_client), 0);
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

                send(m_clients[client_id].GetSocket(), (char*)&o_packet, sizeof(sc_packet_login_other_client), 0);
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

            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                CPlayer& player = m_clients[i].GetPlayer();
        
                player.StateLock();
                sp.player[i].state = player.GetState();
                sp.player[i].position = player.GetPosition();
                sp.player[i].look = player.GetLook();
                sp.player[i].weapon = player.GetWeapon();
                sp.player[i].health = player.GetHealth();
                player.StateUnlock();
            }
            for (auto& cl : m_clients)
                send(cl.GetSocket(), (char*)&sp, sizeof(sc_packet_all_ready), 0);
                          
            
            // 초기 상자 정보를 전송한다.

           
            m_game_state = SCENE::STAGE;
            m_state_lock.unlock();
        }
    }
    else if (packet_type == CS_PACKET_PLAYER_INFO)
    {
	cs_packet_player_info rp;
	memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_player_info));

	CPlayer& player = m_clients[client_id].GetPlayer();

	player.SetPosition(rp.m_position);
	player.SetLook(rp.m_look);
    player.StateLock();
	player.SetState(STATE::TYPE(rp.m_state));
	player.StateUnlock();
	}

    else if (packet_type == CS_PACKET_SHOOT_BULLET)
    {
        if (m_clients[client_id].GetPlayer().GetHealth() > 0)
        {
            cs_packet_shoot_bullet rp;
            memcpy(&rp, m_clients[client_id].GetBuf(), sizeof(cs_packet_shoot_bullet));
            cout << "client [" << int(client_id) << "] recv : CS_PACKET_SHOOT_BULLET \n";

            for (int i = 0; i < MAX_BULLETS; ++i)
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
                    m_bullets[i].SetID(rp.id);
                    m_bullets[i].SetBulletType(rp.bullet_type);
                    //m_bullets[i].SetLook(rp.look);
                    m_bullets[i].SetPosition(rp.position);
                    m_bullets[i].SetBulletPosition(D3DXVECTOR3{ (float)rp.position.x, (float)rp.position.y, 0.f });
                    cout << "rp pos : (" << rp.position.x << ", " << rp.position.y << ") \n";
                    m_bullets[i].SetLook(rp.angle);
                    m_bullets[i].SetDirection({ (float)rp.direction.x * 0.00001f, (float)rp.direction.y * 0.00001f, 0.f });
                    // 서버에서 가지는 총알포지션 값에 델타타임 적용 필요 (서버 충돌체크용)
                    for (auto& weapon : m_weapon_info)
                    {
                        if (weapon.type == rp.bullet_type)
                        {
                            m_bullets[i].SetBulletSpeed(weapon.bulletspeed);
                            m_bullets[i].SetBulletAliveTime(weapon.duration);
                            m_bullets[i].SetUpdateBulletTime(0);
                            break;
                        }
                    }
                    m_bullets[i].StateUnlock();

                    cout << "new_bullet, id: " << (int)i << endl;
                    cout << m_bullets[i].GetPosition().x << ", " << m_bullets[i].GetPosition().y << endl;

                    sc_packet_put_bullet sp;
                    sp.size = sizeof(sc_packet_put_bullet);
                    sp.type = SC_PACKET_PUT_BULLET;
                    sp.bullet_type = rp.bullet_type;
                    sp.bullet_id = i;
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
    }
    else
    {

	}
}

void CMainServer::DoSend(char client_id)
{
	sc_packet_game_state sp;


	for (int i = 0; i < MAX_CLIENTS; ++i)
	{      
		CPlayer& player = m_clients[i].GetPlayer();

		player.StateLock();
		sp.player[i].state = player.GetState();
		sp.player[i].position = player.GetPosition();
		sp.player[i].look = player.GetLook();
		sp.player[i].weapon = player.GetWeapon();
		sp.player[i].health = player.GetHealth();
        player.StateUnlock();

		sp.size = sizeof(sc_packet_game_state);
		sp.type = SC_PACKET_GAME_STATE;

		// 죽은건 체력 0이면 클라에서F 처리하자!
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
    CollisionCheckPlayerBullet();
    CollisionCheckTerrainBullet();
    CollisionCheckPlayerChest();
    UpdateBullet();
    CheckGameEnd();
};

void CMainServer::UpdateBullet()
{
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        m_bullets[i].StateLock();
        if (m_bullets[i].GetState() == OBJECT_STATE::ST_FREE)
        {
            m_bullets[i].StateUnlock();
            continue;
        }
        m_bullets[i].StateUnlock();
        D3DXVECTOR3 m_vDir = m_bullets[i].GetDirection();
        
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        D3DXVECTOR3 m_vPosition = m_bullets[i].GetBulletPosition();
        float deltaTime = 1.f / 30.f;
        m_vPosition += m_vDir * m_bullets[i].GetBulletSpeed() * deltaTime;
        m_bullets[i].SetBulletPosition(m_vPosition);
        m_bullets[i].SetPosition({ (short)m_vPosition.x, (short)m_vPosition.y });
        
        m_bullets[i].SetUpdateBulletTime(m_bullets[i].GetUpdateBulletTime() + deltaTime);

        if (m_bullets[i].GetUpdateBulletTime() >= m_bullets[i].GetBulletAliveTime()) // 이경우 지속시간이 초과된 경우! 삭제함
        {
            m_bullets[i].StateLock();
            m_bullets[i].SetState(OBJECT_STATE::ST_FREE);
            m_bullets[i].StateUnlock();
            m_bullets[i].SetBulletAliveTime(0);
            m_bullets[i].SetUpdateBulletTime(0);
            cout << "time out remove_bullet, id: " << (int)i << endl;

            sc_packet_remove_bullet sp;
            sp.type = SC_PACKET_REMOVE_BULLET;
            sp.size = sizeof(sc_packet_remove_bullet);
            sp.bullet_id = i;
        }

    }
}

void CMainServer::CollisionCheckTerrainBullet()
{
    for (int i = 0; i < MAX_BULLETS; ++i)
    {
        m_bullets[i].StateLock();
        if (m_bullets[i].GetState() == OBJECT_STATE::ST_FREE)
        {
            m_bullets[i].StateUnlock();
            continue;
        }
        m_bullets[i].StateUnlock();
        if (TerrainCollisionCheck(m_bullets[i]))//벽 충돌시
        {
            //총알 삭제
            m_bullets[i].StateLock();
            m_bullets[i].SetState(OBJECT_STATE::ST_FREE);
            m_bullets[i].StateUnlock();

            cout << "remove_bullet, id: " << (int)i << endl;
            cout << m_bullets[i].GetPosition().x << ", " << m_bullets[i].GetPosition().y << endl;

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
            m_bullets[i].StateLock();
            if (m_bullets[i].GetState() == OBJECT_STATE::ST_FREE)
            {
                m_bullets[i].StateUnlock();
                continue;
            }
            m_bullets[i].StateUnlock();

            if (BulletCollisionCheck(m_bullets[i], player) == true && (m_bullets[i].GetID() != client.GetID()) && player.GetHealth() > 0)
            {            
                
                //플레이어 체력 감소
                   
                player.ChangeHealth(-m_weapon_info[m_bullets[i].GetBulletType()].damage);
                cout << "player id : " << (int)client.GetID() << "HP : " << player.GetHealth() << endl;
                //플레이어 사망 판정
                if (player.GetHealth() <= 0)
                {
                    player.StateLock();
                    player.SetState(STATE::TYPE::DEAD);
                    player.StateUnlock();
                }

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
                //플레이어 무기 랜덤 변경
                player.SetWeapon(chest.GetWeaponID());
                
                sc_packet_change_weapon weapon_change_packet;
                weapon_change_packet.size = sizeof(weapon_change_packet);
                weapon_change_packet.type = SC_PACKET_CHANGE_WEAPON;
                weapon_change_packet.id = client.GetID();
                weapon_change_packet.weapon_id = player.GetWeapon();
                for (auto& client : m_clients)
                    send(client.GetSocket(), (char*)&weapon_change_packet, sizeof(sc_packet_change_weapon), 0);
                                        
            

                //아이템 삭제 후 재생성 (실제로는 이동)
                //맵 좌표 받고 좌표 생성 로직 최신화 필요
                chest.SetPosition(GetRandomPosition());
                chest.SetWeaponID(rand() % MAX_WEAPON);

                sc_packet_move_chest sp;
                sp.type = SC_PACKET_MOVE_CHEST;
                sp.size = sizeof(sc_packet_move_chest);
                sp.chest_id = chest.GetID();
                sp.position = chest.GetPosition();
                sp.weapon_id = chest.GetWeaponID();
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
    //UP
    if ((object_1.GetCollisionBox().pos_2.y) < object_2.GetCollisionBox().pos_1.y) return false;
    //DOWN
    if ((object_1.GetCollisionBox().pos_1.y) > object_2.GetCollisionBox().pos_2.y) return false;
    //LEFT
    if ((object_1.GetCollisionBox().pos_2.x) < object_2.GetCollisionBox().pos_1.x) return false;
    //RIGHT
    if ((object_1.GetCollisionBox().pos_1.x) > object_2.GetCollisionBox().pos_2.x) return false;

	return true;
}

template<class T1 >
bool CMainServer::BulletCollisionCheck(CBullet& bullet, T1& object_1) // 회전한 bullet - object간 
{
	fvector2 vector[4];
	D3DXVECTOR2 bullet_vector = m_weapon_info[bullet.GetBulletType()].bulletoffset;
	vector[0].x = bullet.GetDirection().x * bullet_vector.y;
	vector[0].y = bullet.GetDirection().y * bullet_vector.y;
	vector[1].x = bullet.GetDirection().x * bullet_vector.x;
	vector[1].y = bullet.GetDirection().y * bullet_vector.x;
	vector[2].y = (object_1.GetCollisionBox().pos_2.y - object_1.GetCollisionBox().pos_1.y) / 2;
	vector[3].x = (object_1.GetCollisionBox().pos_2.x - object_1.GetCollisionBox().pos_1.x) / 2;

    fvector2 dist{ bullet.GetPosition() - object_1.GetPosition() };
	for (int i = 0; i < 4; ++i)
    {     
        fvector2 unit_vector{ vector[i].GetUnitVector() };
        float sum = 0.f;
        for (int j = 0; j < 4; ++j)
        {
            sum += vector[j].AbsDotVector(unit_vector);
        }
        if (dist.AbsDotVector(unit_vector) > sum)
            return false;
    }
    return true;
}

template<class T1 >
bool CMainServer::TerrainCollisionCheck(T1& object_1) // 맵 안에 있으면 false, 벽이랑 닿거나 넘어가면 true
{
	for (auto& map_rect : m_map_rects)
	{
		//UP
        if (((object_1.GetCollisionBox().pos_1.y) > map_rect.pos_1.y) &&
            //DOWN
            ((object_1.GetCollisionBox().pos_2.y) < map_rect.pos_2.y) &&
            //LEFT
            ((object_1.GetCollisionBox().pos_1.x) > map_rect.pos_1.x) &&
            //RIGHT
            ((object_1.GetCollisionBox().pos_2.x) < map_rect.pos_2.x))
            return false;
	}
    return true;
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
        m_chests[i].SetState(OBJECT_STATE::ST_ALIVE);
        m_chests[i].SetPosition(GetRandomPosition());
        m_chests[i].SetWeaponID(rand() % MAX_WEAPON);      
	}
}

void CMainServer::InitPlayers()
{
    for (auto& cl : m_clients)
    {
        CPlayer& player = cl.GetPlayer();
            player.SetState(STATE::TYPE::IDLE);
			player.SetHealth(PLAYER_MAX_HP);
            player.SetPosition(GetRandomPosition());
            player.SetWeapon(-1);
	}
	//플레이어별 초기 좌표 설정 
}

void CMainServer::InitMapRects()
{
	// map_rect 값 넣어서 초기화
	m_map_rects[0] = { {-288 * 4,-627 * 4},{288 * 4,-245 * 4} };  //1번방
	m_map_rects[1] = { {-32 * 4,-245 * 4},{32 * 4,-128 * 4} };    //1번 복도
	m_map_rects[2] = { {-368 * 4,-128 * 4},{368 * 4,205 * 4} };   //2번방
	m_map_rects[3] = { {-36 * 4,205 * 4},{36 * 4,355 * 4} };      //2번 복도
	m_map_rects[4] = { {-353 * 4,355 * 4},{353 * 4,738 * 4} };    //3번방

	//m_map_rects[MAX_MAP_RECT]
}

void CMainServer::InitRandomSpawner()
{
    for (int i = 0; i < MAX_MAP_RECT; ++i)
    {
        m_max_spawn_point_by_rect[i] = (m_map_rects[i].pos_2.x - m_map_rects[i].pos_1.x - CHEST_WIDTH) *
            (m_map_rects[i].pos_2.y - m_map_rects[i].pos_1.y - CHEST_HEIGHT);
        m_max_spawn_point += m_max_spawn_point_by_rect[i];
    }
    //cout << m_max_spawn_point << endl;
}

char CMainServer::GetRandomWeapon()
{
    uniform_int_distribution<int> weapon_uid{ 0, MAX_WEAPON };

    return weapon_uid(m_dre);
}

svector2 CMainServer::GetRandomPosition()
{
    uniform_int_distribution<int> uid{ 0, m_max_spawn_point };


	int rand_num = uid(m_dre);
	svector2 rst;

	for (int i = 0; i < MAX_MAP_RECT; ++i)
	{
		if (rand_num < m_max_spawn_point_by_rect[i])
		{
            uniform_int_distribution<int> uid_x{ 0, (m_map_rects[i].pos_2.x - m_map_rects[i].pos_1.x - (short)CHEST_WIDTH) };
            uniform_int_distribution<int> uid_y{ 0, (m_map_rects[i].pos_2.y - m_map_rects[i].pos_1.y - (short)CHEST_HEIGHT) };
            rst.x = m_map_rects[i].pos_1.x +uid_x(m_dre);
			rst.y = m_map_rects[i].pos_1.y+uid_y(m_dre);
			break;
		}
		else
            rand_num -= m_max_spawn_point_by_rect[i];
	}
    return rst;
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

void CMainServer::CheckGameEnd()
{
    char winner = -1;
    int alive_player_num = 0;
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
		if (m_clients[i].GetPlayer().GetHealth() > 0)
		{
			winner = i;
			alive_player_num += 1;
		}
	}

    cout << "alive_player_num : " << alive_player_num << endl;

	if (alive_player_num == 0) //전원 사망
	{
		sc_packet_game_end packet;
		packet.size = sizeof(sc_packet_game_end);
		packet.type = SC_PACKET_GAME_END;
		packet.winner_id = winner;
		for (auto& client : m_clients)
			send(client.GetSocket(), (char*)&packet, sizeof(sc_packet_game_end), 0);

        m_state_lock.lock();
        m_game_state = SCENE::ID::END;
        m_state_lock.unlock();

		cout << "Game End, No Winner" << endl;
	}
	else if (alive_player_num == 1) //1명 생존
	{
        sc_packet_game_end packet;
        packet.size = sizeof(sc_packet_game_end);
        packet.type = SC_PACKET_GAME_END;
        packet.winner_id = winner;
        for (auto& client : m_clients)
            send(client.GetSocket(), (char*)&packet, sizeof(sc_packet_game_end), 0);

        m_state_lock.lock();
        m_game_state = SCENE::ID::END;
        m_state_lock.unlock();

		cout << "Game End, Winner: " << (int)winner << endl;
	}
}
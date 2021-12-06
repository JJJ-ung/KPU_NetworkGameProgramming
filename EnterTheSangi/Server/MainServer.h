#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>
#include <thread>
#include <chrono>
#include <mutex>
#include <chrono>
#include <fstream>
#include <random>

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "../Binary/Librarys/x64/d3d9.lib")
#pragma comment(lib, "../Binary/Librarys/x64/d3dx9.lib")
#pragma comment(lib, "../Binary/Librarys/x64/dxgi.lib")
#pragma comment(lib, "../Binary/Librarys/x64/dxguid.lib")
#pragma comment(lib, "../Binary/Librarys/x64/dinput8.lib")

#include "Client.h"
#include "Protocol.h"
#include "Enum.h"
#include "GameObject.h"
#include "Chest.h"
#include "Bullet.h"
#include "Server_Timer.h"

class CMainServer
{
public:
	CMainServer();
	~CMainServer();

	void Init(const int server_port);
	void Activate();
	void Disconnect(char id);

	void ClientThread(char id);
	void ServerThread();

	void DoSend(char client_id);
	int  DoRecv(char id);
	int  DoAccept();
	void ProcessPacket(char id);

	char GetNewID();
	bool IsAllClientsReady();

	void ServerProcess();
	void CollisionCheckTerrainBullet();
	void CollisionCheckPlayerBullet();
	void CollisionCheckPlayerChest();

	template<class T1, class T2 >
	bool CollisionCheck(T1& object_1, T2& object_2);

	template<class T1 >
	bool BulletCollisionCheck(CBullet& bullet, T1& object_1);

	template<class T1>
	bool TerrainCollisionCheck(T1& object_1);

	void UpdateBullet();

	void InitBullets();
	void InitChests();
	void InitPlayers();
	void InitMapRects();
	void InitRandomSpawner();

	svector2 GetRandomPosition();
	char     GetRandomWeapon();
	void     CheckGameEnd();

private:
	SOCKET                   m_listen_socket;

	std::array<CClient, 3>   m_clients;
	std::vector<std::thread> m_client_threads;
	std::vector<std::thread> m_server_threads;
	chrono::high_resolution_clock
		::time_point         m_server_timer;
	HANDLE                   m_client_event[MAX_CLIENTS];
	HANDLE                   m_server_event;

	SCENE::ID                m_game_state;
	std::mutex               m_state_lock;
	Server_Timer			 m_PerformanceCounter;
	std::array<CChest,MAX_CHESTS>    m_chests;
	std::array<CBullet, MAX_BULLETS> m_bullets;
	std::array<srect, MAX_MAP_RECT>  m_map_rects;
	std::vector<WEAPON>				 m_weapon_info;

	HANDLE							 hEvent[3];

	default_random_engine    m_dre{ std::random_device{}() };

	int                      m_max_spawn_point = 2;
	std::array<int, MAX_MAP_RECT> m_max_spawn_point_by_rect;
};
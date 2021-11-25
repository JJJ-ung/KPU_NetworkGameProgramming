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
#include "Client.h"
#include "Protocol.h"
#include "Enum.h"
#include "GameObject.h"
#include "Chest.h"
#include "Bullet.h"

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

	void DoSend();
	int  DoRecv(char id);
	int  DoAccept();
	void ProcessPacket(char id);
	void ServerProcess();
	void CollisionCheckTerrainPlayer();
	void CollisionCheckTerrainBullet();
	void CollisionCheckPlayerBullet();
	void CollisionCheckPlayerChest();
	template<class T1, class T2 >
	bool CollisionCheck(T1& object_1, T2& object_2);

	char GetNewID();
	bool IsAllClientsReady();

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

	list<CChest>             m_chests;
	list<CBullet>            m_bullets;

};
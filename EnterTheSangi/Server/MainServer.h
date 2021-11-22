#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include "Client.h"
#include "Protocol.h"
#include "Enum.h"

class CMainServer
{
public:
	CMainServer();
	~CMainServer();

	void Init(const int server_port);
	void Activate();

	void Disconnect(char id);

	void ClientThread(char id);
	void AccpetThread();

	void ProcessPacket(char id);
	void DoSend();
	int  DoRecv(char id);
	int  DoAccept();


	char GetNewID();

private:
	SOCKET                   m_listen_socket;
	std::array<CClient, 3>   m_clients;
	std::vector<std::thread> m_client_threads;
	std::vector<std::thread> m_accpet_threads;

	SCENE::ID                m_game_state;
};
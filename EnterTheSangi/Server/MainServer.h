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

class CMainServer
{
public:
	CMainServer();
	~CMainServer();

	void Init(const int server_port);
	void Activate();

	void ClientThread();
	void AccpetThread();

	void DoSend();
	void DoRecv();
	void DoAccept();


private:
	SOCKET m_listen_socket;
	std::array<CClient, 3> m_clients;
	std::vector<std::thread> m_client_threads;
	std::vector<std::thread> m_accpet_threads;

	bool m_can_connect;        //MAX_CLIENT��ŭ�� Ŭ���̾�Ʈ�� �����ϸ� m_can_connect=false, ���̻� accpet�� ���� �ʴ´� 
};
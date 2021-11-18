#pragma once
#pragma comment(lib, "ws2_32")
#include "framework.h"
#include "..\Server\Protocol.h"
#include <winsock2.h>

class NetworkMGR {
private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_addr;
	WSADATA			m_wsa;
	int				retval;
	char			m_buf[BUF_SIZE];

public:
	NetworkMGR() {};
	~NetworkMGR() {};

	void Activate();
	
	// 수신, 전송
	void do_send();
	void do_recv();

};
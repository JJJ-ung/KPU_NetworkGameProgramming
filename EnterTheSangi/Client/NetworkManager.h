#pragma once
#pragma comment(lib, "ws2_32")
#include "framework.h"
#include "..\Server\Protocol.h"
#include <winsock2.h>

class NetworkMgr {
private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_addr;
	WSADATA			m_wsa;
	GameStatePlayer m_playerPacket;
	cs_change_color	m_player_color_pakcet;
	int				retval;

public:
	NetworkMgr() {};
	~NetworkMgr() {};

	void			err_quit(const char *msg);
	void			err_display(const char* msg);
	void			Activate();

	// ����, ����
	void			do_send();
	void			do_send_customizing();
	void			do_recv();

	// ����
	int				recvn(SOCKET s, char* buf, int len, int flags);
};
#pragma once

class NetworkMgr
{
	DECLARE_SINGLETON(NetworkMgr)

private:
	NetworkMgr();
	~NetworkMgr();

public:
	HRESULT Ready_WinSock();

public:
	void			err_quit(const char *msg);
	void			err_display(const char* msg);
	void			Activate();

	// ����, ����
	void			do_send();
	void			do_send_customizing();
	void			do_recv();

	// ����
	int			recvn(SOCKET s, char* buf, int len, int flags);

private:
	SOCKET						m_socket;
	SOCKADDR_IN			m_addr;
	WSADATA					m_wsa;
	GameStatePlayer		m_playerPacket;
	cs_packet_change_color		m_player_color_packet;
	int								retval;

public:
	void			Free();
};
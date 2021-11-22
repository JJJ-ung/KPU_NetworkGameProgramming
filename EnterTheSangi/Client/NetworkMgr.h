#pragma once

class NetworkMgr
{
	DECLARE_SINGLETON(NetworkMgr)

private:
	NetworkMgr();
	~NetworkMgr();

public:
	HRESULT Ready_WinSock();
	HRESULT Setup_Networking();

public:
	HRESULT Send_ClientInfo(GameStatePlayer& tPlayerPacket);
	HRESULT Send_CustomizeInfo(cs_packet_change_color& tColorPacket);

public:
	HRESULT Recv_ServerInfo(void* tRecvInfo);
	int			recvn(SOCKET s, char* buf, int len, int flags);

private:
	void Render_Error(const char* msg);

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
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
	HRESULT Send_LoginInfo(cs_packet_login& tLoginPacket);
	HRESULT Send_ClientInfo(sc_packet_game_state& tPlayerPacket);
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

public:
	void			Free();
};
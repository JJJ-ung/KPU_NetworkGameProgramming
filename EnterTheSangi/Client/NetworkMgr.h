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
	HRESULT Send_ClientInfo(char type, void* p);

	HRESULT Send_LoginInfo(const char* name);
	HRESULT Send_ColorInfo(D3DXVECTOR3 body, D3DXVECTOR3 cloth);
	HRESULT Send_ReadyInfo(bool ready);

public:
	char		Recv_ServerInfo(void* p = nullptr);
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
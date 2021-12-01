#pragma once

class GameMgr;
class NetworkMgr
{
	DECLARE_SINGLETON(NetworkMgr)

private:
	NetworkMgr();
	~NetworkMgr();

public:
	CRITICAL_SECTION* Get_Crt(void) { return &m_Crt; }
	BOOL Get_Finish(void) const { return m_bFinish; }

public:
	HRESULT Ready_WinSock();

// For Send /////////////////////////////////////////////////////////////////////////
public:
	HRESULT Send_ClientInfo(char type, void* p);
public:
	HRESULT Send_LoginInfo(const char* name);
	HRESULT Send_ColorInfo(D3DXVECTOR3 body, D3DXVECTOR3 cloth);
	HRESULT Send_ReadyInfo(bool ready);
	HRESULT Send_PlayerInfo(STATE::TYPE eState, D3DXVECTOR3 vPos, float fAngle);
	HRESULT Send_BulletInfo(int type, float angle, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);
///////////////////////////////////////////////////////////////////////////////////////

// For Recv /////////////////////////////////////////////////////////////////////////
public:
	static unsigned int CALLBACK Thread_Recv(void* pArg);
///////////////////////////////////////////////////////////////////////////////////////

private:
	void Render_Error(const char* msg);

private:
	GameMgr*					m_pGameMgr = nullptr;

private:
	SOCKET						m_socket;
	SOCKADDR_IN			m_addr;
	WSADATA					m_wsa;

private:
	HANDLE								m_hThread;
	CRITICAL_SECTION			m_Crt;
	BOOL									m_bFinish = false;

public:
	void			Free();
};
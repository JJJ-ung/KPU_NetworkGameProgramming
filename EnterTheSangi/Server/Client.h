#pragma once
#include <winsock2.h>
#include <mutex>
#include "Player.h"

//client state
enum CLIENT_STATE { ST_FREE, ST_ACCEPT, ST_INROBBY,ST_READY,ST_INGAME };

class CClient
{
public:
	CClient();
	CClient(const CClient& client);
	~CClient();

	void SetID(const int client_id);
	void SetName(char* name);
	void SetState(CLIENT_STATE state);
	void SetSocket(SOCKET socket);


	char GetID();
	CLIENT_STATE GetState();
	SOCKET GetSocket();
	CPlayer& GetPlayer();
	char* GetBuf();
	char* GetName();

	void StateLock();
	void StateUnlock();

	void SocketLock()
	{
		m_socket_lock.lock();
	}

	void SocketUnlock()
	{
		m_socket_lock.unlock();
	}
	

private:
	SOCKET         m_socket;
	SOCKADDR_IN    m_addr;
	char           m_buff[BUF_SIZE];
	char           m_ID;
	char		   m_name[MAX_NAME_SIZE];
	CLIENT_STATE   m_state;
	std::mutex     m_state_lock;
	std::mutex	   m_socket_lock;
	CPlayer        m_player;
};
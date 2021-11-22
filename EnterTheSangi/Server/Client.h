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
	~CClient();

	void SetID(const int client_id);
	void SetState(CLIENT_STATE state);
	void SetSocket(SOCKET socket);


	char GetID();
	CLIENT_STATE GetState();

	void StateLock();
	void StateUnlock();


private:
	SOCKET         m_socket;
	SOCKADDR_IN    m_addr;
	char           m_ID;
	CLIENT_STATE   m_state;
	std::mutex     m_state_lock;

	CPlayer        m_player;
};
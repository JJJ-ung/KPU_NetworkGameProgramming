#pragma once
#include <winsock2.h>
#include <mutex>
#include "Player.h"

//client state
enum STATE { ST_FREE, ST_ACCEPT, ST_INROBBY,ST_READY,ST_INGAME };

class Client
{
public:
	explicit Client();
	~Client();

	void SetID(const int client_id);

	char GetID();

	void StateLock();
	void StateUnlock();

private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	char m_ID;
	STATE m_state;
	std::mutex m_state_lock;

	Player m_player;
};
#pragma once
#include "Client.h"


CClient::CClient() {};

CClient::CClient(const CClient& client) {};

CClient::~CClient() {};

void CClient::SetID(const int client_id)
{
	m_ID = client_id;
};

void CClient::SetName(char* name)
{
	strcpy_s(m_name, name);
}

void CClient::SetState(CLIENT_STATE state)
{
	m_state = state;
}

void CClient::SetSocket(SOCKET socket)
{
	m_socket = socket;
};

char CClient::GetID()
{
	return m_ID;
};

CLIENT_STATE CClient::GetState()
{
	return m_state;
};

SOCKET CClient::GetSocket()
{
	return m_socket;
};

char* CClient::GetBuf()
{
	return m_buff;
}

CPlayer& CClient::GetPlayer()
{
	return m_player;
}

char* CClient::GetName()
{
	return m_name;
}

void CClient::StateLock()
{
	m_state_lock.lock();
}

void CClient::StateUnlock()
{
	m_state_lock.unlock();
}

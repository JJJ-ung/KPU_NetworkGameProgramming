#pragma once
#include "Client.h"


CClient::CClient()
{
	m_state = ST_FREE;
};

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

void  CClient::SocketLock()
{
	m_socket_lock.lock();
}

void  CClient::SocketUnlock()
{
	m_socket_lock.unlock();
}

void CClient::Disconnect()
{
	memset(m_buff, NULL, BUF_SIZE);
	memset(m_name, NULL, MAX_NAME_SIZE);
	StateLock();
	m_state = ST_FREE;
	StateUnlock();
	closesocket(m_socket);
};
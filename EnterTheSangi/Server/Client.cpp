#pragma once
#include "Client.h"


CClient::CClient() {};

CClient::~CClient() {};

void CClient::SetID(const int client_id)
{
	m_ID = client_id;
};
char CClient::GetID()
{
	return m_ID;
};

void CClient::Set_state(CLIENT_STATE state)
{
	m_state = state;
}

void CClient::StateLock()
{
	m_state_lock.lock();
};

void CClient::StateUnlock()
{
	m_state_lock.unlock();
};

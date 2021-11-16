#include "Client.h"


explicit Client::Client() {};

Client::~Client() {};

void Client::SetID(const int client_id)
{
	m_ID = client_id;
};

char Client::GetID()
{
	return m_ID;
};


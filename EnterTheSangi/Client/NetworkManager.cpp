#include "NetworkManager.h"

NetworkMGR::NetworkMGR()
{

}

NetworkMGR::~NetworkMGR()
{
	closesocket(m_socket);
}

void NetworkMGR::Activate()
{

}
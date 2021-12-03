#pragma once
#include "MainServer.h"
#include "Protocol.h"

int main()
{
	CMainServer* main_server=new CMainServer;

	main_server->Init(SERVER_PORT);
	main_server->Activate();

}
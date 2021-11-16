#include "MainServer.h"
#include "Protocol.h"

int main()
{
	MainServer* main_server=new MainServer;

	main_server->Init(SERVER_PORT);
	main_server->Activate();

}
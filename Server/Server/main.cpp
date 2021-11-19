#include <iostream>
#include <thread>
#include "ClientModel.h"
#include "ServerTCP.h"



int main()
{
	ServerTCP newServer;
	newServer.Start();
}
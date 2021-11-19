#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ServerTCP.h"

#include <iostream>
#include <thread>
#include <future>

#include "ClientModel.h"

void ServerTCP::WriteErrorAndExitProg(std::string errorName)
{
	std::cout << errorName << std::endl;
	exit(-1);
}


void ServerTCP::GetMsg(SOCKET client ,std::string& dst)
{
	char buf[100];
	recv(client, buf, 100, NULL);
	dst = buf;
}

bool ServerTCP::SendMsg(SOCKET client, const std::string& str)
{
	int size = str.length();
	if (send(client, str.c_str(), str.length() + 1, NULL) == SOCKET_ERROR) {
		return false;
	};
	return true;
}

void ServerTCP::ProcessingClient(SOCKET connection)
{
	
	std::string msg;
	GetMsg(connection, msg);
	auto tmp = ClientModel::GetIterations(msg);
	if (tmp.second == Corrected)
	{
		SendMsg(connection, tmp.first);
	}
	else
	{
		SendMsg(connection, "Error");		
	}
}

void ServerTCP::AddClients()
{
	int sizeSockAddrIn = sizeof(sockAdrIn);
	while (true)
	{		
		SOCKET newConnection = accept(serverSocket, (SOCKADDR*)&sockAdrIn, &sizeSockAddrIn);
		if (newConnection == 0)
		{
			std::cout << "Error\n";
		}
		else
		{
			pollMutex.lock();
			pollfd newClinet;
			newClinet.fd = newConnection;
			newClinet.events = 0;
			newClinet.revents = 0;			
			clients.push(newClinet);		
			pollMutex.unlock();
		}	
	}
}

ServerTCP::ServerTCP()
{
}

void ServerTCP::Start(WORD version, USHORT port, const std::string& inetAddr)
{
	if (WSAStartup(version,&wsaData) == SOCKET_ERROR)
	{
		WriteErrorAndExitProg(STARTUP_ERROR_MSG);
	}

	sockAdrIn.sin_family = AF_INET;
	sockAdrIn.sin_port = htons(port);
	sockAdrIn.sin_addr.S_un.S_addr = inet_addr( inetAddr == INET_ADDR? INET_ADDR : inetAddr.c_str());
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == SOCKET_ERROR)
	{
		WriteErrorAndExitProg(SOCKET_ERROR_MSG);
	}

	if (bind(serverSocket, (SOCKADDR*)&sockAdrIn, sizeof(sockAdrIn)) == SOCKET_ERROR)
	{
		WriteErrorAndExitProg(BIND_ERROR_MSG);
	}

	listen(serverSocket, SOMAXCONN);

	std::cout << "server started" << std::endl;

	auto as = std::async(std::launch::async,&ServerTCP::AddClients,this);

	PollsStart();
}

int ServerTCP::GetCountClientsInOnePoll()
{	
	return clients.size() > MAX_CLIENTS_IN_POLL ? MAX_CLIENTS_IN_POLL : clients.size();
}

void ServerTCP::PollsInit()
{
	for (int i = 0; i < MAX_POLLS; i++)
	{
		threadPolls.push_back(std::thread(&ServerTCP::ProcessingPoll,this,i));		
	}
}

void ServerTCP::PollsStart()
{
	PollsInit();
	for (auto& i : threadPolls)
		i.detach();	
}

std::pair<std::shared_ptr<pollfd[]>,int> ServerTCP::TakeClientsInPoll()
{
	pollMutex.lock();
	
	int countClientsInOnePoll = GetCountClientsInOnePoll();	
	std::shared_ptr<pollfd[]> clientsInPoll(new pollfd[countClientsInOnePoll]);
	if (countClientsInOnePoll)
	{
		
		for (int i = 0; i < countClientsInOnePoll; i++)
		{
			pollfd client = clients.front();
			client.events = POLLRDNORM;
			clientsInPoll[i] = client;
			clients.pop();
		}
	}	
	pollMutex.unlock();
	return std::make_pair(clientsInPoll, countClientsInOnePoll);}

void ServerTCP::PushClients(std::shared_ptr<pollfd[]> pollClients,int size)
{
	pollMutex.lock();
	for (int i = 0; i < size; i++)
	{
		if (pollClients[i].revents == POLLERR || pollClients[i].revents == POLLHUP || pollClients[i].revents == POLLNVAL)
		{
			closesocket(pollClients[i].fd);
		}
		else
		{
			clients.push(pollClients[i]);
		}
	}
	pollMutex.unlock();
}

void ServerTCP::ProcessingPoll(int pollNumber)
{
	while (true)
	{		
		std::pair<std::shared_ptr<pollfd[]>, int> pollInfo = TakeClientsInPoll();
		int counts = WSAPoll(pollInfo.first.get(), pollInfo.second, 1000);
		if (counts != 0 && counts != -1)
		{
			for (int i = 0; i < pollInfo.second; i++)
			{
				if (pollInfo.first[i].revents == POLLRDNORM)
				{
					ProcessingClient(pollInfo.first[i].fd);
				}
			}				
		}
		PushClients(pollInfo.first, pollInfo.second);		
	}		
}

ServerTCP::~ServerTCP()
{
	WSACleanup();
	closesocket(serverSocket);
}

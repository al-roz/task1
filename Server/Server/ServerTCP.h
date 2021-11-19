#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>

#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <future>
#include <queue>


#define STARTUP_ERROR_MSG		"Startup error"
#define SOCKET_ERROR_MSG		"Socket error"
#define BIND_ERROR_MSG			"Bind error"
#define INET_ADDR				"127.0.0.1"
#define POLL_ERROR_MSG			"Poll error"

#define MAX_POLLS				5
#define MAX_CLIENTS_IN_POLL		10



class ServerTCP
{
private:
	SOCKET serverSocket;
	WSADATA wsaData;
	SOCKADDR_IN sockAdrIn;

	std::mutex pollMutex;
	int numberUnprocessedClients;
	//std::vector<std::future<void>> threadPolls;
	std::vector<std::thread> threadPolls;
	std::vector<HANDLE> liveThreads;

	pollfd clientsPoll[MAX_CLIENTS_IN_POLL];
	
	std::queue<pollfd> clients;

	void GetMsg(SOCKET client, std::string& dst);
	bool SendMsg(SOCKET client, const std::string& str);
	void ProcessingClient(SOCKET connection);


	void WriteErrorAndExitProg(std::string errorName);
	void AddClients();


	std::pair<std::shared_ptr<pollfd[]>, int> TakeClientsInPoll();

	void PushClients(std::shared_ptr<pollfd[]> clients, int size);

	int GetCountClientsInOnePoll();
	void PollsInit();
	void PollsStart();
	void ProcessingPoll(int pollNumber);


public:
	ServerTCP();
	void Start(WORD version = MAKEWORD(2, 2), USHORT port = 1111, const std::string& inetAddr = INET_ADDR);

	~ServerTCP();


};


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> 
#include <cstring> 
#include <string>
#include <iostream>

#define STARTUP_ERROR_MSG		"Startup error"
#define SOCKET_ERROR_MSG		"Socket error"
#define CONNECT_ERROR_MSG		"Connect Error"
#define GET_MASSEG_ERROR_MSG	"MSG error"
#define SEND_MASSEG_ERROR_MSG	"Unable to send"
#define INET_ADDR				"127.0.0.1"

void GetMsgRequest(SOCKET sock, char* str, int len)
{

	if (recv(sock, str, len, 0) == SOCKET_ERROR)
	{
		std::cout << GET_MASSEG_ERROR_MSG;
		exit(SOCKET_ERROR);
	}
}

void SendMsg(SOCKET sock, std::string respons)
{
	if (send(sock, respons.c_str(), respons.size() + 1, 0) == SOCKET_ERROR)
	{
		std::cout << SEND_MASSEG_ERROR_MSG;
		exit(SOCKET_ERROR);
	}
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << STARTUP_ERROR_MSG;
		WSACleanup();
		return SOCKET_ERROR;
	}
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(1111);
	serverInfo.sin_addr.S_un.S_addr = inet_addr(INET_ADDR);

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == SOCKET_ERROR)
	{
		std::cout << SOCKET_ERROR_MSG;
		WSACleanup();
		return SOCKET_ERROR;
	}


	if (connect(clientSocket, (SOCKADDR*)&serverInfo, sizeof(serverInfo)) == SOCKET_ERROR)
	{
		std::cout << CONNECT_ERROR_MSG;
		WSACleanup();
		return SOCKET_ERROR;
	}

	std::cout << "Connected\n";

	while (true)
	{
		std::string request = "";
		
		std::getline(std::cin, request);
		SendMsg(clientSocket, request);

		char response[20] = "";
		GetMsgRequest(clientSocket, response, 20);
		std::cout << response << std::endl;
	}	
}
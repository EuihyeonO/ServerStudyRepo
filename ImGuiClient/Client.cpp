#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <thread>
#include <iostream>

#include "Client.h"
#include "Resource.h"
#include "DataManager.h"

void Client::ConnectToServer()
{
	WSADATA Wsa;

	int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);
	if (WsaStartResult != 0)
	{
		std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
	}

	SOCKADDR_IN Addr = { 0, };
	Addr.sin_addr.s_addr = inet_addr(Client::GetInstance()->GetServerIP_Cstr());
	Addr.sin_port = PORT;
	Addr.sin_family = AF_INET;

	while (connect(ServerSocket, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr)));
	std::thread(&DataManager::RecvData, DataManager::GetInstance(), std::ref(ServerSocket)).detach();

	send(ServerSocket, Name.c_str(), (int)Name.size(), 0);
}

void Client::End()
{
	shutdown(ServerSocket, SD_BOTH);
	closesocket(ServerSocket);

	WSACleanUp();
}

void Client::WSACleanUp()
{
	WSADATA Wsa;

	int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);
	if (WsaStartResult != 0)
	{
		std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
	}
}

void Client::SendMassage(const std::string& _Chat)
{
	send(ServerSocket, _Chat.c_str(), (int)_Chat.size() + 1, 0);
}

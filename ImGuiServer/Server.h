#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <vector>
#include <set>
#include <string_view>

#include "Define.h"

struct Client
{
public:
	SOCKET ClientSock = { 0, };
	SOCKADDR_IN Addr = { 0, };

	int ClientSize = sizeof(Addr);
	int ID = -1;

	Client() {}
};

class Server
{
public:
	static Server* GetInstance()
	{
		static Server Instance;
		return &Instance;
	}

	int Start();
	void End();

	void RecvChat(SOCKET _Socket, int Num);
	void AcceptClient(SOCKET& _Socket);
	void SendMsgToAllCLient(std::string_view _Msg, const std::set<int>& _IgnoreIndex);

private:
	Server(){}
	~Server(){}

private:
	SOCKET ListenSocket;

	std::vector<std::pair<Client, std::string>> Clients;
	std::set<int> DeathID;
};
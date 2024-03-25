#pragma once
#pragma comment(lib, "ws2_32.lib")

//Windows.h 에는 WinSock.h(WinSock2.h의 구버전)이 포함되어 있어 중복 정의가 발생. 
//구버전을 무시하겠다는 의미.
#define _WINSOCKAPI_

//이렇게 그냥 WinSock2.h 를 먼저 include 해도 됨. (WinSock2.h 내부에는 WinSock.h의 포함을 막는 코드가 있다고 함) 
#include <WinSock2.h>
#include <windows.h>

#include <iostream>
#include <thread>
#include <vector>
#include <string>


class Client
{
public:
	SOCKET ClientSock;
	SOCKADDR_IN Addr = { 0, };
	int ClientSize = sizeof(Addr);
	int Number = -1;

	INT32 bIsDeath : 1;

	Client()
	{

	}
};

std::vector<std::pair<Client, std::string>> Clients;
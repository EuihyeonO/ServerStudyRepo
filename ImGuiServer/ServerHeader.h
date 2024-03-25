#pragma once
#pragma comment(lib, "ws2_32.lib")

//Windows.h ���� WinSock.h(WinSock2.h�� ������)�� ���ԵǾ� �־� �ߺ� ���ǰ� �߻�. 
//�������� �����ϰڴٴ� �ǹ�.
#define _WINSOCKAPI_

//�̷��� �׳� WinSock2.h �� ���� include �ص� ��. (WinSock2.h ���ο��� WinSock.h�� ������ ���� �ڵ尡 �ִٰ� ��) 
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
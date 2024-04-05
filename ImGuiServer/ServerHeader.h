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
	SOCKET ClientSock = { 0, };
	SOCKADDR_IN Addr = { 0, };

	int ClientSize = sizeof(Addr);
	int Number = -1;

	bool bIsDeath = false;

	Client(){}
};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

void PrintLog();

int WindowInit(HINSTANCE& _hInstance);
int ServerInit();

void RecvData(SOCKET _Socket, int Num);
void AcceptClient(SOCKET& _Socket);

void AddRecvChat(const std::string& _Chat);
void EraseRecvChat(const std::vector<std::string>::iterator& _ChatIter);

std::vector<std::pair<Client, std::string>> Clients;
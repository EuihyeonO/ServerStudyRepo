#pragma once
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define PACKET_SIZE 1024
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//�̷��� �׳� WinSock2.h �� ���� include �ص� ��. (WinSock2.h ���ο��� WinSock.h�� ������ ���� �ڵ尡 �ִٰ� ��) 
#include <WinSock2.h>
#include <windows.h>

#include <iostream>
#include <thread>
#include <string>
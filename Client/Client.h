#pragma once
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define PACKET_SIZE 1024
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//이렇게 그냥 WinSock2.h 를 먼저 include 해도 됨. (WinSock2.h 내부에는 WinSock.h의 포함을 막는 코드가 있다고 함) 
#include <WinSock2.h>
#include <windows.h>

#include <iostream>
#include <thread>
#include <string>
#pragma once

#include <WinSock2.h>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d11.lib")

int WindowInit(HINSTANCE& _hInstance);
void RecvData(SOCKET& _Socket);
int WSAInit();
void ConnectToServer();

void AddChat(const std::string& _Chat);
void EraseChat(const std::vector<std::string>::iterator& _ChatIter);
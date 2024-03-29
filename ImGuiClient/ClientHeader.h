#pragma once

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d11.lib")

int WindowInit(HINSTANCE& _hInstance);
void RecvData(SOCKET& _Socket);
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




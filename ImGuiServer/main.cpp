// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.



#include "framework.h"
#include "resource.h"

#include "ServerHeader.h"

#include <tchar.h>

#include <filesystem>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <thread>
#include <mutex>

#pragma comment(lib, "d3d11.lib")

#include "DirectXManager.h"
#include "GUIManager.h"
#include "Server.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DirectXManager::GetInstance()->Start(hInstance);
    GUIManager::GetInstance()->Start();

    if (Server::GetInstance()->Start() == 1)
    {
        return 1;
    }

    // Main loop
    GUIManager::GetInstance()->Loop();


    GUIManager::GetInstance()->End();
    DirectXManager::GetInstance()->End();
    Server::GetInstance()->End();
    return 0;
}



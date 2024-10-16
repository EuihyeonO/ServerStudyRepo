// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "resource.h"

#include "Client.h"
#include "GUIManager.h"
#include "DirectXManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DirectXManager::GetInstance()->Start(hInstance);
    GUIManager::GetInstance()->Start();

    //Loop
    {
        GUIManager::GetInstance()->Loop();
    }

    GUIManager::GetInstance()->End();
    DirectXManager::GetInstance()->End();
    Client::GetInstance()->End();

    return 0;
}





// WindowsProject1.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include "framework.h"
#include "resource.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ServerHeader.h"

#include <d3d11.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

#include <WinSock2.h>

#pragma comment(lib, "d3d11.lib")

#define MAX_LOADSTRING 100
#define PACKET_SIZE 1024
#define PORT 8080

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// 전역 변수:
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];            

WNDCLASSEXW wc;
HWND hwnd;
ImVec4 clear_color;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

SOCKET Server;
std::vector<std::string> RecvChats;
std::mutex ChatsMutex;

void AddRecvChat(const std::string& _Chat)
{
    std::lock_guard<std::mutex> lock(ChatsMutex);
    RecvChats.push_back(_Chat);
}

void EraseRecvChat(const std::vector<std::string>::iterator& _ChatIter)
{
    std::lock_guard<std::mutex> lock(ChatsMutex);
    RecvChats.erase(_ChatIter);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (WindowInit(hInstance) == 1)
    {
        return 1;
    }

    if (ServerInit() == 1)
    {
        return 1;
    }

    char Name[PACKET_SIZE] = { 0, };
    char Message[PACKET_SIZE] = { 0, };

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

        PrintLog();

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

int WindowInit(HINSTANCE& _hInstance)
{
    // 전역 문자열을 초기화합니다.
    LoadStringW(_hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(_hInstance, IDC_IMGUISERVER, szWindowClass, MAX_LOADSTRING);

    //IMGUI
    wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);

    hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 300, 400, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    return 0;
}

int ServerInit()
{
    //서버
    WSADATA Wsa;

    //윈속 라이브러리 초기화, 데이터를 두 번째 인자에 대입.
    int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);

    if (WsaStartResult != 0)
    {
        std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
        return 1; // 프로그램 종료 또는 오류 처리
    }

    Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN Addr = { 0, };

    Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Addr.sin_port = PORT;
    Addr.sin_family = AF_INET;

    int BindResult = bind(Server, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr));

    if (BindResult != 0)
    {
        std::cerr << "bind failed with error code: " << BindResult << std::endl;
        return 1; // 프로그램 종료 또는 오류 처리
    }

    int ListenResult = listen(Server, SOMAXCONN);

    if (ListenResult != 0) {
        std::cerr << "listen failed with error code: " << ListenResult << std::endl;
        return 1; // 프로그램 종료 또는 오류 처리
    }

    std::thread(Accept, std::ref(Server)).detach();

    return 0;
}

void Accept(SOCKET& _Socket)
{
    int Cnt = 0;

    while (true)
    {
        Clients.push_back(std::pair<Client, std::string>{Client(), ""});
        Clients[Cnt].first.ClientSock = accept(_Socket, reinterpret_cast<SOCKADDR*>(&Clients[Cnt].first.Addr), &Clients[Cnt].first.ClientSize);
        Clients[Cnt].first.Number = Cnt;
        Clients[Cnt].first.bIsDeath = false;

        std::thread(RecvData, Clients[Cnt].first.ClientSock, Cnt).detach();

        Cnt++;
    }
}

void RecvData(SOCKET _Socket, int Num)
{
    char Buffer[PACKET_SIZE] = { 0, };
    recv(_Socket, Buffer, sizeof(Buffer), 0);
    Clients[Num].second = Buffer;

    AddRecvChat(Clients[Num].second + " join.");
    
    for (int i = 0; i < Clients.size(); i++)
    {
        if (i == Num || Clients[i].first.bIsDeath == true)
        {
            continue;
        }

        std::string SendMsg = Clients[Num].second;
        SendMsg += " join.";

        send(Clients[i].first.ClientSock, SendMsg.c_str(), sizeof(Buffer), 0);
    }

    while (true)
    {
        ZeroMemory(Buffer, sizeof(Buffer));
        int RecvReturn = recv(_Socket, Buffer, sizeof(Buffer), 0);

        if (RecvReturn == -1)
        {
            Clients[Num].first.bIsDeath = true;
            AddRecvChat(Clients[Num].second + " leave. \n");

            for (int i = 0; i < Clients.size(); i++)
            {
                if (i == Num || Clients[i].first.bIsDeath == true)
                {
                    continue;
                }

                std::string SendMsg = Clients[Num].second;
                SendMsg += " Leave.";

                send(Clients[i].first.ClientSock, SendMsg.c_str(), sizeof(Buffer), 0);
            }

            break;
        }

        if (Buffer[0] != 0)
        {
            AddRecvChat(Clients[Num].second + " : " + Buffer + "\n");

            for (int i = 0; i < Clients.size(); i++)
            {
                if (i == Num || Clients[i].first.bIsDeath == true)
                {
                    continue;
                }

                std::string SendMsg = Clients[Num].second;
                SendMsg += " : ";
                SendMsg += Buffer;

                send(Clients[i].first.ClientSock, SendMsg.c_str(), sizeof(Buffer), 0);
            }
        }
    }
}

void PrintLog()
{
    ImGui::Begin("Server");

    char A[PACKET_SIZE] = { 0, };

    ImGui::Text("Log");

    for (int i = 0; i < RecvChats.size(); i++)
    {
        ImGui::Text(RecvChats[i].c_str());
    }

    if (RecvChats.size() > 20)
    {
        EraseRecvChat(RecvChats.begin());
    }

    ImGui::End();
}
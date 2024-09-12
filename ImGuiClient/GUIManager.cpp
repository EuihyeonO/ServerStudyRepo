#include <filesystem>

#include "GUIManager.h"
#include "Client.h"
#include "DirectXManager.h"
#include "DataManager.h"

ImVec4 clear_color;

// Forward declare message handler from imgui_impl_win32.cpp

void GUIManager::Start()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    std::filesystem::path MyPath = std::filesystem::current_path();
    MyPath += "\\malgun.ttf";

    io.Fonts->AddFontFromFileTTF(MyPath.string().c_str(), 17.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(DirectXManager::GetInstance()->GethWnd());
    ImGui_ImplDX11_Init(DirectXManager::GetInstance()->GetDevice(), DirectXManager::GetInstance()->GetDeviceContext());

    bool show_demo_window = true;
    bool show_another_window = false;
    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void GUIManager::Loop()
{
    Client* ClientInst = Client::GetInstance();

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
        DirectXManager::GetInstance()->ResizeRenderTarget();

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

        ImGui::Begin("Client");

        if (ClientInst->GetName() == "")
        {
            ImGui::Text("Please Input Name");

            char Name[PACKET_SIZE] = { 0, };

            if (ImGui::InputText(" ", Name, IM_ARRAYSIZE(Name), ImGuiInputTextFlags_EnterReturnsTrue) == true)
            {
                ClientInst->SetName(Name);
            }
        }
        else if (ClientInst->GetIP() == "")
        {
            ImGui::Text("Please Input Server IP");

            char IP[PACKET_SIZE] = { 0, };

            if (ImGui::InputText(" ", IP, IM_ARRAYSIZE(IP), ImGuiInputTextFlags_EnterReturnsTrue) == true)
            {
                ClientInst->SetServerIP(IP);
                ClientInst->ConnectToServer();
            }
        }
        else
        {
            char ChatText[1024] = { 0, };

            ImGui::Text("Chat Start");
            std::string NameText = "User Name : ";
            NameText += Client::GetInstance()->GetName();

            ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, NameText.c_str());

            if (ImGui::InputText(" ", ChatText, IM_ARRAYSIZE(ChatText), ImGuiInputTextFlags_EnterReturnsTrue) == true)
            {
                DataManager::GetInstance()->AddChat(ChatText, true);
            }

            const std::vector<std::string>& Chats = DataManager::GetInstance()->GetChats();

            for (int i = 0; i < Chats.size(); i++)
            {
                ImGui::Text(Chats[i].c_str());
            }
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        DirectXManager::GetInstance()->GetDeviceContext()->OMSetRenderTargets(1, DirectXManager::GetInstance()->GetRTVDoublePtr(), nullptr);
        DirectXManager::GetInstance()->GetDeviceContext()->ClearRenderTargetView(DirectXManager::GetInstance()->GetRTV(), clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        DirectXManager::GetInstance()->GetSwapChain()->Present(1, 0); // Present with vsync
    }
}

void GUIManager::End()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

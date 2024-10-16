#include "Server.h"
#include "DataManager.h"

#include <iostream>
#include <thread>

int Server::Start()
{
    //¼­¹ö
    WSADATA Wsa;

    int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);

    if (WsaStartResult != 0)
    {
        std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
        return 1;
    }

    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN Addr = { 0, };

    Addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Addr.sin_port = PORT;
    Addr.sin_family = AF_INET;

    int BindResult = bind(ListenSocket, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr));

    if (BindResult != 0)
    {
        std::cerr << "Bind failed. error code : " << BindResult << std::endl;
        return 1;
    }

    int ListenResult = listen(ListenSocket, SOMAXCONN);

    if (ListenResult != 0)
    {
        std::cerr << "Listen failed. Error code : " << ListenResult << std::endl;
        return 1;
    }

    std::thread(&Server::AcceptClient, Server::GetInstance(), std::ref(ListenSocket)).detach();

    return 0;
}

void Server::AcceptClient(SOCKET& _Socket)
{
    while (true)
    {
        int ID = 0;

        if (DeathID.size() > 0)
        {
            ID = *(DeathID.begin());
            DeathID.erase(DeathID.begin());
        }
        else
        {
            ID = (int)Clients.size();
            Clients.push_back(std::pair<Client, std::string>{Client(), ""});
        }

        Clients[ID].first.ID = ID;
        Clients[ID].first.ClientSock = accept(_Socket, reinterpret_cast<SOCKADDR*>(&Clients[ID].first.Addr), &Clients[ID].first.ClientSize);

        std::thread(&Server::RecvChat, Server::GetInstance(), Clients[ID].first.ClientSock, ID).detach();
    }
}

void Server::RecvChat(SOCKET _Socket, int Num)
{
    char Buffer[PACKET_SIZE] = { 0, };
    recv(_Socket, Buffer, sizeof(Buffer), 0);
    Clients[Num].second = Buffer;

    DataManager::GetInstance()->AddRecvChat(Clients[Num].second + " Join.");

    std::string JoinMsg = Clients[Num].second;
    JoinMsg += " Join.";

    SendMsgToAllCLient(JoinMsg, { Num });

    while (true)
    {
        ZeroMemory(Buffer, sizeof(Buffer));

        int RecvReturn = recv(_Socket, Buffer, sizeof(Buffer), 0);

        if (RecvReturn == 0)
        {
            DeathID.insert(Num);
            DataManager::GetInstance()->AddRecvChat(Clients[Num].second + " leave. \n");

            std::string LeaveMsg = Clients[Num].second;
            LeaveMsg += " Leave.";

            SendMsgToAllCLient(LeaveMsg, {Num});

            shutdown(Clients[Num].first.ClientSock, SD_BOTH);
            closesocket(Clients[Num].first.ClientSock);

            break;
        }

        DataManager::GetInstance()->AddRecvChat(Clients[Num].second + " : " + Buffer + "\n");

        std::string SendMsg = Clients[Num].second;
        SendMsg += " : ";
        SendMsg += Buffer;

        SendMsgToAllCLient(SendMsg, { Num });
    }
}

void Server::SendMsgToAllCLient(std::string_view _Msg, const std::set<int>& _IgnoreIndex)
{
    for (int i = 0; i < Clients.size(); i++)
    {
        if (_IgnoreIndex.size() != 0 && _IgnoreIndex.find(i) != _IgnoreIndex.end())
        {
            continue;
        }

        send(Clients[i].first.ClientSock, _Msg.data(), sizeof(char) * PACKET_SIZE, 0);
    }
}

void Server::End()
{
    shutdown(ListenSocket, SD_BOTH);
    closesocket(ListenSocket);

    WSACleanup();
}
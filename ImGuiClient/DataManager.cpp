#include "DataManager.h"
#include "Client.h"

void DataManager::AddChat(const std::string& _Chat, bool _isSend)
{
    std::lock_guard<std::mutex> lock(ChatsMutex);

    std::string ChatStr = "";

    if (_isSend == true)
    {
        Client::GetInstance()->SendMassage(_Chat);

        ChatStr = Client::GetInstance()->GetName();
        ChatStr += " : ";
    }

    ChatStr += _Chat;

    Chats.push_back(ChatStr);
    EraseOldestChat();
}

void DataManager::EraseOldestChat()
{
    if (Chats.size() > 20)
    {
        std::lock_guard<std::mutex> lock(ChatsMutex);
        Chats.erase(Chats.begin());
    }
}

void DataManager::RecvData(SOCKET& _Socket)
{
    char Buffer[PACKET_SIZE];

    while (true)
    {
        ZeroMemory(Buffer, sizeof(Buffer));
        int RecvReturn = recv(_Socket, Buffer, sizeof(Buffer), 0);

        if (Buffer[0] != 0)
        {
            AddChat(Buffer, false);
        }
    }
}
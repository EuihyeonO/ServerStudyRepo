#include "DataManager.h"
#include "Client.h"

void DataManager::AddChat(const std::string& _Chat, bool _isSend)
{
    std::string ChatStr = "";

    if (_isSend == true)
    {
        Client::GetInstance()->SendMassage(_Chat);

        ChatStr = Client::GetInstance()->GetName();
        ChatStr += " : ";
    }

    ChatStr += _Chat;

    ChatsMutex.lock();
    Chats.push_back(ChatStr);
    ChatsMutex.unlock();

    EraseOldestChat();
}

void DataManager::EraseOldestChat()
{
    ChatsMutex.lock();

    if (Chats.size() > 20)
    {
        Chats.erase(Chats.begin());
    }

    ChatsMutex.unlock();
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
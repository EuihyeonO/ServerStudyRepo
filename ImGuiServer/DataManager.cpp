#include "DataManager.h"

void DataManager::AddRecvChat(const std::string& _Chat)
{
    ChatsMutex.lock();
    RecvChats.push_back(_Chat);
    ChatsMutex.unlock();

    EraseTopRecvChat();
}

void DataManager::EraseTopRecvChat()
{
    ChatsMutex.lock();

    if (RecvChats.size() > 20)
    {
        RecvChats.erase(RecvChats.begin());
    }

    ChatsMutex.unlock();
}
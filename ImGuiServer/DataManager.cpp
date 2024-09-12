#include "DataManager.h"

void DataManager::AddRecvChat(const std::string& _Chat)
{
    std::lock_guard<std::mutex> lock(ChatsMutex);
    RecvChats.push_back(_Chat);

    EraseTopRecvChat();
}

void DataManager::EraseTopRecvChat()
{
    if (RecvChats.size() > 20)
    {
        std::lock_guard<std::mutex> lock(ChatsMutex);
        RecvChats.erase(RecvChats.begin());
    }
}
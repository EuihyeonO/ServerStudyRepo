#pragma once
#include <mutex>
#include <vector>

#include "ClientHeader.h"
#include "Define.h"

class DataManager
{
public:
	static DataManager* GetInstance()
	{
		static DataManager Instance;
		return &Instance;
	}

	const std::vector<std::string>& GetChats()
	{
		return Chats;
	}

	void AddChat(const std::string& _Chat, bool _isSend);
	void EraseOldestChat();
	void RecvData(SOCKET& _Socket);

private:
	DataManager(){}
	~DataManager(){}

	std::vector<std::string> Chats;
	std::mutex ChatsMutex;
};


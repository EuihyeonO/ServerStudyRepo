#pragma once
#include <vector>
#include <string>
#include <mutex>

class DataManager
{
public:
	static DataManager* GetInstance()
	{
		static DataManager Instance;
		return &Instance;
	}

	const std::vector<std::string>& GetRecvChats()
	{
		return RecvChats;
	}

	void AddRecvChat(const std::string& _Chat);
	void EraseTopRecvChat();

private:
	DataManager(){}
	~DataManager(){}

private:
	std::vector<std::string> RecvChats;
	std::mutex ChatsMutex;
};


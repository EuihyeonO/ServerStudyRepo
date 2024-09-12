#pragma once

#include "Define.h"
#include "ClientHeader.h"

class Client
{
public:
	static Client* GetInstance()
	{
		static Client Instance;
		return &Instance;
	}

	void SetName(const char* _Name)
	{
		Name = _Name;
	}

	void SetServerIP(const char* _IP)
	{
		ServerIP = _IP;
	}

	const std::string& GetName()
	{
		return Name;
	}

	const char* GetName_Cstr()
	{
		return Name.c_str();
	}

	const std::string& GetIP()
	{
		return ServerIP;
	}

	const char* GetServerIP_Cstr()
	{
		return ServerIP.c_str();
	}

	void ConnectToServer();
	void End();

	void WSACleanUp();

	void SendMassage(const std::string& _Chat);
private:
	Client() 
	{
		ConnectedSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	~Client() {}

private:
	SOCKET ConnectedSocket;

	std::string Name = "";
	std::string ServerIP = "";
};
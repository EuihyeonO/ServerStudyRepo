#include "Server.h"

void RecvData(SOCKET _Socket, int Num)
{
	char Buffer[PACKET_SIZE] = { 0, };
	recv(_Socket, Buffer, sizeof(Buffer), 0);
	Clients[Num].second = Buffer;

	while (true)
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		recv(_Socket, Buffer, sizeof(Buffer), 0);
		std::cout << Clients[Num].second << " : " << Buffer << std::endl;
	}
}

void Accept(SOCKET & _Socket)
{
	int Cnt = 0;

	while (true)
	{
		Clients.push_back(std::pair<Client, std::string>{Client(), ""});
		Clients[Cnt].first.ClientSock = accept(_Socket, reinterpret_cast<SOCKADDR*>(&Clients[Cnt].first.Addr), &Clients[Cnt].first.ClientSize);
		Clients[Cnt].first.Number = Cnt;

	std::thread(RecvData, Clients[Cnt].first.ClientSock, Cnt).detach();
		Cnt++;
	}
}

int main()
{
	WSADATA Wsa;

	//���� ���̺귯�� �ʱ�ȭ, �����͸� �� ��° ���ڿ� ����.
	int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);

	if (WsaStartResult != 0) {
		std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
		return 1; // ���α׷� ���� �Ǵ� ���� ó��
	}

	/*
	ù ��° ���ڴ� �ּ��йи��� Ÿ��.
	AF_INET�� IPv4�̴�.

	�� ���� ���ڴ� ������ ����.
	SOCK_STREAM�� TCP����� ����
	SOCK_DGRAM�� UDP����� ����
	���..
	
	�� ��° ���ڴ� ������ �������� �ɼ�.
	IPPROTO_TCP�� TCP ���������� ����ϰڴٴ� ��.

	��ȯ�� ������ ���̵�
	*/

	SOCKET Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//���� �ּ�, ��Ʈ ���� ��� ����ü.
	//SOCKADDR �� ������, IP�� ��Ʈ�� �и��Ǿ����� �ʾ� �а� ���� �����ϴٰ� ��.
	SOCKADDR_IN Addr = { 0, };
	
	//�ּ� ����. INADDR_ANY�� ��밡���� ��ī���� IP�ּҸ� �ڵ����� �����ϴ� ��ũ��.
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Addr.sin_port = PORT;
	Addr.sin_family = AF_INET;

	//���ϰ� �ּ������� ���´�.
	int BindResult = bind(Server, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr));

	if (BindResult != 0) {
		std::cerr << "bind failed with error code: " << BindResult << std::endl;
		return 1; // ���α׷� ���� �Ǵ� ���� ó��
	}

	//Ŭ���̾�Ʈ�� ��û ��⿭. Queue�� ���� ���������� ��û�� ������.
	//�� ��° ���ڴ� ��⿭�� ũ��. SOMACONN�� ��⿭�� �ִ�ũ��.
	int ListenResult = listen(Server, SOMAXCONN);

	if (ListenResult != 0) {
		std::cerr << "listen failed with error code: " << ListenResult << std::endl;
		return 1; // ���α׷� ���� �Ǵ� ���� ó��
	}

	std::thread(Accept, std::ref(Server)).detach();

	char Name[PACKET_SIZE] = { 0, };
	char Message[PACKET_SIZE] = { 0, };

	while (true)
	{
		ZeroMemory(Name, sizeof(Name));
		ZeroMemory(Message, sizeof(Message));

		std::cin >> Name >> Message;

		for (int i = 0; i < Clients.size(); i++)
		{
			if (!strcmp(Clients[i].second.c_str(), Name))
			{
				send(Clients[i].first.ClientSock, Message, sizeof(Message), 0);
			}
		}
	}

	return 0;
}*
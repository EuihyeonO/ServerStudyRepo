//#include "Server.h"
//
//void RecvData(SOCKET _Socket, int Num)
//{
//	char Buffer[PACKET_SIZE] = { 0, };
//	recv(_Socket, Buffer, sizeof(Buffer), 0);
//	Clients[Num].second = Buffer;
//
//	while (true)
//	{
//		ZeroMemory(Buffer, sizeof(Buffer));
//		int RecvReturn = recv(_Socket, Buffer, sizeof(Buffer), 0);
//
//		if (RecvReturn == -1)
//		{
//			Clients[Num].first.bIsDeath = true;
//			std::cout << Clients[Num].second << "���� �����̽��ϴ�." << std::endl;
//			break;
//		}
//
//		std::cout << Clients[Num].second << " : " << Buffer << std::endl;
//
//	}
//}
//
//void Accept(SOCKET& _Socket)
//{
//	int Cnt = 0;
//
//	while (true)
//	{
//		Clients.push_back(std::pair<Client, std::string>{Client(), ""});
//		Clients[Cnt].first.ClientSock = accept(_Socket, reinterpret_cast<SOCKADDR*>(&Clients[Cnt].first.Addr), &Clients[Cnt].first.ClientSize);
//		Clients[Cnt].first.Number = Cnt;
//		Clients[Cnt].first.bIsDeath = false;
//
//		std::thread(RecvData, Clients[Cnt].first.ClientSock, Cnt).detach();
//		Cnt++;
//	}
//}
//
//int main()
//{
//	
//
//	/*
//	ù ��° ���ڴ� �ּ��йи��� Ÿ��.
//	AF_INET�� IPv4�̴�.
//
//	�� ���� ���ڴ� ������ ����.
//	SOCK_STREAM�� TCP����� ����
//	SOCK_DGRAM�� UDP����� ����
//	���..
//
//	�� ��° ���ڴ� ������ �������� �ɼ�.
//	IPPROTO_TCP�� TCP ���������� ����ϰڴٴ� ��.
//
//	��ȯ�� ������ ���̵�
//	*/
//
//	
//
//	//���� �ּ�, ��Ʈ ���� ��� ����ü.
//	//SOCKADDR �� ������, IP�� ��Ʈ�� �и��Ǿ����� �ʾ� �а� ���� �����ϴٰ� ��.
//
//
//	//���ϰ� �ּ������� ���´�.
//	
//
//	//Ŭ���̾�Ʈ�� ��û ��⿭. Queue�� ���� ���������� ��û�� ������.
//	//�� ��° ���ڴ� ��⿭�� ũ��. SOMACONN�� ��⿭�� �ִ�ũ��.
//
//	//while (true)
//	//{
//	//	ZeroMemory(Name, sizeof(Name));
//	//	ZeroMemory(Message, sizeof(Message));
//	//
//	//	std::cin >> Name >> Message;
//	//
//	//	for (int i = 0; i < Clients.size(); i++)
//	//	{
//	//		if (!strcmp(Clients[i].second.c_str(), Name))
//	//		{
//	//			send(Clients[i].first.ClientSock, Message, sizeof(Message), 0);
//	//		}
//	//	}
//	//}
//
//	return 0;
//}
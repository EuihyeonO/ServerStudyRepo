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
//			std::cout << Clients[Num].second << "님이 떠나셨습니다." << std::endl;
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
//	첫 번째 인자는 주소패밀리의 타입.
//	AF_INET는 IPv4이다.
//
//	두 번쨰 인자는 소켓의 형식.
//	SOCK_STREAM은 TCP기반의 소켓
//	SOCK_DGRAM은 UDP기반의 소켓
//	등등..
//
//	세 번째 인자는 소켓의 프로토콜 옵션.
//	IPPROTO_TCP는 TCP 프로토콜을 사용하겠다는 것.
//
//	반환은 소켓의 아이디
//	*/
//
//	
//
//	//전송 주소, 포트 등을 담는 구조체.
//	//SOCKADDR 도 있으나, IP와 포트가 분리되어있지 않아 읽고 쓰기 불편하다고 함.
//
//
//	//소켓과 주소정보를 묶는다.
//	
//
//	//클라이언트의 요청 대기열. Queue와 같이 순차적으로 요청을 꺼낸다.
//	//두 번째 인자는 대기열의 크기. SOMACONN은 대기열의 최대크기.
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
#include "Client.h"

void RecvData(SOCKET& _Socket)
{
	char Buffer[PACKET_SIZE];
	while (true)
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		recv(_Socket, Buffer, sizeof(Buffer), 0);
		std::cout << "Server : " << Buffer << std::endl;
	}
}

int main()
{
	char IP[PACKET_SIZE] = {0, };
	char Name[PACKET_SIZE] = {0, };


	std::cout << "이름을 입력하시오 : " << std::endl;
	std::cin >> Name;

	std::cout << "아이피를 입력하시오 : " << std::endl;
	std::cin >> IP;

	WSADATA Wsa;

	//윈속 라이브러리 초기화, 데이터를 두 번째 인자에 대입.
	int WsaStartResult = WSAStartup(MAKEWORD(2, 2), &Wsa);

	if (WsaStartResult != 0) {
		std::cerr << "WSAStartup failed with error code: " << WsaStartResult << std::endl;
		return 1; // 프로그램 종료 또는 오류 처리
	}

	/*
	첫 번째 인자는 주소패밀리의 타입.
	AF_INET는 IPv4이다.

	두 번쨰 인자는 소켓의 형식.
	SOCK_STREAM은 TCP기반의 소켓
	SOCK_DGRAM은 UDP기반의 소켓
	등등..

	세 번째 인자는 소켓의 프로토콜 옵션.
	IPPROTO_TCP는 TCP 프로토콜을 사용하겠다는 것.

	반환은 소켓의 아이디
	*/

	SOCKET Server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//전송 주소, 포트 등을 담는 구조체.
	//SOCKADDR 도 있으나, IP와 포트가 분리되어있지 않아 읽고 쓰기 불편하다고 함.
	SOCKADDR_IN Addr = { 0, };

	//주소 저장. INADDR_ANY는 사용가능한 랜카드의 IP주소를 자동으로 저장하는 매크로.
	//클라이언트에선 inet_addr 이고, 서버에선 htonl이다.
	Addr.sin_addr.s_addr = inet_addr(IP);
	Addr.sin_port = PORT;
	Addr.sin_family = AF_INET;

	while (connect(Server, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr)));
	send(Server, Name, sizeof(Name), 0); //이름 보내기.

	std::thread(RecvData, std::ref(Server)).detach();

	while (true)
	{
		ZeroMemory(Name, sizeof(Name));
		std::cout << "메세지 : ";
		std::cin >> Name;

		send(Server, Name, sizeof(Name), 0);
	}

	return 0;
}
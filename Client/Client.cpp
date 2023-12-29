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


	std::cout << "�̸��� �Է��Ͻÿ� : " << std::endl;
	std::cin >> Name;

	std::cout << "�����Ǹ� �Է��Ͻÿ� : " << std::endl;
	std::cin >> IP;

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
	//Ŭ���̾�Ʈ���� inet_addr �̰�, �������� htonl�̴�.
	Addr.sin_addr.s_addr = inet_addr(IP);
	Addr.sin_port = PORT;
	Addr.sin_family = AF_INET;

	while (connect(Server, reinterpret_cast<SOCKADDR*>(&Addr), sizeof(Addr)));
	send(Server, Name, sizeof(Name), 0); //�̸� ������.

	std::thread(RecvData, std::ref(Server)).detach();

	while (true)
	{
		ZeroMemory(Name, sizeof(Name));
		std::cout << "�޼��� : ";
		std::cin >> Name;

		send(Server, Name, sizeof(Name), 0);
	}

	return 0;
}
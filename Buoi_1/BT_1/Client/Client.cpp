// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"

int main()
{
	// Khoi tao Winsock
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// Khoi tao socket
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Khai bao dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	system("PAUSE");

	// Ket noi den server
	int ret = connect(client, (SOCKADDR *)&addr, sizeof(addr));

	char buf[256];
	while (1)
	{
		printf("Nhap du lieu tu ban phim: ");
		gets_s(buf, sizeof(buf));

		send(client, buf, strlen(buf), 0);

		if (strncmp(buf, "exit", 4) == 0)
			break;
	}

	closesocket(client);
	WSACleanup();

	return 0;
}
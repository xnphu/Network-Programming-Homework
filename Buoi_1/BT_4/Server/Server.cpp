// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "winsock2.h"
#include "sstream"

struct information {
	char computerName[128];
	char diskNameChar[128];
	int size[10];
};

int main(int argc, char *argv[])
{
	std::istringstream iss(argv[1]);
	int port;
	iss >> port;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);
	printf("Server is listening... \n");
	SOCKET client = accept(listener, NULL, NULL);
	printf("Client connected \n");
	char buf[1024];
	int ret = recv(client, buf, sizeof(buf), 0);

	buf[ret] = 0;
	printf("Received: %s\n", buf);

	struct information info;
	memcpy(&info, buf, sizeof(info));
	printf("Computer's name: %s \n", info.computerName);
	for (int i = 0; i < sizeof(info.diskNameChar) / sizeof(char); i++) {
		if (info.diskNameChar[i] >= 'A' && info.diskNameChar[i] <= 'Z')
			printf("Hard disk %c has %d MB \n", info.diskNameChar[i], info.size[i]);
	}

	closesocket(client);
	closesocket(listener);

	system("PAUSE");

	WSACleanup();

	return 0;
}
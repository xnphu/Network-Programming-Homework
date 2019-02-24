// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include "winsock2.h"

#include "sstream"
int main(int argc, char *argv[])
{

	std::istringstream iss(argv[1]);
	int port;
	//convert to int ss
	if (iss >> port) {
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);

		SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		bind(listener, (SOCKADDR *)&addr, sizeof(addr));
		listen(listener, 5);
		printf("Server is listening at port: %d \n", port);
		while (true) {
			SOCKET client = accept(listener, NULL, NULL);
			char buf[1024];

			printf("Client connected");

			// Read file
			FILE *f = fopen(argv[2], "rb");
			FILE *fw = fopen(argv[3], "w");
			//gui noi dung file
			while (true) {
				int read = fread(buf, 1, sizeof(buf), f);
				if (read > 0) {
					send(client, buf, read, 0);
				}
				else {
					break;
				}
			}

			fclose(f);

			// Write file
			while (true) {
				char bufRecv[1024];
				int ret = recv(client, bufRecv, sizeof(bufRecv), 0);
				bufRecv[ret] = 0;
				printf("Client send: %s", bufRecv);
				int write = fwrite(bufRecv, 1, ret, fw);
			}
			fclose(fw);
		}
	}
	WSACleanup();
	return 0;
}
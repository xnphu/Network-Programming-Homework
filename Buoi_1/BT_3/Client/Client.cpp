// Client.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include "sstream"
#include "ws2tcpip.h"
#include "winsock2.h"
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#define INFO_BUFFER_SIZE 32767

struct information {
	char name[128];
	char diskNameChar[128];
	int size[10];
};

int main(int argc, char *argv[])
{
	TCHAR  temporName[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	DWORD mydrives = 100;
	char lpBuffer[100];
	int c = 0;
	int counterSize = 0;
	__int64 freeBytesToCaller, total, free;
	struct information Info;
	char allName[128];

	bufCharCount = INFO_BUFFER_SIZE;
	if (!GetComputerName(temporName, &bufCharCount))
		printf("Can't get name..");
	char computerName[512];
	wcstombs(computerName, temporName, wcslen(temporName) + 1);

	strncpy_s(Info.name, computerName, sizeof(computerName));
	printf("Computer's name: %s \n", Info.name);

	int getDiskname = GetLogicalDriveStrings(mydrives, (LPWSTR)lpBuffer);

	for (int i = 0; i < getDiskname * 2; i++) {
		if ('A' <= lpBuffer[i] && lpBuffer[i] <= 'Z') {
			allName[c] = lpBuffer[i];
			c++;
		}
	}

	for (int i = 0; i < c; i++) {
		char Dname[5];
		Dname[0] = allName[i];
		Dname[1] = ':';
		Dname[2] = '\\';
		Dname[3] = '\\';
		Dname[4] = 0;
		wchar_t wtext[20];
		mbstowcs(wtext, Dname, strlen(Dname) + 1);
		LPWSTR ptr = wtext;
		bool getDiskStorage = GetDiskFreeSpaceEx(ptr, (PULARGE_INTEGER)&freeBytesToCaller, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
		if (getDiskStorage) {
			Info.size[counterSize] = (int)(total / 1048576);
			Info.diskNameChar[counterSize] = allName[i];
			printf("o dia %c co dung luong %d MB!\n", Info.diskNameChar[i], Info.size[counterSize], total);
			counterSize++;
		}
	}
	Info.diskNameChar[counterSize + 1] = 0;
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::istringstream iss(argv[2]);
	int port;
	SOCKADDR_IN addr;
	
	if (iss >> port)
	{
		printf("\n connect den %s o port %d", argv[1], port);

		if (strcmp("localhost", argv[1]) == 0) {
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		}
		else {
			addrinfo * result;
			int res = getaddrinfo(argv[1], "http", NULL, &result);
			memcpy(&addr, result->ai_addr, result->ai_addrlen);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		int ret = connect(client, (SOCKADDR *)&addr, sizeof(addr));
		send(client, (char *)&Info, sizeof(Info), 0);

	}
	closesocket(client);
	WSACleanup();
	system("PAUSE");
	return 0;
}

/*
	Simple winsock Server
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#define DS_TEST_PORT (u_short)68000

#pragma warning(disable : 4996)

int main()
{
	// Initialise winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	printf("\nInitialising Winsock...");
	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0)
	{
		fprintf(stderr, "\nWinsock setup fail! Error Code : %d\n", WSAGetLastError());
		return 1;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		fprintf(stderr, "\nSocket creationg fail! Error Code : %d\n", WSAGetLastError());
		return 1;
	}

	printf("\nSocket created.");

	// Bind the socket (ip address and port)
	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(DS_TEST_PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (SOCKADDR*)&hint, sizeof(hint));

	// Setup the socket for listening
	listen(listening, SOMAXCONN);

	// Wait for connection
	SOCKADDR_IN client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (SOCKADDR*)&client, &clientSize);

	// Main program loop
	char strMsg[1204];
	char strRec[1024];

	int i = 1;

	strcpy(strMsg, "100 OK");
	printf("\n%s\n", strMsg);
	send(clientSocket, strMsg, (int)strlen(strMsg) + 1, 0);

	while (true)
	{
		ZeroMemory(strRec, 1024);
		int bytesReceived = recv(clientSocket, strRec, 1024, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("\nReceive error!\n");
			break;
		}
		if (bytesReceived == 0)
		{
			printf("\nClient disconnected!\n");
			clientSocket = accept(listening, (SOCKADDR*)&client, &clientSize);
		}

		printf("%i : %s\n", i++, strRec);

		if (strcmp(strupr(strRec), "QUIT") == 0)
		{
			strcpy(strMsg, "400 BYE");
			send(clientSocket, strMsg, (int)strlen(strMsg) + 1, 0);
			clientSocket = accept(listening, (SOCKADDR*)&client, &clientSize);

			strcpy(strMsg, "100 OK");
			printf("\n%s\n", strMsg);
			send(clientSocket, strMsg, (int)strlen(strMsg) + 1, 0);
		}
		else
		{
			send(clientSocket, strRec, bytesReceived + 1, 0);
		}
	}

	// Close listening socket
	closesocket(listening);

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	return 0;
}
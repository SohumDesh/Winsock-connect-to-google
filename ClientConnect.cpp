#include <iostream>
#include <string>

#include <winsock2.h>
#include <windows.h >
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define newline std::endl

void main() {

	/*
	1. Initialize winsock
	2. Create a socket object
	3. Create the hints structure, and populate it with the ip address of the google server
		- Connect on port 80
		- Use google ipv4 : 93.184.216.34
	4. connect the socket
	5. Send and recive data
	6. Close/cleanup socket
	*/

//Initializing Winsock: 

	std::cout << "[+] Checkpoint 1. Initalizing\n";
	WSADATA wsaData;
	int iResult; 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0) { //error checking
		std::cerr << "[x] Checkpoint 1 - Failed: \n\tCannot initalize winsock " << iResult << newline;
		return;
	}
	std::cout << "[+] Checkpoint 1 - Complete\n\n";

//Create Socket:

	std::cout << "[+] Checkpoint 2 - SOCKET\n";

	SOCKET googleConnectSock = socket(AF_INET, SOCK_STREAM, 0);

	if (googleConnectSock == INVALID_SOCKET) { //error checking
		std::cerr << "[x] Checkpoint 2 - Failed: \n\tFailed to create socket " << WSAGetLastError() << newline;
		WSACleanup();
		return;
	}
	std::cout << "[+] Checkpoint 2 - Complete\n\n";

//Create hints struct
	std::string IP = "93.184.216.34";

	sockaddr_in ipv4Struct;
	ipv4Struct.sin_family = AF_INET;
	ipv4Struct.sin_port = htons(80); //convert port # to little ndn
	inet_pton(AF_INET, IP.c_str(), &ipv4Struct.sin_addr); //add the address to the structure

//connect to google

	std::cout << "[+] Checkpoint 3 - Connection\n";
	std::cout << "\t[-] This might take a while...\n";

	if (connect(googleConnectSock, (sockaddr*)&ipv4Struct, sizeof(ipv4Struct)) == SOCKET_ERROR) { //pass the struct in as a sockaddr pointer type
		std::cerr << "[x] Checkpoint 3 - Failed \n\tCould not connect: " << WSAGetLastError() << newline;
		closesocket(googleConnectSock);
		WSACleanup();
		return;
	}

	std::cout << "[+] Checkpoint 3 - Completed\n\n";

//send and recive data:
	/*
	send a command to google and get it to send back it's html data
		-> "GET / HTTP/1.1\r\n\r\n"
	*/

	std::string message = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";

	std::cout << "[o] SENDING...\n";

	if (send(googleConnectSock, message.c_str(), message.size() + 1, 0) == SOCKET_ERROR) {
		std::cerr << "[x] SEND FAILED \n\tCould not send data to destination: " << WSAGetLastError() << newline;
		closesocket(googleConnectSock);
		WSACleanup();
		return;
	}
	std::cout << "[o] DATA SENT...\n";

	char serverReply[2048];

	if (recv(googleConnectSock, serverReply, 2048, 0) < 0) {
		std::cerr << "Receive Failed: " << WSAGetLastError() << newline;
		closesocket(googleConnectSock);
		WSACleanup();
		return;
	}

	std::cout << "Recived Data...\n";
	std::cout << "Attemping to print..\n";
	std::cout << (serverReply);

	closesocket(googleConnectSock);
	WSACleanup();

}
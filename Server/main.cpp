#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <sstream>
#include <list>
#include <map>


#pragma comment(lib, "ws2_32.lib")


class Server
{
private:
	WSAData data;

	addrinfo hints;
	addrinfo* serverInfo = nullptr;

	SOCKET ListenSocket;

	//true until server work
	bool itWorks = true;

	std::map<SOCKET, std::string> connectionsInfo;

public:
	Server() { }

	~Server() {
		//clear("", serverInfo);
		for (auto& pair : connectionsInfo) {
			closesocket(pair.first);
		}
		WSACleanup();
	}


	bool startup()
	{
		int res = WSAStartup(MAKEWORD(2, 2), &data);
		if (res != 0) {
			std::cout << "Startup failed with code " << res << '\n';
			return 1;
		}

		setHints();
		res = getaddrinfo(NULL, "7770", &hints, &serverInfo);
		if (res != 0) {
			std::stringstream ss;
			ss << "getaddrinfo error. Code : " << res << '\n';
			clear(ss.str());
			return false;
		}

		ListenSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			clear("Connection ListenSocket failed.\n", serverInfo);
			return false;
		}

		res = bind(ListenSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
		if (res == SOCKET_ERROR) {
			clear("binding socket failed.\n", serverInfo, &ListenSocket);
			return false;
		}

		res = listen(ListenSocket, SOMAXCONN);
		if (res == SOCKET_ERROR) {
			clear("listen failed.\n", serverInfo, &ListenSocket);
			return false;
		}

		std::cout << "Server starts work.\n";
		return true;
	}

	void execute()
	{
		std::thread read_thread(&Server::receiveMessageFromClients, this);
		std::thread accept_thread(&Server::acceptToServer, this);

		read_thread.join();
		accept_thread.join();
	}

private:
	void acceptToServer()
	{
		SOCKET socket;
		while (true) {
			socket = accept(ListenSocket, nullptr, nullptr);
			if (socket != INVALID_SOCKET) {
				connectionsInfo[socket];
				std::cout << "Client #" << connectionsInfo.size() << " connected.\n";
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void clear(const std::string& message, addrinfo* info = nullptr, SOCKET* socket = nullptr)
	{
		std::cout << message;
		if (info) {
			freeaddrinfo(info);
		}
		if (socket) {
			closesocket(*socket);
		}
	}

	void setHints()
	{
		//clear fields of hints
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
	}

	void sendMessageToClients(const char* buffer)
	{
		for (auto& pair : connectionsInfo) {
			send(pair.first, buffer, strlen(buffer), 0);
		}
	}

	void receiveMessageFromClients()
	{
		char receivebuffer[256];
		while (true) {
			for (auto& pair : connectionsInfo) {
				if (recv(pair.first, receivebuffer, 256, 0) != SOCKET_ERROR) {
					// if it's first message -
					// it's a nickname
 					if (pair.second == "") {
						pair.second = receivebuffer;
						std::cout << "User " << receivebuffer << " authorized\n";
					}
					else {
						std::stringstream ss;
						ss << pair.second << ':' << receivebuffer << '\n';
						std::cout << ss.str();
						sendMessageToClients(ss.str().c_str());
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
};

int main()
{
	Server server;
	
	
	if (!server.startup()) {
		return 1;
	}

	server.execute();
}
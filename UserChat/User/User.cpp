#include "User.h"

User::User()
{
}

User::~User()
{
    clear("", serverInfo, &ConnectSocket);
    WSACleanup();
}

bool User::startup(const std::string &ipAddr, const std::string &port)
{
	int res = WSAStartup(MAKEWORD(2, 2), &data);
	if (res != 0)
	{
		std::cout << "Startup failed with code " << res << '\n';
		return false;
	}

	setHints();

	// 127.0.0.1 - localhost
	res = getaddrinfo(ipAddr.c_str(), port.c_str(), &hints, &serverInfo);
	if (res != 0)
	{
		clear("getaddrinfo error. Code : " + std::to_string(res) + '\n');
		return false;
	}

	ConnectSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		clear("Connection ConnectSocket failed.\n", serverInfo);
		return false;
	}

	res = connect(ConnectSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		clear("Unable connect to server.\n", serverInfo, &ConnectSocket);
		return false;
	}

	std::cout << "Connected to server.\n";
	return true;
}

void User::setHints()
{
	// clear fields of hints
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

bool User::sendMessageToServer(const char *message)
{
	if (send(ConnectSocket, message, 256, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void User::sendNickname(const char *nickname)
{
	send(ConnectSocket, nickname, 256, 0);
}

std::string User::receiveMessageFromServer()
{
	char receivebuffer[256];
	if (recv(ConnectSocket, receivebuffer, 256, 0) != SOCKET_ERROR)
	{
		return receivebuffer;
	}
	return "";
}

void clear(const std::string &message, addrinfo *info, SOCKET *socket)
{
	std::cout << message;
	if (info)
	{
		freeaddrinfo(info);
	}
	if (socket)
	{
		closesocket(*socket);
	}
	WSACleanup();
}

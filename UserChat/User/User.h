#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

void clear(const std::string &message, addrinfo *info = nullptr, SOCKET *socket = nullptr);

class User
{
private:
    WSAData data;

    addrinfo hints;
    addrinfo *serverInfo = nullptr;

    SOCKET ConnectSocket;

    std::thread receive_thread;

public:
    User();

    ~User();

    bool startup(const std::string &ipAddr, const std::string &port);

    void execute();

    bool sendMessageToServer(const char *message);

    void sendNickname(const char *nickname);

    std::string receiveMessageFromServer();

    bool hasNewMessage();

    const char *getLastMessage();

private:
    void setHints();
};

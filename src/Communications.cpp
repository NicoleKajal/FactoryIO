
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h>
#include <iostream>
#include "Communications.hpp"

using namespace std;

const int32_t STATUS_SUCCESS(0);
const int32_t STATUS_FAILURE(-1);
std::string START_OF_TEXT = "\a\a";
std::string END_OF_TEXT = "\b\b";

Communications::Communications(CommunicationsEventHandler* communicationsEventHandler)
    : m_socketFd(STATUS_FAILURE), m_eventHandler(communicationsEventHandler), m_thread(nullptr) {
}

bool Communications::openSocket(string ipAddress, uint32_t port) {
    
    const int USE_DEFAULT_PROTOCOL(0);
    m_socketFd = socket(AF_INET, SOCK_STREAM, USE_DEFAULT_PROTOCOL);
    
    if (m_socketFd == STATUS_FAILURE) {
        std::cerr << "failed to create socket" << std::endl;
        return false;
    }
    
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    server.sin_port = htons(port);
    
    if (connect(m_socketFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        std::cout << "failed to connect" << std::endl;
        return false;
    }
    m_eventHandler->handleConnectionEstablished();
    m_thread = new std::thread(&Communications::receiverThread, this);
    return true;
}

void Communications::sendMessage(std::string text) {
    text = START_OF_TEXT + text + END_OF_TEXT;
    if (send(m_socketFd, text.c_str(), text.size(), 0) == STATUS_FAILURE) {
        close(m_socketFd);
        m_eventHandler->handleConnectionLost();
        throw std::runtime_error("failed send");
    }
}

void Communications::receiverThread() {
    std::cout << "Started receiver thread" << std::endl;

    const int BUFFER_SIZE(8 * 1024);
    char buffer[BUFFER_SIZE];

    size_t startIndex = string::npos;
    size_t endIndex = string::npos;
    string data;

    for (;;) {
        if (startIndex == string::npos)
            startIndex = data.find(START_OF_TEXT, 0);

        if (startIndex != string::npos)
            endIndex = data.find(END_OF_TEXT, startIndex);

        if (endIndex != string::npos) {
            string messageText = data.substr(startIndex + START_OF_TEXT.size(), endIndex - (startIndex + START_OF_TEXT.size()));
            m_eventHandler->handleMessageReceived(messageText);
            data = data.substr(endIndex + END_OF_TEXT.size(), data.length() - (endIndex + END_OF_TEXT.size()));
            startIndex = string::npos;
            endIndex = string::npos;
            if (data.length() > 0) {
                continue;
            }
        }

        int receivedCount = recv(m_socketFd, buffer, BUFFER_SIZE, 0);

        if (receivedCount <= 0) {
            close(m_socketFd);
            return;
        }

        buffer[receivedCount] = '\0';
        string newData(buffer, receivedCount);
        data.append(newData);
    }
}


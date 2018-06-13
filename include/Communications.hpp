/* 
 * File:   Communications.hpp
 * Author: Nicole
 *
 * Created on June 1, 2018, 10:08 PM
 */

#ifndef COMMUNICATIONS_HPP
#define COMMUNICATIONS_HPP

#include <stdint.h>
#include <string>
#include <thread>
#include "CommunicationsEventHandler.hpp"

class Communications {
public:
    Communications(CommunicationsEventHandler* communicationsEventHandler);
    bool openSocket(std::string ipAddress, uint32_t port);
    void sendMessage(std::string text);

private:
    void receiverThread();

    int32_t m_socketFd;
    CommunicationsEventHandler* m_eventHandler;
    std::thread* m_thread;
};

#endif


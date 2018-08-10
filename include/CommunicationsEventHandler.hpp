/* 
 * File:   CommunicationsEventHandler.hpp
 * Author: 6749
 *
 * Created on June 10, 2018, 4:18 PM
 */

#pragma once
#ifndef COMMUNICATIONS_EVENT_HANDLER_HPP
#define COMMUNICATIONS_EVENT_HANDLER_HPP

#include <string>

class CommunicationsEventHandler {
public:
    virtual void handleConnectionEstablished() = 0;
    virtual void handleConnectionLost() = 0;
    virtual void handleMessageReceived(std::string message) = 0;
};

#endif


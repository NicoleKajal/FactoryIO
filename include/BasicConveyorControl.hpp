/* 
 * File:   BasicConveyorControl.hpp
 * Author: Nicole
 *
 * Created on June 21, 2018, 8:51 PM
 */

#pragma once
#ifndef BASIC_CONVEYOR_CONTROL_HPP
#define BASIC_CONVEYOR_CONTROL_HPP

#include <stdint.h>
#include <mutex>
#include <string>
#include "Parts.hpp"
#include "Factory.hpp"
#include "Sensors.hpp"

class BasicConveyorControl {
public:
    BasicConveyorControl(Factory& factory, std::string stationPrefix, int32_t maxBoxCount);
    void start();
    void stop();
    void waitUntilDone();

private:  
    void handleBoxEntry();
    void handleBoxExit();
    
    Factory&              m_factory;
    Emitter               m_emitter;
    Remover               m_remover;
    DigitalRollerConveyor m_entryConveyor;
    DigitalRollerConveyor m_exitConveyor;
    RetroreflectiveSensor m_entrySensor;
    RetroreflectiveSensor m_exitSensor;
    int32_t               m_maxBoxCount;
    int32_t               m_boxCount;
    std::thread*          m_entryThread;
    std::thread*          m_exitThread;
    std::mutex            m_boxCountMutex;
};

#endif 
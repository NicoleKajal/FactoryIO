/* 
 * File:   BasicPackingStation.hpp
 * Author: Nicole
 *
 * Created on June 22, 2018, 2:10 PM
 */

#pragma once
#ifndef BASIC_PACKING_STATION_HPP
#define BASIC_PACKING_STATION_HPP


#include <atomic>
#include <thread>
#include "Parts.hpp"
#include "Factory.hpp"
#include "Sensors.hpp"

class BasicPackingStation {
public:
    BasicPackingStation(Factory& factory);
    void packingManager();
    void palletManager();
    void boxConveyorManager();
    void start();
    void stop();
    void waitUntilDone();
    
private:
    Factory&                m_factory;
    Emitter                 m_palletEmitter;
    Remover                 m_palletRemover;
    DigitalRollerConveyor   m_palletEntryConveyor;
    DigitalRollerConveyor   m_palletExitConveyor;
    RollerStop              m_palletRollerStop;
    RetroreflectiveSensor   m_palletEntrySensor;
    RetroreflectiveSensor   m_palletPackingLocationSensor;
    Emitter                 m_boxEmitter;
    DigitalRollerConveyor   m_boxConveyor;
    RetroreflectiveSensor   m_boxEntrySensor;
    RetroreflectiveSensor   m_boxPackingLocationSensor;
    StopBlade               m_boxStopBlade;
    PickAndPlace            m_pickAndPlace;
    std::thread*            m_palletManagerThread;
    std::thread*            m_boxConveyorManagerThread;
    std::thread*            m_packingManagerThread;
    std::atomic_bool        m_boxReady;
    std::atomic_bool        m_palletReady;
    DisplayActuator         m_digitalDisplay;
};

#endif 


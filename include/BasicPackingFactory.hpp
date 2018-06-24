/* 
 * File:   BasicPackingFactory.hpp
 * Author: Nicole
 *
 * Created on June 22, 2018, 2:10 PM
 */

#pragma once
#ifndef BASIC_PACKING_FACTORY_HPP
#define BASIC_PACKING_FACTORY_HPP


#include <atomic>
#include <thread>
#include "Parts.hpp"
#include "Factory.hpp"
#include "Sensors.hpp"

class BasicPackingFactory {
public:
    BasicPackingFactory(Factory& factory);
    void packingManager();
    void palletManager();
    void boxConveyorManager();
    void start();
    void stop();
    void waitUntilDone();
    
private:
    Factory&                    m_factory;
    Station                     m_palletStation;
    Station                     m_boxStation;
    Station                     m_packingStation;
    Emitter                     m_palletEmitter;
    Remover                     m_palletRemover;
    DigitalRollerConveyor       m_palletEntryConveyor;
    DigitalRollerConveyor       m_palletExitConveyor;
    RollerStop                  m_palletRollerStop;
    RetroreflectiveSensor       m_palletEntrySensor;
    RetroreflectiveSensor       m_palletPackingLocationSensor;
    Emitter                     m_boxEmitter;
    DigitalRollerConveyor       m_boxConveyor;
    RetroreflectiveSensor       m_boxEntrySensor;
    RetroreflectiveSensor       m_boxPackingLocationSensor;
    StopBlade                   m_boxStopBlade;
    PickAndPlace                m_pickAndPlace;
    DisplayNumberActuator<int>  m_digitalDisplay;
    std::thread*                m_palletManagerThread;
    std::thread*                m_boxConveyorManagerThread;
    std::thread*                m_packingManagerThread;
    std::atomic_bool            m_boxReady;
    std::atomic_bool            m_palletReady;
    std::atomic_bool            m_palletFull;
};

#endif 


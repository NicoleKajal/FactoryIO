/* 
 * File:   Station.hpp
 * Author: Nicole
 *
 * Created on June 23, 2018, 5:53 PM
 */

#ifndef STATION_HPP
#define STATION_HPP

#include "Factory.hpp"
#include "ActuatorSerializer.hpp"
#include "SensorDeserializer.hpp"

class Station {
public:
    Station(Factory& factory)
    :  m_factory(factory), m_actuatorList(), m_mutex() {
    }
    
    void add(ActuatorSerializer* actuatorSerializer) {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        m_actuatorList.push_back(actuatorSerializer);
    }
    
    void add(SensorDeserializer* sensorDeserializer) {
        m_factory.add(sensorDeserializer);
//        std::lock_guard<std::mutex> scopedLock(m_mutex);
//        m_sensorList.push_back(sensorDeserializer);
    }
    void applyChanges() {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        m_factory.applyChanges(m_actuatorList);
    }
    
    void waitForSensorChange() {
        m_factory.waitForSensorChange();
    }
private:
    Factory&                       m_factory;
    std::list<ActuatorSerializer*> m_actuatorList;
//    std::list<SensorDeserializer*> m_sensorList;
    std::mutex                     m_mutex;
    
};

#endif 


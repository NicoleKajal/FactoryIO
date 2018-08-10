/* 
 * File:   Factory.hpp
 * Author: Nicole
 *
 * Created on June 8, 2018, 8:49 PM
 */

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <list>
#include <mutex>
#include <string>
#include <condition_variable>
#include "Communications.hpp"
#include "ActuatorSerializer.hpp"
#include "SensorDeserializer.hpp"

class Factory {
public:
      Factory();
      Factory& add(ActuatorSerializer* actuatorSerializer);
      Factory& add(SensorDeserializer* sensorDeserializer);
      void applyChanges();
      void applyChanges(std::list<ActuatorSerializer*>& actuatorSerializerList);
      bool start();
      void handleNewSensorValues(std::string jsonString);
      void waitForSensorChange();
      void loadSensorValues();
    
private:    

    const std::string IP_ADDRESS = "10.0.0.100";
    const uint32_t TCP_PORT = 910;
    
    Communications                  m_communications;
    std::list<ActuatorSerializer*>  m_actuatorSerializerList;
    std::list<SensorDeserializer*>  m_sensorDeserializerList;
    std::mutex                      m_mutex;
    std::condition_variable         m_changeControl;
};

#endif


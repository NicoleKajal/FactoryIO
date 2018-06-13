
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Factory.hpp"

using namespace rapidjson;

class FactoryCommunicationsEventHandler : public CommunicationsEventHandler {
public:
    FactoryCommunicationsEventHandler(Factory* factory)
        : m_factory(factory) {
    }
    virtual void handleConnectionEstablished() {
        
    }
    virtual void handleConnectionLost() {
        
    }
    virtual void handleMessageReceived(std::string message) {
        std::cout << "Received: " << message << std::endl;
        m_factory->handleNewSensorValues(message);
        
    }
    private:
        Factory* m_factory;
};

Factory::Factory() 
    : m_communications(new FactoryCommunicationsEventHandler(this)),
      m_actuatorSerializerList(), m_sensorDeserializerList(), m_mutex(), 
      m_changeControl(), m_changed(false) {
}

bool Factory::start() {
    return m_communications.openSocket(IP_ADDRESS, TCP_PORT);
}

Factory& Factory::add(ActuatorSerializer* actuatorSerializer) {
    std::lock_guard<std::mutex> scopedLock(m_mutex);
    m_actuatorSerializerList.push_back(actuatorSerializer);
    return *this;
}

Factory& Factory::add(SensorDeserializer* sensorDeserializer) {
    std::lock_guard<std::mutex> scopedLock(m_mutex);
    m_sensorDeserializerList.push_back(sensorDeserializer);
    return *this;
}

void Factory::applyChanges() {
    std::lock_guard<std::mutex> scopedLock(m_mutex);
    rapidjson::Document jsonDocument;
    jsonDocument.SetObject();
    for (ActuatorSerializer* actuatorSerializer : m_actuatorSerializerList)
        actuatorSerializer->serialize(jsonDocument, true);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    jsonDocument.Accept(writer);

    m_communications.sendMessage(buffer.GetString());
}

void Factory::handleNewSensorValues(std::string jsonString) {
    std::lock_guard<std::mutex> scopedLock(m_mutex);
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(jsonString.c_str());
    for (SensorDeserializer* sensorDeserializer : m_sensorDeserializerList) {
        if (sensorDeserializer->deserialize(jsonDocument)) {
            m_changed = true;
            m_changeControl.notify_all();
        }
    }
}

void Factory::waitForChange() {
    std::unique_lock<std::mutex> scopedLock(m_mutex);
    m_changeControl.wait(scopedLock, [this] {return m_changed;});
    m_changed = false;
}

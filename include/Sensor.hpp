#pragma once
#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <mutex>
#include <string>
#include <condition_variable>
#include "rapidjson/document.h"
#include "Factory.hpp"
#include "SensorDeserializer.hpp"

/**
 * An output device that is read by a user to obtain information about a factory
 * component.  The "device present" sensor of a "pick & place" is an example of
 * a sensor.  Its data type is boolean, where a value of true indicates that a
 * box is present and false indicates that one is not.
 * @param <T>
 */

template<typename T>
class Sensor : public SensorDeserializer {
public:

    /**
     * Initializes a sensor with a name and a default value.
     *
     * @param name      Name of the sensor
     * @param value     Sensor's default value
     */
    Sensor(Factory& factory, std::string name, T value)
    : m_name(name), m_value(value), m_changed(false), m_mutex(), m_changeControl() {
        factory.add(this);
    }

    /**
     * Gets the name of the sensor.
     *
     * @return  The name of the sensor.
     */
    inline std::string getName() const {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        return m_name;
    }

    /**
     * Sets the value of the sensor with a new value if the new value is specified in the JSON
     * document.
     * 
     * @param jsonDocument  Contains new sensor values
     * 
     * @return true if the sensor received an update
     */
    virtual bool deserialize(const rapidjson::Document& jsonDocument) {
        // If the document has a value for a sensor with this name and with the
        // same type as the value, set the new value.
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        if ((jsonDocument.HasMember(m_name.c_str())) && (jsonDocument[m_name.c_str()].Is<T>())) {
            T value = jsonDocument[m_name.c_str()].Get<T>();
            if (value != m_value) {
                m_value = value;
                m_changed = true;
                m_changeControl.notify_all();
                return true;
            }
        }
        return false;
    }

    void waitForChange() {
        std::unique_lock<std::mutex> scopedLock(m_mutex);
        if (!m_changed) {
            m_changeControl.wait(scopedLock);
            m_changed = false;
        }
    }
    
protected:
        
    /**
     * Gets the value of the sensor.
     *
     * @return  The value of the sensor.
     */
    inline T getValue() const {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        return m_value;
    }

    /**
     * Sets the value of the sensor.
     *
     * @param value     Sensor's new value
     */
    inline void setValue(T value) {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        m_value = value;
    }
    
private:
    std::string                     m_name;             // Name of the sensor
    T                               m_value;            // Value of the sensor
    bool                            m_changed;          // Used to report changed value
    mutable std::mutex              m_mutex;            // Provides thread-safety for the class
    mutable std::condition_variable m_changeControl;
};

#endif

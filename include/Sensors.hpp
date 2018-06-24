#pragma once
#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <stdint.h>
#include <mutex>
#include <string>
#include <condition_variable>
#include "rapidjson/document.h"
#include "Station.hpp"
#include "SensorDeserializer.hpp"

/**
 * An output device that is read by a user to obtain information about a station
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
    Sensor(Station& station, std::string name, T value)
    : m_name(name), m_value(value), m_changed(false), m_mutex(), m_changeControl() {
        station.add(this);
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

/**
 * Generic "Item Detected" sensor, which is used by the Capacitive Sensor, Diffuse Sensor, and the
 * Inductive Sensor.
 */
class ItemDetectedSensor : public Sensor<bool> {
public:
    ItemDetectedSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline bool itemDetected() const {
        return getValue();
    }
};

/**
 * Generic "Position" sensor, which is used by the pick and place station.
 */
class PositionSensor : public Sensor<float> {
public:
    PositionSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline float getPosition() const {
        return getValue();
    }
};

/**
 * Generic "Weight" sensor, which is used by the pick and place station.
 */
class WeightSensor : public Sensor<float> {
public:
    WeightSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline float getWeight() const {
        return getValue();
    }
};

/**
 * Generic "Limit" sensor, which is used by the pick and place station.
 */
class LimitSensor : public Sensor<bool> {
public:
    LimitSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline bool atLimit() const {
        return getValue();
    }
};

/**
 * Capacitive Sensor, which is used for close detection of any material. It is equipped with an LED,
 * which indicates the presence of an object within its range.  Details:
 *      LED: green (detecting)
 *      Detectable materials: solids and liquids.
 *      Sensing range: 0 - 0.2 m
 */
typedef ItemDetectedSensor CapacitiveSensor;

/**
 * Diffuse photoelectric sensor which can detect any solid objects.  Details:
 *     LED: red (detecting)
 *     Detectable materials: solids
 *     Sensing range: 0 - 1.6 m
 */
typedef ItemDetectedSensor DiffuseSensor;

/**
 * Proximity sensor used for close detection of conductive materials. It is equipped with an LED,
 * which indicates the presence of an object within its range.  Details:
 *     LED: red (detecting)
 *     Detectable materials: conductive
 *     Sensing range: 0 - 0.1 m
 */
typedef ItemDetectedSensor InductiveSensor;

/**
 * A set of parallel light beams used to create a sensing field. To ensure the synchronization of
 * both devices (emitter and receiver), they must by properly aligned and facing each other. Once
 * this alignment is guaranteed (indicated by a green LED), all beams can be interrupted without
 * breaking the synchronization.  Details:
 *     Number of beams: 8
 *     Range: 1.5 m
 *     Detectable materials: solids
 *     Analog value = 10 * number of interrupted beams / 8
 * 
 * @param station
 * @param name
 */

class LightArraySensor : public Sensor<uint32_t> {
public:

    LightArraySensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline uint32_t getArrayBitMask() const {
        return getValue();
    }

    inline bool bitSet(uint32_t bitPosition) {
        return (getValue() & (1 << bitPosition) == bitPosition);
    }
};

/**
 * Retroreflective Sensor and Reflector, which unlike the other sensors, the retroreflective sensor 
 * requires a reflector. To work properly, it must be aligned with the reflector. It is equipped 
 * with two LED which indicate the correct alignment (green) and detection status (yellow).  Details:
 *      Green LED: aligned with reflector
 *      Yellow LED: light beam not interrupted
 *      Detectable materials: solids
 *      Sensing range: 0 - 6 m
 */
class RetroreflectiveSensor : public Sensor<bool> {
public:

    RetroreflectiveSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline bool beamDetected() {
        return getValue();
    }
};

/**
 * Items that can be detected by a vision sensor.
 */
enum VisionSensorItems {
    ITEM_NONE = 0,
    ITEM_BLUE_RAW_MATERIAL = 1,
    ITEM_BLUE_BASE = 2,
    ITEM_BLUE_LID = 3,     
    ITEM_GREEN_RAW_MATERIAL = 4,
    ITEM_GREEN_BASE = 5,
    ITEM_GREEN_LID = 6,   
};
/**
 * The Vision Sensor recognizes Raw Materials, Product Lids and Product Bases, and their respective
 * colors.  Details:
 *     LED: red (detecting)
 *     Detectable materials: Raw Materials, Product Bases, Product Lids
 *     Sensing range: 0.375 - 2 m
 * 
 * @param station
 * @param name
 */
class VisionSensor : public Sensor<uint32_t> {
public:

    VisionSensor(Station& station, std::string name)
    : Sensor(station, name, false) {
    }

    inline VisionSensorItems item() {
        return static_cast<VisionSensorItems>(getValue());
    }
};

#endif

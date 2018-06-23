#pragma once
#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <stdint.h>
#include "Factory.hpp"
#include "Sensor.hpp"

/**
 * Generic "Item Detected" sensor, which is used by the Capacitive Sensor, Diffuse Sensor, and the
 * Inductive Sensor.
 */
class ItemDetectedSensor : public Sensor<bool> {
public:
    ItemDetectedSensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
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
    PositionSensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
    }

    inline float getPosition() const {
        return getValue();
    }
};

/**
 * Generic "Limit" sensor, which is used by the pick and place station.
 */
class LimitSensor : public Sensor<bool> {
public:
    LimitSensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
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
 * @param factory
 * @param name
 */

class LightArraySensor : public Sensor<uint32_t> {
public:

    LightArraySensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
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

    RetroreflectiveSensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
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
 * @param factory
 * @param name
 */
class VisionSensor : public Sensor<uint32_t> {
public:

    VisionSensor(Factory& factory, std::string name)
    : Sensor(factory, name, false) {
    }

    inline VisionSensorItems item() {
        return static_cast<VisionSensorItems>(getValue());
    }
};
  
#endif


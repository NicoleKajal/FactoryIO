#pragma once
#ifndef ACTUATORS_HPP
#define ACTUATORS_HPP

#include <mutex>
#include <string>
#include "rapidjson/document.h"
#include "Station.hpp"
#include "ActuatorSerializer.hpp"

template<typename T>
class Actuator : public ActuatorSerializer {
public:

    /**
     * Initialize the actuator.
     * 
     * @param station   The station that the actuator belongs to
     * @param name      Name of the actuator
     * @param value     Value of the actuator
     * 
     * The actuator's changed state is initially set to true so that its value will be sent to the
     * station.
     */
    Actuator(Station& station, std::string name, T value)
    : m_name(name), m_value(value), m_changed(true), m_mutex() {
        station.add(this);
    }

    inline std::string getName() const {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        return m_name;
    }

    void serialize(rapidjson::Document& jsonDocument, bool onlyIfChanged) {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        if (!onlyIfChanged || m_changed) {
            rapidjson::GenericStringRef<char> name(m_name.c_str());
            jsonDocument.AddMember(name, m_value, jsonDocument.GetAllocator());
            m_changed = false;
        }
    }

protected:
    
    inline T getValue() const {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        return m_value;
    }
    
    inline void setValue(T value) {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        if (value != m_value) {
            m_value = value;
            m_changed = true;
        }
    }
        
private:
    std::string         m_name;     // Name of the actuator
    T                   m_value;    // Value of the actuator
    bool                m_changed;  // True when a change has not been reported (through serialize)
    mutable std::mutex  m_mutex;    // Provides thread-safety for the class
};

class OnOffActuator : public Actuator<bool> {
public:
    OnOffActuator(Station& station, std::string name)
    : Actuator(station, name, false) {
    }

    inline bool getOn() const {
       return getValue();
    }

    inline void setOn(bool on) {
        setValue(on);
    }
};

class RaiseLowerActuator : public Actuator<bool> {
public:

    RaiseLowerActuator(Station& station, std::string name)
    : Actuator(station, name, false) {
    }

    inline bool getRaised() const {
       return getValue();
    }

    inline void setRaised(bool on) {
        setValue(on);
    }
};

class PositionActuator : public Actuator<float> {
public:

    PositionActuator(Station& station, std::string name)
    : Actuator(station, name, 0) {
    }

    inline float getPosition() const {
       return getValue();
    }

    inline void setPosition(float position) {
        setValue(position);
    }
};

class SpeedActuator : public Actuator<float> {
public:

    SpeedActuator(Station& station, std::string name)
    : Actuator(station, name, 0) {
    }
    inline float getSpeed() const {
       return getValue();
    }

    void setSpeed(float speed) {
        setValue(speed);
    }
};

template<typename T>
class DisplayNumberActuator : public Actuator<T> {
public:

    DisplayNumberActuator(Station& station, std::string name)
    : Actuator<T>(station, name, 0) {
    }

    inline T getNumber() const {
       return Actuator<T>::getValue();
    }

    inline void setNumber(T number) {
        Actuator<T>::setValue(number);
    }
};

typedef OnOffActuator Emitter;

typedef OnOffActuator Remover;

typedef RaiseLowerActuator RollerStop;

typedef RaiseLowerActuator StopBlade;

typedef OnOffActuator DigitalRollerConveyor;

typedef SpeedActuator BeltConveyor;

typedef SpeedActuator RollerConveyor;

typedef OnOffActuator LightIndicator;

typedef OnOffActuator WarningLight;

typedef OnOffActuator AlarmSiren;

#endif

#pragma once
#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP

#include <mutex>
#include <string>
#include <typeinfo>
#include "rapidjson/document.h"
#include "Factory.hpp"
#include "ActuatorSerializer.hpp"

template<typename T>
class Actuator : public ActuatorSerializer {
public:

    Actuator(Factory& factory, std::string name, T value)
    : m_name(name), m_value(value), m_changed(false), m_mutex() {
        factory.add(this);
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
    OnOffActuator(Factory& factory, std::string name, bool value)
    : Actuator(factory, name, value) {
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

    RaiseLowerActuator(Factory& factory, std::string name, bool value)
    : Actuator(factory, name, value) {
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

    PositionActuator(Factory& factory, std::string name, float value)
    : Actuator(factory, name, value) {
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

    SpeedActuator(Factory& factory, std::string name, float value)
    : Actuator(factory, name, value) {
    }
    inline float getSpeed() const {
       return getValue();
    }

    void setSpeed(float speed) {
        setValue(speed);
    }
};

class DisplayActuator : public Actuator<float> {
public:

    DisplayActuator(Factory& factory, std::string name, float value)
    : Actuator(factory, name, value) {
    }

    inline float getNumber() const {
       return getValue();
    }

    inline void setNumber(float on) {
        setValue(on);
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

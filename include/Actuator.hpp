#pragma once
#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP

#include <mutex>
#include <string>
#include <typeinfo>
#include "rapidjson/document.h"
#include "ActuatorSerializer.hpp"

template<typename T>
class Actuator : public ActuatorSerializer {
public:

    Actuator(std::string name, T value)
    : m_name(name), m_value(value), m_changed(false), m_mutex() {
    }

    inline std::string getName() const {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        return m_name;
    }

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

    void serialize(rapidjson::Document& jsonDocument, bool onlyIfChanged) {
        std::lock_guard<std::mutex> scopedLock(m_mutex);
        if (!onlyIfChanged || m_changed) {
            rapidjson::GenericStringRef<char> name(m_name.c_str());
            jsonDocument.AddMember(name, m_value, jsonDocument.GetAllocator());
            m_changed = false;
        }
    }

private:
    std::string         m_name;     // Name of the actuator
    T                   m_value;    // Value of the actuator
    bool                m_changed;  // True when a change has not been reported (through serialize)
    mutable std::mutex  m_mutex;    // Provides thread-safety for the class
};

#endif

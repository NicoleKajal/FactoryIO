#pragma once
#ifndef SENSOR_DESERIALIZER_HPP
#define SENSOR_DESERIALIZER_HPP

#include "rapidjson/document.h"

class SensorDeserializer
{
public:
    virtual bool deserialize(const rapidjson::Document& jsonDocument) = 0;
};

#endif


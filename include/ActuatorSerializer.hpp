#pragma once
#ifndef ACTUATOR_SERIALIZER_HPP
#define ACTUATOR_SERIALIZER_HPP

#include "rapidjson/document.h"

class ActuatorSerializer {
public:

    virtual void serialize(rapidjson::Document& jsonDocument, bool onlyIfChanged) = 0;
};

#endif


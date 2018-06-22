#pragma once
#ifndef PARTS_HPP
#define PARTS_HPP

#include <string>
#include "Actuator.hpp"
#include "Factory.hpp"
#include "Sensor.hpp"

enum Rotation {
    ROTATION_OFF,
    ROTATION_FORWARD,
    ROTATION_BACKWARD
};

enum Direction {
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_STRAIGHT
};

class OnOffPart {
public:

    OnOffPart(Factory& factory, std::string name, bool value)
    : m_actuator(name, value) {
        factory.add(&m_actuator);
    }

    inline bool getOn() const {
       return m_actuator.getValue();
    }

    inline void setOn(bool on) {
        m_actuator.setValue(on);
    }

private:
    Actuator<bool> m_actuator;
};

typedef OnOffPart Emitter;

typedef OnOffPart Remover;

typedef OnOffPart LightIndicator;

typedef OnOffPart WarningLight;

typedef OnOffPart AlarmSiren;

typedef OnOffPart StopBlade;

typedef OnOffPart DigitalRollerConveyor;

class SpeedActuator {
public:

    SpeedActuator(Factory& factory, std::string name, float value)
    : m_actuator(name, value) {
        factory.add(&m_actuator);
    }

    void setSpeed(float speed) {
        m_actuator.setValue(speed);
    }
private:
    Actuator<float> m_actuator;
};

/**
 * Belt Conveyors are used to transport light load cargo.
 * Details:
 *     Required configuration: analog
 *     Available lengths: 2, 4 and 6 m
 *     Max. conveying speed: 3 m/s
 */
typedef SpeedActuator BeltConveyor;

typedef SpeedActuator RollerConveyor;

/**
 * High speed conveyor scale used for weight control. It measured different weight ranges
 * according to the selected configuration.
 * Details:
 *     Max. conveying speed: 0.6 m/s
 *     Capacities: 20 and 100 Kg
 */
class ConveyorScale {
public:
    ConveyorScale(Factory& factory, std::string name)
    : m_rotateForward(name + " Forward", false), 
      m_rotateBackward(name + " Backward", false), 
      m_weightSensor(factory, name + " Weight", 0.0) {
        factory.add(&m_rotateForward).add(&m_rotateBackward);
    }
    
    void setRotation(Rotation rotation) {
        switch (rotation) {
            case ROTATION_FORWARD:
                m_rotateForward.setValue(true);
                m_rotateBackward.setValue(false);
                break;
            case ROTATION_BACKWARD:
                m_rotateForward.setValue(false);
                m_rotateBackward.setValue(true);
                break;
            default: 
                m_rotateForward.setValue(false);
                m_rotateBackward.setValue(false);
                break;         
        }
    }

//    float getWeight() {
//        return m_weightSensor.getValue();
//    }
private:
    Actuator<bool> m_rotateForward;
    Actuator<bool> m_rotateBackward;
    Sensor<float> m_weightSensor;
    
};

/**
 * A 45º power face arm diverter, powered by a gearmotor. Equipped with a belt that helps to
 * deviate the conveyed items onto the next part. The arm can rotate left or right according
 * to the selected configuration.
 * Details:
 *     Belt speed: 2 m/s
 *     Arm angular speed: 5 rad/s
 */
class PivotArmSorter {
public:
    PivotArmSorter(Factory& factory, std::string name)
    : m_rotateForward(name + " Forward", false), 
      m_rotateBackward(name + " Backward", false), 
      m_turn(name + " Turn", false) {
        factory.add(&m_rotateForward).add(&m_rotateBackward).add(&m_turn);
    }

    void setRotation(Rotation rotation) {
        switch (rotation) {
            case ROTATION_FORWARD:
                m_rotateForward.setValue(true);
                m_rotateBackward.setValue(false);
                break;
            case ROTATION_BACKWARD:
                m_rotateForward.setValue(false);
                m_rotateBackward.setValue(true);
                break;
            default: 
                m_rotateForward.setValue(false);
                m_rotateBackward.setValue(false);
                break;         
        }
    }
    
    void turn(bool on) {
        m_turn.setValue(on);
    }
private:
    Actuator<bool> m_rotateForward;
    Actuator<bool> m_rotateBackward;
    Actuator<bool> m_turn;
};
  
/**
 * A 45º double-sided pop-up wheel sorter used to divert items to three different directions via
 * pivoted rollers.
 * Details:
 *     Wheel radius: 0.05 m
 *     Wheels stroke: 0.003 m
 */
class PopUpWheelSorter {  
public:    
    PopUpWheelSorter(Factory& factory, std::string name)
    : m_rotateForward(name + " Forward", false), 
      m_wheelsLeft(name + " Left", false), 
      m_wheelsRight(name + " Right", false) {
        factory.add(&m_rotateForward).add(&m_wheelsLeft).add(&m_wheelsRight);
    }
    
    void rotateWheels(bool on){
        m_rotateForward.setValue(on);
    }
        
    void setDirection(Direction direction) {
        switch (direction) {
            case DIRECTION_RIGHT:
                m_wheelsLeft.setValue(false);
                m_wheelsRight.setValue(true);
                break;
            case DIRECTION_LEFT: 
                m_wheelsLeft.setValue(true);
                m_wheelsRight.setValue(false);
                break;
            default: 
                m_wheelsLeft.setValue(false);
                m_wheelsRight.setValue(false);
                break;
        }
    }
private:
    Actuator<bool> m_rotateForward;
    Actuator<bool> m_wheelsLeft;
    Actuator<bool> m_wheelsRight;
};

/**
 * Pneumatic pusher sorter equipped with two reed sensors indicating the front and ba
 *     Conveying speed: 2.5 m/s
 *     Normally centeredck limits.
 * It also includes a servo valve which can be used to set and measure the rod position.
 *   Details:
 *     Required configuration: Analog
 *     Default speed: 1 m/s
 *     Fast speed: 4 m/s
 *     Stroke: 0.9 m
 */
class Pusher {
public: 
    Pusher(Factory& factory, std::string name)
    : m_speedActuator(name + " Speed", false), 
      m_positionSensor(factory, name + " Position", false) {
        factory.add(&m_speedActuator);
    }
    
    void push(float speed) {
        m_speedActuator.setValue(speed);
    }
 //   float getPosition() {
 //       return m_positionSensor.getValue();
 //   }
private:
    Actuator<float> m_speedActuator;
    Sensor<float> m_positionSensor;
};

/**
 * A device used to consecutively position items at the same place by clamping them. Available in
 * two different configurations, left and right.  It is commonly used with Pick and Place Systems.
 * Details:
 *     Vertical stroke: 0.373 m
 *     Clamper stroke: 0.48 m
 */
#if 0
class PositioningBar {
public:
    PositioningBar(Factory& factory, std::string name)
    : m_clampActuator(name + " Clamp", false), 
      m_raiseActuator(name + " Raise", false),
      m_clampSensor(name + " Clamped", false), 
      m_limitSensor(factory, name + " Limit", false) {
        factory.add(&m_clampActuator).add(&m_raiseActuator).add(&m_clampSensor);
    }
    
    inline void setRaised(bool enabled) {
        m_raiseActuator.setValue(enabled);
    }
    
    inline void setClamp(bool enabled) {
        m_clampActuator.setValue(enabled);
    }

//    inline bool raisedLoweredLimit() const {
//        return m_limitSensor.getValue();
//    }
    
//    inline bool clamped() const {
//        return m_clampSensor.getValue();
//    }
private:
    Actuator<bool> m_clampActuator;
    Actuator<bool> m_raiseActuator;
    Sensor<bool> m_clampSensor;
    Sensor<bool> m_limitSensor;
    

};
#endif

#endif


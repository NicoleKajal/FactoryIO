#pragma once
#ifndef PARTS_HPP
#define PARTS_HPP

#include <string>
#include "Actuator.hpp"
#include "Factory.hpp"

enum Rotation {
    ROTATION_OFF,
    ROTATION_FORWARD,
    ROTATION_BACKWARD
};

enum Direction {
    DIRECTION_RIGHT,
    DIRECTION_LEFT
};

class OnOffPart {
public:

    OnOffPart(Factory& factory, std::string name, bool value)
    : m_actuator(name, value) {
        factory.add(&m_actuator);
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
    void setRotation(Rotation rotation) {
        
    }
    float getWeight() {
        return 0;    
    }
    
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
 *     Conveying speed: 2.5 m/s
 *     Normally centered
 *     Wheels stroke: 0.003 m
 */
class PopUpWheelSorter {  
public:    
    void setRotation(Rotation rotation){
    }
    void setDirection(Direction direction) {
    }
};

/**
 * Pneumatic pusher sorter equipped with two reed sensors indicating the front and back limits.
 * It also includes a servo valve which can be used to set and measure the rod position.
 *   Details:
 *     Required configuration: Analog
 *     Default speed: 1 m/s
 *     Fast speed: 4 m/s
 *     Stroke: 0.9 m
 */
class Pusher {
public:    
    void setPosition(float poisition) {
        
    }
    float getPosition() {
        return 0; 
    }
};

/**
 * A device used to consecutively position items at the same place by clamping them. Available in
 * two different configurations, left and right.  It is commonly used with Pick and Place Systems.
 * Details:
 *     Vertical stroke: 0.373 m
 *     Clamper stroke: 0.48 m
 */
class PositioningBar {
public:
    void setRaised(bool enabled) {
    }
    
    void clamp(bool enabled) {
        
    }

    bool raisedLoweredLimit() {
        return false;
    }
    
    bool clamped() {
        return false;
    }

};
#endif


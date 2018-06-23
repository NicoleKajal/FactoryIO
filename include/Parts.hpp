#pragma once
#ifndef PARTS_HPP
#define PARTS_HPP

#include <string>
#include "Actuator.hpp"
#include "Factory.hpp"
#include "Sensor.hpp"
#include "Sensors.hpp"

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


/**
 * High speed conveyor scale used for weight control. It measured different weight ranges
 * according to the selected configuration.
 * Details:
 *     Max. conveying speed: 0.6 m/s
 *     Capacities: 20 and 100 Kg
 */
#if 0
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
#endif
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
    : m_speedActuator(factory, name + " Speed", false), 
      m_positionSensor(factory, name + " Position") {
    }
    
    inline void push(float speed) {
        m_speedActuator.setSpeed(speed);
    }
    
    inline float getPosition() const {
        return m_positionSensor.getPosition();
    }
private:
    SpeedActuator  m_speedActuator;
    PositionSensor m_positionSensor;
};


class PickAndPlace {
public:
    PickAndPlace(Factory& factory, std::string name)
    : m_rotateActuator(factory, name + " Rotate", false),
      m_grabActuator(factory, name + " Grab", false),              
      m_xPositionActuator(factory, name + " Set X", 0),
      m_yPositionActuator(factory, name + " Set Y", 0),           
      m_zPositionActuator(factory, name + " Set Z", 0),         
      m_xPositionSensor(factory, name + " X"),            
      m_yPositionSensor(factory, name + " Y"),             
      m_zPositionSensor(factory, name + " Z"),             
      m_itemDetectedSensor(factory, name + " Item Detected"),     
      m_rotateLimitSensor(factory, name + " Rotate Limit") {
    }
    
    inline float getX() const {
        return m_xPositionSensor.getPosition();
    }
    
    inline void setX(float position) {
        m_xPositionActuator.setPosition(position);
    }
    
    inline float getY() const {
        return m_yPositionSensor.getPosition();
    }
    
    inline void setY(float position) {
        m_yPositionActuator.setPosition(position);
    }

    inline float getZ() const {
        return m_zPositionSensor.getPosition();
    }
    
    inline void setZ(float position) {
        m_zPositionActuator.setPosition(position);
    }
    
    inline void setGrab(bool grab) {
        m_grabActuator.setOn(grab);
    }

    inline void setRotate(bool rotate) {
        m_rotateActuator.setOn(rotate);
    }
    
    inline bool itemDetected() const {
        return m_itemDetectedSensor.itemDetected();
    }

    inline bool atRotateLimit() const {
        return m_rotateLimitSensor.atLimit();
    }
    
private:
    OnOffActuator      m_rotateActuator; 
    OnOffActuator      m_grabActuator; 
    PositionActuator   m_xPositionActuator; 
    PositionActuator   m_yPositionActuator; 
    PositionActuator   m_zPositionActuator; 
    PositionSensor     m_xPositionSensor;
    PositionSensor     m_yPositionSensor;
    PositionSensor     m_zPositionSensor;
    ItemDetectedSensor m_itemDetectedSensor;
    LimitSensor        m_rotateLimitSensor;
    
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


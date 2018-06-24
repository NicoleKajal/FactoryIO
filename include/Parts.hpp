#pragma once
#ifndef PARTS_HPP
#define PARTS_HPP

#include <string>
#include "Station.hpp"
#include "Sensors.hpp"
#include "Actuators.hpp"

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


class PickAndPlace {
public:
    PickAndPlace(Station& station, std::string name)
    : m_rotateActuator(station, name + " Rotate"),
      m_grabActuator(station, name + " Grab"),              
      m_xPositionActuator(station, name + " Set X"),
      m_yPositionActuator(station, name + " Set Y"),           
      m_zPositionActuator(station, name + " Set Z"),         
      m_xPositionSensor(station, name + " X"),            
      m_yPositionSensor(station, name + " Y"),             
      m_zPositionSensor(station, name + " Z"),             
      m_itemDetectedSensor(station, name + " Item Detected"),     
      m_rotateLimitSensor(station, name + " Rotate Limit") {
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
    Pusher(Station& station, std::string name)
    : m_speedActuator(station, name + " Speed"), 
      m_positionSensor(station, name + " Position") {
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

/**
 * A 45º double-sided pop-up wheel sorter used to divert items to three different directions via
 * pivoted rollers.
 * Details:
 *     Wheel radius: 0.05 m
 *     Wheels stroke: 0.003 m
 */
class PopUpWheelSorter {  
public:    
    PopUpWheelSorter(Station& station, std::string name)
    : m_rotateForward(station, name + " Forward"), 
      m_wheelsLeft(station, name + " Left"), 
      m_wheelsRight(station, name + " Right") {
    }
    
    void rotateWheels(bool rotate){
        m_rotateForward.setOn(rotate);
    }
        
    void setDirection(Direction direction) {
        switch (direction) {
            case DIRECTION_RIGHT:
                m_wheelsLeft.setOn(false);
                m_wheelsRight.setOn(true);
                break;
            case DIRECTION_LEFT: 
                m_wheelsLeft.setOn(true);
                m_wheelsRight.setOn(false);
                break;
            default: 
                m_wheelsLeft.setOn(false);
                m_wheelsRight.setOn(false);
                break;
        }
    }
private:
    OnOffActuator m_rotateForward;
    OnOffActuator m_wheelsLeft;
    OnOffActuator m_wheelsRight;
};

/**
 * High speed conveyor scale used for weight control. It measured different weight ranges
 * according to the selected configuration.
 * Details:
 *     Max. conveying speed: 0.6 m/s
 *     Capacities: 20 and 100 Kg
 */
class ConveyorScale {
public:
    ConveyorScale(Station& station, std::string name)
    : m_rotateForward(station, name + " Forward"), 
      m_rotateBackward(station, name + " Backward"), 
      m_weightSensor(station, name + " Weight") {
    }
    
    void setRotation(Rotation rotation) {
        switch (rotation) {
            case ROTATION_FORWARD:
                m_rotateForward.setOn(true);
                m_rotateBackward.setOn(false);
                break;
            case ROTATION_BACKWARD:
                m_rotateForward.setOn(false);
                m_rotateBackward.setOn(true);
                break;
            default: 
                m_rotateForward.setOn(false);
                m_rotateBackward.setOn(false);
                break;         
        }
    }

    float getWeight() {
        return m_weightSensor.getWeight();
    }
private:
    OnOffActuator   m_rotateForward;
    OnOffActuator   m_rotateBackward;
    WeightSensor    m_weightSensor;
    
};

#if 0

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
    PivotArmSorter(Station& station, std::string name)
    : m_rotateForward(name + " Forward", false), 
      m_rotateBackward(name + " Backward", false), 
      m_turn(name + " Turn", false) {
        station.add(&m_rotateForward).add(&m_rotateBackward).add(&m_turn);
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
  
#endif




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
    PositioningBar(Station& station, std::string name)
    : m_clampActuator(name + " Clamp", false), 
      m_raiseActuator(name + " Raise", false),
      m_clampSensor(name + " Clamped", false), 
      m_limitSensor(station, name + " Limit", false) {
        station.add(&m_clampActuator).add(&m_raiseActuator).add(&m_clampSensor);
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


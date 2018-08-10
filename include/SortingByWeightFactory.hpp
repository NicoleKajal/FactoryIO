/* 
 * File:   SortingByWeightFactory.hpp
 * Author: Nicole
 *
 * Created on June 24, 2018, 10:04 PM
 */

#ifndef SORTING_BY_WEIGHT_FACTORY_HPP
#define SORTING_BY_WEIGHT_FACTORY_HPP

#include <thread>
#include "Parts.hpp"
#include "Factory.hpp"
#include "Sensors.hpp"
#include "Station.hpp"
#include "Actuators.hpp"

class SortingByWeightFactory {
public:
    SortingByWeightFactory(Factory& factory);
    void sortingManager();
    void waitUntilDone();
    
private:
    Factory&              m_factory;
    Station               m_station;
    Emitter               m_emitter;
    Remover               m_leftRemover;
    Remover               m_rightRemover;
    Remover               m_backRemover;
    DigitalRollerConveyor m_entryConveyor;
    DigitalRollerConveyor m_backConveyor;
    DigitalRollerConveyor m_leftConveyor;
    DigitalRollerConveyor m_rightConveyor;
    ConveyorScale         m_conveyorScale;
    PopUpWheelSorter      m_popUpWheelSorter;
    RetroreflectiveSensor m_entrySensor;
    DiffuseSensor         m_scaleSensor; 
    std::thread          *m_managerThread;
};

#endif 


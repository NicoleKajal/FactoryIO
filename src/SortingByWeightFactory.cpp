#include "SortingByWeightFactory.hpp"
#include <unistd.h>

SortingByWeightFactory::SortingByWeightFactory(Factory& factory) 
: m_factory(factory),
  m_station(factory),
  m_emitter(m_station, "Emitter"),
  m_leftRemover(m_station, "Left Remover"),
  m_rightRemover(m_station, "Right Remover"),
  m_backRemover(m_station, "Back Remover"),
  m_entryConveyor(m_station, "Entry Conveyor"),
  m_backConveyor(m_station, "Back Conveyor"),  
  m_leftConveyor(m_station, "Left Conveyor"),
  m_rightConveyor(m_station, "Right Conveyor"),
  m_conveyorScale(m_station, "Conveyor Scale"),
  m_popUpWheelSorter(m_station, "Wheel Sorter"),
  m_entrySensor(m_station, "Entry Sensor"),
  m_scaleSensor(m_station, "Scale Sensor"),
  m_managerThread(new std::thread(SortingByWeightFactory::sortingManager, this)) {
}     
        
void SortingByWeightFactory::sortingManager() {
  m_backConveyor.setOn(true); 
  m_leftConveyor.setOn(true); 
  m_rightConveyor.setOn(true); 
  m_leftRemover.setOn(true); 
  m_rightRemover.setOn(true); 
  m_backRemover.setOn(true); 
  
  m_emitter.setOn(true);
  m_entryConveyor.setOn(true);
  m_conveyorScale.setRotation(ROTATION_FORWARD);
  m_popUpWheelSorter.rotateWheels(true);
  m_popUpWheelSorter.setDirection(DIRECTION_STRAIGHT);
  m_station.applyChanges();
  sleep(1000);
}

void SortingByWeightFactory::waitUntilDone() {
    m_managerThread->join();
}
       

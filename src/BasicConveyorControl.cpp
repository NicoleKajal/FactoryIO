#include "BasicConveyorControl.hpp"
#include <iostream>

BasicConveyorControl::BasicConveyorControl(Factory& factory, std::string stationPrefix, int32_t maxBoxCount)
: m_factory(factory),
  m_station(factory),
  m_emitter(m_station, stationPrefix + "Emitter"),
  m_remover(m_station, stationPrefix + "Remover"),
  m_entryConveyor(m_station, stationPrefix + "Entry Conveyor"),
  m_exitConveyor(m_station, stationPrefix + "Exit Conveyor"),
  m_entrySensor(m_station, stationPrefix + "At Entry"),
  m_exitSensor(m_station, stationPrefix + "At Exit"),
  m_maxBoxCount(maxBoxCount),
  m_boxCount(0),
  m_entryThread(nullptr),
  m_exitThread(nullptr),
  m_boxCountMutex() {
}

void BasicConveyorControl::start() {
    m_factory.loadSensorValues();
    if ((m_entryThread == nullptr) && (m_entryThread == nullptr)) {
        m_entryThread = new std::thread(&BasicConveyorControl::handleBoxEntry, this);
        m_exitThread = new std::thread(&BasicConveyorControl::handleBoxExit, this);
    }
    else {
        std::cerr << "Nice try buddy!" << std::endl;
    }
}    

void BasicConveyorControl::stop() {
    //TODO(nkb): Add ability to stop the application
}

void BasicConveyorControl::waitUntilDone() {
    m_entryThread->join();
    m_exitThread->join();
}

void BasicConveyorControl::handleBoxEntry() { 
    std::cout << "Handle box entry thread started" << std::endl;
    bool boxDetected = false;
    m_emitter.setOn(true);
    m_remover.setOn(true);
    m_entryConveyor.setOn(true);
    m_exitConveyor.setOn(true);
    m_station.applyChanges();
    for (;;) {
        if (!m_entrySensor.beamDetected() && !boxDetected) {
            boxDetected = true;
        }
        if ((boxDetected) && (m_entrySensor.beamDetected())) {
            boxDetected = false;
            uint32_t currentBoxCount = 0;
            {
                std::lock_guard<std::mutex> scopedLock(m_boxCountMutex);
                currentBoxCount = ++m_boxCount;
            }
            
            if (currentBoxCount >= m_maxBoxCount) {
                std::lock_guard<std::mutex> scopedLock(m_boxCountMutex);  // temp
                m_emitter.setOn(false);
                m_station.applyChanges();
            }
        }
        m_entrySensor.waitForChange();
    }
}

void BasicConveyorControl::handleBoxExit() {
    std::cout << "Handle box exit thread started" << std::endl;

    bool boxDetected = false;
    for (;;) {
        if (!m_exitSensor.beamDetected() && !boxDetected) {
            boxDetected = true;
        }
        if ((boxDetected) && (m_exitSensor.beamDetected())) {
            boxDetected = false;
            {
                 std::lock_guard<std::mutex> scopedLock(m_boxCountMutex);
                --m_boxCount;
            }
            if (!m_emitter.getOn()) {
                m_emitter.setOn(true);
                m_station.applyChanges();
            }
        }    
        m_exitSensor.waitForChange();
    }
}

        


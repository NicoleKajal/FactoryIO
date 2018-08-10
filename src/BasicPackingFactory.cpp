
#include <unistd.h>
#include <chrono>
#include "BasicPackingFactory.hpp"

BasicPackingFactory::BasicPackingFactory(Factory& factory)
: m_factory(factory),
  m_palletStation(factory),  
  m_boxStation(factory),
  m_packingStation(factory),
  m_palletEmitter(m_palletStation, "Pallet Emitter"),
  m_palletRemover(m_palletStation, "Pallet Remover"),
  m_palletEntryConveyor(m_palletStation, "Pallet Entry Conveyor"),
  m_palletExitConveyor(m_palletStation, "Pallet Exit Conveyor"),
  m_palletRollerStop(m_palletStation, "Pallet Roller Stop"),
  m_palletEntrySensor(m_palletStation, "Pallet At Entry"),
  m_palletPackingLocationSensor(m_palletStation, "Pallet At Packing Location"),
  m_boxEmitter(m_boxStation, "Box Emitter"),
  m_boxConveyor(m_boxStation, "Box Conveyor"),
  m_boxEntrySensor(m_boxStation, "Box At Entry"),
  m_boxPackingLocationSensor(m_boxStation, "Box At Packing Location"),
  m_boxStopBlade(m_boxStation, "Box Stop Blade"),
  m_pickAndPlace(m_packingStation, "Pick and Place"),
  m_digitalDisplay(m_packingStation, "Box Count"),
  m_palletManagerThread(nullptr),
  m_boxConveyorManagerThread(nullptr),
  m_packingManagerThread(nullptr),
  m_boxReady(false),
  m_palletReady(false),
  m_palletFull(false) {
}

void BasicPackingFactory::start() {
    m_factory.loadSensorValues();
    m_palletManagerThread = new std::thread(&BasicPackingFactory::palletManager, this);
    m_boxConveyorManagerThread = new std::thread(&BasicPackingFactory::boxConveyorManager, this);
    m_packingManagerThread = new std::thread(&BasicPackingFactory::packingManager, this);
}

class Position {
public:
    float x;
    float y;
    float z;
};



void BasicPackingFactory::packingManager() {
    const float X_PICK_UP = 7.7;
    const float Y_PICK_UP = 5.3;
    const float Z_PICK_UP = 5.5;
    const float Z_TOP = 0.0;
    const float DELTA = 0.3;
    Position boxPosition[6] = {{3.2, 4.2, 10},
                               {3.2, 7.2, 10},
                               {3.2, 4.2, 5.5},
                               {3.2, 7.2, 5.5},
                               {3.2, 4.2, .5},
                               {3.2, 7.2, .5}};
    
    for (;;) {
        for (int32_t boxIndex = 0; boxIndex < 6; ++boxIndex) {
            m_pickAndPlace.setX(X_PICK_UP);
            m_pickAndPlace.setY(Y_PICK_UP);
            m_pickAndPlace.setZ(Z_TOP);
            m_packingStation.applyChanges();

            while (!m_boxReady || !m_palletReady) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            m_pickAndPlace.setZ(Z_PICK_UP);
            m_packingStation.applyChanges();

            while (!m_pickAndPlace.itemDetected()) {
                m_packingStation.waitForSensorChange();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            m_pickAndPlace.setGrab(true);
            m_packingStation.applyChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            m_pickAndPlace.setZ(Z_TOP);
            m_packingStation.applyChanges();
            while (m_pickAndPlace.getZ() > DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            m_pickAndPlace.setX(boxPosition[boxIndex].x);
            m_pickAndPlace.setY(boxPosition[boxIndex].y);
            m_packingStation.applyChanges();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_pickAndPlace.setZ(boxPosition[boxIndex].z);
            m_packingStation.applyChanges();

            while (m_pickAndPlace.getZ() < boxPosition[boxIndex].z - DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            m_pickAndPlace.setGrab(false);
            m_packingStation.applyChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            m_pickAndPlace.setZ(Z_TOP);
            m_digitalDisplay.setNumber(boxIndex + 1);
            m_packingStation.applyChanges();
            m_boxReady = false;
        }
        m_palletFull = true;
        m_palletReady = false;
    }
}

void BasicPackingFactory::boxConveyorManager() {
    
    for (;;) {
        m_boxEmitter.setOn(true);
        m_boxStation.applyChanges();

        // wait for box to arrive
        while (m_boxEntrySensor.beamDetected()) {
            m_boxEntrySensor.waitForChange();
        }

        m_boxEmitter.setOn(false);
        m_boxConveyor.setOn(true);
        m_boxStopBlade.setRaised(true);
        m_boxStation.applyChanges();

        while (m_boxPackingLocationSensor.beamDetected()) {
            m_boxPackingLocationSensor.waitForChange();
        }

        m_boxConveyor.setOn(false);
        m_boxStation.applyChanges();
        sleep(1);
        m_boxStopBlade.setRaised(false);
        m_boxStation.applyChanges();

        m_boxReady = true;

        while (m_boxReady) {
            sleep(1);
        }
    }
    sleep(1000);
}
    


/**
 * The pallet manager performs the following sequence of actions:
 *  1) emits a pallet (turns on pallet and stops emitting once detected)
 *  2) moves the pallet to the packing location
 */
void BasicPackingFactory::palletManager() {

    m_palletExitConveyor.setOn(true);
    m_palletRemover.setOn(true);

    for (;;) {
        m_palletEmitter.setOn(true);
        m_palletStation.applyChanges();

        // wait for pallet to arrive
        while (m_palletEntrySensor.beamDetected()) {
            m_palletEntrySensor.waitForChange();
        }

        m_palletEmitter.setOn(false);
        m_palletRollerStop.setRaised(true);
        m_palletEntryConveyor.setOn(true);
        m_palletStation.applyChanges();

        while (m_palletPackingLocationSensor.beamDetected()) {
            m_palletPackingLocationSensor.waitForChange();
        }
        m_palletEntryConveyor.setOn(false);
        m_palletStation.applyChanges();
        m_palletReady = true;

        while (!m_palletFull) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }

        m_palletRollerStop.setRaised(false);
        m_palletEntryConveyor.setOn(true);
        m_palletStation.applyChanges();
        
        while (!m_palletPackingLocationSensor.beamDetected()) {
            m_palletPackingLocationSensor.waitForChange();
        }
        m_palletFull = false;
        m_palletEntryConveyor.setOn(false);
        m_palletStation.applyChanges();
        
    }
}


void BasicPackingFactory::stop() {

}
void BasicPackingFactory::waitUntilDone() {
    sleep(1000);
}

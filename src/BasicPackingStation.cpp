
#include <unistd.h>
#include <chrono>
#include "BasicPackingStation.hpp"

BasicPackingStation::BasicPackingStation(Factory& factory)
: m_factory(factory),
  m_palletEmitter(factory, "Pallet Emitter", false),
  m_palletRemover(factory, "Pallet Remover", false),
  m_palletEntryConveyor(factory, "Pallet Entry Conveyor", false),
  m_palletExitConveyor(factory, "Pallet Exit Conveyor", false),
  m_palletRollerStop(factory, "Pallet Roller Stop", false),
  m_palletEntrySensor(factory, "Pallet At Entry"),
  m_palletPackingLocationSensor(factory, "Pallet At Packing Location"),
  m_boxEmitter(factory, "Box Emitter", false),
  m_boxConveyor(factory, "Box Conveyor", false),
  m_boxEntrySensor(factory, "Box At Entry"),
  m_boxPackingLocationSensor(factory, "Box At Packing Location"),
  m_boxStopBlade(factory, "Box Stop Blade", false),
  m_pickAndPlace(factory, "Pick and Place"),
  m_palletManagerThread(nullptr),
  m_boxConveyorManagerThread(nullptr),
  m_packingManagerThread(nullptr),
  m_boxReady(false),
  m_palletReady(false),
  m_digitalDisplay(factory, "Box Count" , 0) {
}

void BasicPackingStation::start() {
    m_factory.loadSensorValues();
    m_palletManagerThread = new std::thread(BasicPackingStation::palletManager, this);
    m_boxConveyorManagerThread = new std::thread(BasicPackingStation::boxConveyorManager, this);
    m_packingManagerThread = new std::thread(BasicPackingStation::packingManager, this);
}

class Position {
public:
    float x;
    float y;
    float z;
};



void BasicPackingStation::packingManager() {
    const float X_PICK_UP = 7.7;
    const float Y_PICK_UP = 5.3;
    const float Z_PICK_UP = 5.5;
    const float Z_TOP = 0.0;
    const float DELTA = 0.3;
    Position boxPosition[6] = {{3.2, 4.2, 10},
                               {3.2, 7.0, 10},
                               {3.2, 4.2, 7},
                               {3.2, 7.0, 7},
                               {3.2, 4.2, 3},
                               {3.2, 7.0, 3}};
    
    for (;;) {
        for (int32_t boxIndex = 0; boxIndex < 6; ++boxIndex) {
            m_pickAndPlace.setX(X_PICK_UP);
            m_pickAndPlace.setY(Y_PICK_UP);
            m_pickAndPlace.setZ(Z_TOP);
            m_factory.applyChanges();

            while (!m_boxReady || !m_palletReady) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            m_pickAndPlace.setZ(Z_PICK_UP);
            m_factory.applyChanges();

            while (!m_pickAndPlace.itemDetected()) {
                m_factory.waitForSensorChange();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            m_pickAndPlace.setGrab(true);
            m_factory.applyChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            m_pickAndPlace.setZ(Z_TOP);
            m_factory.applyChanges();
            while (m_pickAndPlace.getZ() > DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            m_pickAndPlace.setX(boxPosition[boxIndex].x);
            m_pickAndPlace.setY(boxPosition[boxIndex].y);
            m_factory.applyChanges();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_pickAndPlace.setZ(boxPosition[boxIndex].z);
            m_factory.applyChanges();

            while (m_pickAndPlace.getZ() < boxPosition[boxIndex].z - DELTA) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            m_pickAndPlace.setGrab(false);
            m_factory.applyChanges();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            m_pickAndPlace.setZ(Z_TOP);
            m_digitalDisplay.setNumber((float)(boxIndex + 1));
            m_factory.applyChanges();
            m_boxReady = false;
        }
    }
}

void BasicPackingStation::boxConveyorManager() {
    
    for (;;) {
        m_boxEmitter.setOn(true);
        m_factory.applyChanges();

        // wait for box to arrive
        while (m_boxEntrySensor.beamDetected()) {
            m_boxEntrySensor.waitForChange();
        }

        m_boxEmitter.setOn(false);
        m_boxConveyor.setOn(true);
        m_boxStopBlade.setRaised(true);
        m_factory.applyChanges();

        while (m_boxPackingLocationSensor.beamDetected()) {
            m_boxPackingLocationSensor.waitForChange();
        }

        m_boxConveyor.setOn(false);
        m_factory.applyChanges();
        sleep(1);
        m_boxStopBlade.setRaised(false);
        m_factory.applyChanges();

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
void BasicPackingStation::palletManager() {
    m_palletEmitter.setOn(true);
    m_palletRemover.setOn(true);
    m_factory.applyChanges();
    
    // wait for pallet to arrive
    while (m_palletEntrySensor.beamDetected()) {
        m_palletEntrySensor.waitForChange();
    }
    
    m_palletEmitter.setOn(false);
    m_palletRollerStop.setRaised(true);
    m_palletEntryConveyor.setOn(true);
    m_factory.applyChanges();
    
    while (m_palletPackingLocationSensor.beamDetected()) {
        m_palletPackingLocationSensor.waitForChange();
    }
    m_palletEntryConveyor.setOn(false);
    m_factory.applyChanges();
    m_palletReady = true;
    
    sleep(1000);
}


void BasicPackingStation::stop() {

}
void BasicPackingStation::waitUntilDone() {
    sleep(1000);
}

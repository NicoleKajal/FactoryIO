/* 
 * File:   Main.cpp
 * Author: Nicole
 *
 * Created on May 30, 2018, 3:08 PM
 */

#include <iostream>
#include "Factory.hpp"
#include "BasicPackingFactory.hpp"
#include "BasicConveyorControl.hpp"
#include "SortingByWeightFactory.hpp"

void basicConveyorControlDemo() {
    Factory factory;
    factory.start();
    BasicConveyorControl basicConveyorControl1(factory, "Station 1 ", 3);
//    BasicConveyorControl basicConveyorControl2(factory, "Station 2 ", 3);
    basicConveyorControl1.start();
//    basicConveyorControl2.start();
    basicConveyorControl1.waitUntilDone();
//    basicConveyorControl2.waitUntilDone();
}

void basicPackingStationDemo() {
    Factory factory;
    factory.start();
    BasicPackingFactory basicPackingStation(factory);
    basicPackingStation.start();
    basicPackingStation.waitUntilDone();
}

void weightSortingDemo() {
    Factory factory;
    factory.start();
    SortingByWeightFactory sortingByWeightFactory(factory);
    sortingByWeightFactory.waitUntilDone();
}
#include <iostream>

int main() {
#ifdef KAJU
    std::cout << "hey kahu" << std::endl;
#endif
    basicConveyorControlDemo();
    return 0;    
}    

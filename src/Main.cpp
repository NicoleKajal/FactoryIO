/* 
 * File:   Main.cpp
 * Author: Nicole
 *
 * Created on May 30, 2018, 3:08 PM
 */

#include "Factory.hpp"
#include "BasicConveyorControl.hpp"
#include <iostream>

int main() {
    Factory factory;
    std::cout << "Waiting for connection to factory..." << std::endl;
    factory.start();
    std::cout << "Connection established!" << std::endl;
    BasicConveyorControl basicConveyorControl(factory, 3);
    basicConveyorControl.start();
    std::cout << "Starting basic conveyor control" << std::endl;
    basicConveyorControl.waitUntilDone();
    return 0;    
}    

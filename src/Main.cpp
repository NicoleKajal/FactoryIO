/* 
 * File:   Main.cpp
 * Author: Nicole
 *
 * Created on May 30, 2018, 3:08 PM
 */

#include <unistd.h>
#include "Factory.hpp"
#include "Parts.hpp"
#include "Sensors.hpp"
#include <iostream>

void bufferConveyor() {
    Factory factory;
    Emitter emitter(factory, "Emitter", false);
    Remover remover(factory, "Remover", false);
    
    factory.start();
    sleep(1000);
}

int main() {
    bufferConveyor();
    return 0;
}

#if 0
//    Factory factory;
    //Emitter emitter(factory, "Emitter", false);
    //Remover remover(factory, "Remover", false);
    //BeltConveyor beltConveyor(factory, "Belt Conveyor", 0.0);
  //  PivotArmSorter pivotArmSorter(factory, "Pivot Arm Sorter");
    //DiffuseSensor backDiffuseSensor(factory, "Back Diffuse Sensor");
//    factory.start();

    for (;;) {
        //emitter.setOn(true);
        //remover.setOn(true);
      //  pivotArmSorter.turn(true);
     //   factory.applyChanges();
     //   sleep(4);

      //  pivotArmSorter.turn(false);
     //   factory.applyChanges();
    //    sleep(4);

    //    pivotArmSorter.setRotation(ROTATION_FORWARD);
   //     factory.applyChanges();
  //      sleep(4);

  //      pivotArmSorter.setRotation(ROTATION_BACKWARD);
   //     factory.applyChanges();
  //      sleep(4);

  //      pivotArmSorter.setRotation(ROTATION_OFF);
  //      factory.applyChanges();
   //     sleep(4);
        
        
        //while (!backDiffuseSensor.itemDetected()) {
       //     sleep(1);
       // }
        //beltConveyor.setSpeed(4.0);
        //emitter.setOn(false);
        factory.applyChanges();
        sleep(500);
    }

    return 0;
}
#endif
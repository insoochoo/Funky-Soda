#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "truck.h"
#include "vendingMachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
    prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour), cargo(new unsigned int [4]), index(0){};


void Truck::main(){
    // print start

    while(true){
        VendingMachine **machineList = nameServer.getMachineList();

        yield(mprng(1,10)); // tom hortons

        try {
            plant.getShipment(cargo);
            //print picked up shipment

            for(unsigned int i = 0; i < numVendingMachines; i++){
                int sum = 0;
                for(int a = 0; a < 4; a++){
                    sum += cargo[a];
                }
                if(sum == 0){
                    //print unsuccessfully filled vending machine
                    break; // cargo is empty
                }

                //begin delivery to vending machine
                index = (index+1) % numVendingMachines;
                unsigned int *inventory = machineList[index]->inventory();
                // stock vending machine, remove from cargo
                for(unsigned int j = 0; j < 4; j++){
                    int needToFill = maxStockPerFlavour - inventory[j];
                    if(cargo[j] >= needToFill){
                        cargo[j] -= needToFill;
                        inventory[j] += needToFill;
                    } else { // cargo has few items left
                        inventory[j] += cargo[j];
                        cargo[j] = 0;
                    }
                }
                machineList[index]->restocked(); // done restocking
                // print end delivery to vending machine
            }

        } catch (BottlingPlant::Shutdown){
            break;
        }
    }

    // print finished
}

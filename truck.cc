#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "truck.h"
#include "vendingMachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

Truck::~Truck() {
    delete cargo;
}

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
    prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour), cargo(new unsigned int [4]), index(0){};

int Truck::getSum(){
    // return the sum of all items in the cargo
    int sum = 0;
    for(int a = 0; a < 4; a++){
        sum += cargo[a];
    }
    return sum;
}

void Truck::main(){
    // print start
    prt.print(Printer::Truck, 'S');

    int sum = 0;

    while(true){
        // receive vending machine list to distribute the items to
        VendingMachine **machineList = nameServer.getMachineList();

        yield(mprng(1,10)); // "tom hortons" break

        try {
            // receive shipments from the bottling plant
            plant.getShipment(cargo);
            sum = Truck::getSum();
            //print picked up shipment: total amount a of all sodas in the shipment
            prt.print(Printer::Truck, 'P', sum);

            // visit all vending machines
            for(unsigned int i = 0; i < numVendingMachines; i++){
                //begin delivery to vending machine: vending machine v, total amount remaining r in the shipment
                prt.print(Printer::Truck, 'd', machineList[index]->getId(), sum);

                // get inventories for a vending machine
                unsigned int *inventory = machineList[index]->inventory();

                // stock vending machine from cargo
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

                // calculate how many bottles were unfilled
                unsigned int unfilled = 0;
                for(unsigned int m = 0; m < 4; m++){
                    unfilled = unfilled + (maxStockPerFlavour - inventory[m]);
                }

                if(unfilled != 0){
                    prt.print(Printer::Truck, 'U', machineList[index]->getId(), unfilled);
                }

                machineList[index]->restocked(); // done restocking

                // print end delivery to vending machine: vending machine v, total amount remaining r in the shipment
                sum = Truck::getSum();
                prt.print(Printer::Truck, 'D', machineList[index]->getId(), sum);
                index = (index+1) % numVendingMachines;

                // if truck is empty, return back
                if(sum == 0){
                    break; // no more sodas left
                }
            }
        } catch (BottlingPlant::Shutdown){
            break;
        }
    }

    // print finished
    prt.print(Printer::Truck, 'F');
}

#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
               unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
               unsigned int timeBetweenShipments) :
  prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments), producedCargo(new unsigned int[5]){

    shutdown = false;
    for (unsigned int i = 0; i < 4; i++) {
        producedCargo[i] = 0;
    }
};

void BottlingPlant::getShipment(unsigned int cargo[]){
    if (shutdown) {
        throw Shutdown;
    }

    for (unsigned int i = 0; i < 4; i++) {
        cargo[i] = produceCargo[i];
    }
}

void BottlingPlant::main(){
    //print started
    Truck truck(prt, nameServer, this, numVendingMachines, maxStockPerFlavour);

    while (true){
        // make items
        for (unsigned int i = 0; i < 4; i++) {
            produceCargo[i] = mprng(0, maxShippedPerFlavour);
        }

        //print # of bottles generated

        _Accept(~BottlingPlant){
            shutdown = true;
            _Accept(BottlingPlant::getShipment);
            break;
        } or _Accept(BottlingPlant::getShipment){
            yield(TimeBetweenShipments);
        }
    }
    //print ended
}

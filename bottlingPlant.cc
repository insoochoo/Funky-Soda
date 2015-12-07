#include "printer.h"
#include "nameServer.h"
#include "truck.h"
#include "bottlingPlant.h"
#include "MPRNG.h"

extern MPRNG mprng;

BottlingPlant::~BottlingPlant(){
    delete[] producedCargo;
}

BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
               unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
               unsigned int timeBetweenShipments) :
  prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments), producedCargo(new unsigned int[5]){

    // set up initial variables
    shutdown = false;

    // set cargo to be empty
    for (unsigned int i = 0; i < 4; i++) {
        producedCargo[i] = 0;
    }
};

void BottlingPlant::getShipment(unsigned int cargo[]){
    // if plant has shutdown, getShipment should fail
    if (shutdown) {
        uRendezvousAcceptor();
        throw BottlingPlant::Shutdown();
    }

    prt.print(Printer::BottlingPlant, 'P');

    // load items into truck's cargo
    for (unsigned int i = 0; i < 4; i++) {
        cargo[i] = producedCargo[i];
    }
}

void BottlingPlant::main(){
    prt.print(Printer::BottlingPlant, 'S');
    Truck *truck = new Truck(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);

    // keep producing items until bottlingPlant shuts down
    while (true){
        int count = 0;
        // produce items
        for (unsigned int i = 0; i < 4; i++) {
            producedCargo[i] = mprng(0, maxShippedPerFlavour);
            count += producedCargo[i];
        }

        //print # of bottles generated
        prt.print(Printer::BottlingPlant, 'G', count);

        _Accept(~BottlingPlant){
            shutdown = true;
            _Accept(BottlingPlant::getShipment);
            break;
        } or _Accept(BottlingPlant::getShipment){
            yield(timeBetweenShipments);
        }
    }

    delete truck;
    prt.print(Printer::BottlingPlant, 'F');
}

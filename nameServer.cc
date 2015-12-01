#include "printer.h"
#include "vendingMachine.h"
#include "nameServer.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
      : prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents), vendingMachineCount(0),
        studentCountList(new unsigned int[numStudents]), vendingMachineList(new VendingMachine* [numVendingMachines]) {};

void NameServer::VMregister(VendingMachine *vendingmachine){
    // vending machine v registering
    prt.print(Printer::NameServer, 'R', vendingmachine->getId());

    vendingMachineList[vendingMachineCount] = vendingmachine;
    vendingMachineCount++;
}

VendingMachine *NameServer::getMachine(unsigned int id){
    unsigned int index = studentCountList[id];
    studentCountList[id] = (studentCountList[id] + 1) % numVendingMachines;

    // student s requesting vending machine, new vending machine v
    prt.print(Printer::NameServer, 'N', id, vendingMachineList[index]->getId());

    return vendingMachineList[index];
}

VendingMachine **NameServer::getMachineList(){
    return vendingMachineList;
}

void NameServer::main(){
    // print started
    prt.print(Printer::NameServer, 'S');

    for(unsigned int i = 0; i < numStudents; i++){
        studentCountList[i] = i % numVendingMachines;
    }

    for(unsigned int i = 0; i < numVendingMachines; i++){
        _Accept(VMregister);
    }

    while(true){
        _Accept(~NameServer){
            break;
        } or _Accept(getMachine){} or _Accept(getMachineList){}
    }

    //print finished
    prt.print(Printer::NameServer, 'F');
}


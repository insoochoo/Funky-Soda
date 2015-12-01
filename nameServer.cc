#include "printer.h"
#include "nameServer.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents )
      : prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents), vendingMachineCount(0),
        studentCountList(new unsigned int[numStudents]), vendingMachineList(new VendingMachine* [numVendingMachines]) {};

void NameServer::VMregister(VendingMachine *vendingmachine){
    vendingMachineList[vendingMachineCount] = vendingmachine;
    vendingMachineCount++;
}

VendingMachine *NameServer::getMachine(unsigned int id){
    unsigned int index = studentCountList[id];
    studentCountList[id] = (studentCountList[id] + 1) % numVendingMachines;
    return vendingMachineList[index];
}

VendingMachine **NameServer::getMachineList(){
    return vendingMachineList;
}

void NameServer::main(){
    // print started
    for(unsigned int i = 0; i < numStudents; i++){
        studentCountList[i] = i % numVendingMachines;
    }

    for(unsigned int i = 0; i < numVendingMachines; i++){
        _Accept(VMregister);
    }

    _Accept(~NameServer);

    //print finished
}

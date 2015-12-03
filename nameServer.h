#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "vendingMachine.h"

_Task NameServer {
    Printer &prt;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    unsigned int *studentCountList;
    int vendingMachineCount;
    VendingMachine **vendingMachineList;

    void main();
  public:
    ~NameServer();
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif

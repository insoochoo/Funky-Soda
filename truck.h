#include "bottlingPlant.h"

_Task Truck {
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int *cargo;
    unsigned int index;

    void main();
  public:
    Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

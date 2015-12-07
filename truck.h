#include "bottlingPlant.h"

_Task Truck {
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int *cargo;
    unsigned int index; // represents the id of next vending machine to go to

    int getSum();
    void main();
  public:
    ~Truck();
    Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

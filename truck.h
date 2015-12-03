#include "bottlingPlant.h"

_Task Truck {
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int *cargo;
    unsigned int index;

    int getSum();
    void main();
  public:
    ~Truck();
    Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

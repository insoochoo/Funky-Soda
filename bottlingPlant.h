#ifndef __BOTTLINGPLANT__
#define __BOTTLINGPLANT__

_Task BottlingPlant {
    Printer &prt;
    NameServer &nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    unsigned int *producedCargo;
    bool shutdown; // flag to check if plant has shut down
    void main();
  public:
    _Event Shutdown{};                    // shutdown plant
    ~BottlingPlant();
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments);
    void getShipment(unsigned int cargo[]);
};


#endif

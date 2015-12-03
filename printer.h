#ifndef __PRINTER_H__
#define __PRINTER_H__

_Monitor Printer {
    unsigned int totalCount;
    unsigned int numStudents;
    unsigned int numVendingMachines;
    unsigned int numCouriers;

    int *bufferStates;
    int *bufferSecond;
    int *bufferThird;

    enum State {
        Start = 'S',
        Deposit = 'D',
        Finish = 'F',
        WorkCompleted = 'W',
        CreateCompleted = 'C',
        TransferCompleted = 'T',
        RegisterVM = 'R',
        NewVM = 'N',
        PickupShipment = 'P',
        BeginDelivery = 'd',
        UnsuccessfulFill = 'U',
        EndDelivery = 'D',
        GenerateSoda = 'G',
        SelectVM = 'V',
        GiftedSoda = 'G',
        BoughtSoda = 'B',
        WATCardLost = 'L',
        StartReloading = 'r',
        CompleteReloading = 'R',
        FundsTransfer = 't',
        CompleteTransfer = 'T'
    };

    void printColumn();
    void finalFlush(unsigned int lid);
  public:
    enum Kind {Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier};
    ~Printer();
    Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers);
    void print(Kind kind, char state);
    void print(Kind kind, char state, int value1);
    void print(Kind kind, char state, int value1, int value2);
    void print(Kind kind, unsigned int lid, char state);
    void print(Kind kind, unsigned int lid, char state, int value1);
    void print(Kind kind, unsigned int lid, char state, int value1, int value2);

  private:
    unsigned int getID(Kind kind, unsigned int lid);
};

#endif

#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "watCard.h"
_Task NameServer;

_Task VendingMachine {
    Printer &prt;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;
    unsigned int *stock;

    void main();
  public:
    enum Flavours{
        Blues,
        BlackCherry,
        CreamSoda,
        RootBeer
    };                 // flavours of soda (YOU DEFINE)
    _Event Funds{};                       // insufficient funds
    _Event Stock{};                       // out of stock for particular flavour
    ~VendingMachine();
    VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour);
    void buy(Flavours flavour, WATCard &card);
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif // __VENDINGMACHINE_H__

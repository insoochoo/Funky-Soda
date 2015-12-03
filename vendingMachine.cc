#include "printer.h"
#include "watCard.h"
#include "nameServer.h"
#include "vendingMachine.h"

VendingMachine::~VendingMachine() {
    delete stock;
}

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour):
    prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour), stock(new unsigned int [4]){
        nameServer.VMregister(this);
        for(int i = 0; i < 4; i++){
            stock[i] = 0;
        }
    };

void VendingMachine::buy(Flavours flavour, WATCard &card){
    if (card.getBalance() < sodaCost){
        uRendezvousAcceptor();
        throw Funds();
    }

    if(stock[flavour] == 0){
        uRendezvousAcceptor();
        throw Stock();
    }

    stock[flavour]--;
    card.withdraw(sodaCost);
    //print bought: flavour f of soda purchased, amount remaining r of this flavour
    prt.print(Printer::Vending, id, 'B', flavour, stock[flavour]);
}

unsigned int *VendingMachine::inventory(){
    //print start reloading by truck
    prt.print(Printer::Vending, id, 'r');
    return stock;
}

void VendingMachine::restocked(){
    //print complete reloading by truck
    prt.print(Printer::Vending, id, 'R');
}

unsigned int VendingMachine::cost(){
    return sodaCost;
}

unsigned int VendingMachine::getId(){
    return id;
}

void VendingMachine::main(){
    //print started: cost c per bottle
    prt.print(Printer::Vending, id, 'S', sodaCost);

    while (true) {
        _Accept(~VendingMachine){
            break;
        } or _Accept(inventory){    // don't accept buy while restocking
            _Accept(restocked);
        } or _Accept(buy);
    }
    //print ended
    prt.print(Printer::Vending, id, 'F');
}

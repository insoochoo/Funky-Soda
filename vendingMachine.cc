#include "printer.h"
#include "watCard.h"
#include "nameServer.h"
#include "vendingMachine.h"

VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour):
    prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour), stock(new unsigned int [4]){
        nameServer.VMregister(this);
        for(int i = 0; i < 4; i++){
            stock[i] = 0;
        }
    };

void VendingMachine::buy(Flavours flavour, WATCard &card){
    if (card.getBalance() < sodaCost){
        throw Funds();
    }

    if(stock[flavour] == 0) {
        throw Stock();
    }

    stock[flavour]--;
    card.withdraw(sodaCost);
    //print flavor student
}

unsigned int *VendingMachine::inventory(){
    return stock;
}

void VendingMachine::restocked(){
    //print complete reloading by truck
}

unsigned int VendingMachine::cost(){
    return sodaCost;
}

unsigned int VendingMachine::getId(){
    return id;
}

void VendingMachine::main(){
    //print started

    while (true) {
        _Accept(~VendingMachine){
            break;
        } or _Accept(inventory){    // don't accept buy while restocking
            _Accept(restocked);
        } or _Accept(buy);
    }
    //print ended
}

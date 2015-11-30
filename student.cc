#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "vendingMachine.h"
#include "student.h"


Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases)
             : prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases), boughtCount(0) {};

Student::main(){
    unsigned int numberOfBottles = mprng(1, maxPurchases);
    int favoriteFlavour = mprng(0, 3);
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();

    VendingMachine* vendingMachine = nameServer.getMachine(id);
    bool lost = false;

    while(true){
        if (boughtCount == numberOfBottles){
            break;
        }

        try{
            if(!lost){
                yield(1,10);
            }
            lost = false;

            _Select(watCard){
                vendingMachine->buy(favoriteFlavour, watCard);
            } or _Select(giftCard){
                vendingMachine->buy(favoriteFlavour, giftCard);
                giftCard.reset();
            } // blocks waiting for money to be transferred

            boughtCount++;
        } _Catch(VendingMachine::Funds){
            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard);
        } _Catch(VendingMachine::Stock){
            vendingMachine = nameServer.getMachine(id);
        } _Catch(WATCardOffice::Lost){
            //print lost watcard
            watCard = cardOffice.create(id, 5);
            lost = true;
        }
    }
    //print finish
}

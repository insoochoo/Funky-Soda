#include "printer.h"
#include "nameServer.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "vendingMachine.h"
#include "student.h"
#include "MPRNG.h"

extern MPRNG mprng;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff, unsigned int id, unsigned int maxPurchases)
             : prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases), boughtCount(0) {};

void Student::main(){
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
                yield(mprng(1,10));
            }
            lost = false;

            _Select(watCard){
                vendingMachine->buy(VendingMachine::Flavours(favoriteFlavour), *watCard);
            } or _Select(giftCard){
                vendingMachine->buy(VendingMachine::Flavours(favoriteFlavour), *giftCard);
                giftCard.reset();
            } // blocks waiting for money to be transferred

            boughtCount++;
        } catch(VendingMachine::Funds){
            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard);
        } catch(VendingMachine::Stock){
            vendingMachine = nameServer.getMachine(id);
        } catch(WATCardOffice::Lost){
            //print lost watcard
            watCard = cardOffice.create(id, 5);
            lost = true;
        }
    }
    //print finish
}

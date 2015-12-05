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

    // favourite soda f , number of bottles b to purchase
    prt.print(Printer::Student, id, 'S', favoriteFlavour, numberOfBottles);

    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();


    VendingMachine* vendingMachine = nameServer.getMachine(id);

    // vending machine v selected
    prt.print(Printer::Student, id, 'V', vendingMachine->getId());

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
                // watcard balance b after purchase
                prt.print(Printer::Student, id, 'B', watCard()->getBalance());
            } or _Select(giftCard){
                WATCard *container = giftCard;
                vendingMachine->buy(VendingMachine::Flavours(favoriteFlavour), *giftCard);
                // giftcard balance b
                prt.print(Printer::Student, id, 'G', giftCard()->getBalance());
                giftCard.reset();
                delete container;
            } // blocks waiting for money to be transferred

            boughtCount++;
        } catch(VendingMachine::Funds){
            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard);
        } catch(VendingMachine::Stock){
            vendingMachine = nameServer.getMachine(id);
        } catch(WATCardOffice::Lost){
            //print lost watcard
            prt.print(Printer::Student, id, 'L');

            watCard = cardOffice.create(id, 5);
            lost = true;
        }
    }

    WATCard* wat = watCard();
    delete wat;

    //print finish
    prt.print(Printer::Student, id, 'F');
}

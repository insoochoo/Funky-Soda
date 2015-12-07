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
    // initialize # of bottles that a student will purchase
    unsigned int numberOfBottles = mprng(1, maxPurchases);

    // decide on the flavour
    int favoriteFlavour = mprng(0, 3);

    // favourite soda f , number of bottles b to purchase
    prt.print(Printer::Student, id, 'S', favoriteFlavour, numberOfBottles);

    // receive a watcard and a giftcard
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();

    // receive the nearest vendingMachine
    VendingMachine* vendingMachine = nameServer.getMachine(id);

    // vending machine v selected
    prt.print(Printer::Student, id, 'V', vendingMachine->getId());

    // flag to check if a watcard is lost
    bool lost = false;

    while(true){
        // if student has bought enough bottles, terminate
        if (boughtCount == numberOfBottles){
            break;
        }

        try{
            if(!lost){
                yield(mprng(1,10));
            }
            lost = false;

            // select watCard or giftCard depending on what's ready
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
            // if not enough fund, transfer more from the office
            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard);
        } catch(VendingMachine::Stock){
            // if out of stock, choose a different vending machine
            vendingMachine = nameServer.getMachine(id);
        } catch(WATCardOffice::Lost){
            //print lost watcard
            prt.print(Printer::Student, id, 'L');

            // if watcard is lost, create a new one
            watCard = cardOffice.create(id, 5);
            lost = true;
        }
    }

    // delete the last watcard student was using
    // if lost, no need to delete, since the watcard is already deleted
    try{
        WATCard* wat = watCard();
        delete wat;
    } catch(WATCardOffice::Lost){}

    //print finish
    prt.print(Printer::Student, id, 'F');
}

#include "student.h"

Student::main() {
    unsigned int numberOfBottles = mprng(1, maxPurchases);
    int favoriteFlavour = mprng(0, 3);
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    // TODO: create gift card
    VendingMachine* vendingMachine = nameServer.getMachine(id);

    for (unsigned int i = 0; i < numberOfBottles; i++) {
        yield(1,10);
        // blocks waiting for money to be transferred
        vendingMachine.buy(favoriteFlavour, watCard);

    }

}

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases )
             : prt(prt), nameServer(nameServer), cardOffice(cardOffice),
                groupoff(groupoff), id(id), maxPurchases(maxPurchases) {};



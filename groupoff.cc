#include "printer.h"
#include "watCard.h"
#include "groupoff.h"
#include "MPRNG.h"

extern MPRNG mprng;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay) :
  prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), fwatcards(new WATCard::FWATCard [numStudents]), index(0) {};

WATCard::FWATCard Groupoff::giftCard(){
    return fwatcards[index++];
}

void Groupoff::main() {
    //print start
    prt.print(Printer::Groupoff, 'S');

    for (unsigned int i = 0; i < numStudents; i++){
        _Accept(Groupoff::giftCard);
    }

    int counter = 0;
    while(true) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            if(counter == numStudents){
                break;
            }
            int randomize = mprng(0,numStudents-1);
            // TODO: look over this
            if(!fwatcards[randomize].available()){
                yield(groupoffDelay);
                WATCard *watCard = new WATCard();
                watCard->deposit(sodaCost);

                // print gift amount deposited
                prt.print(Printer::Groupoff, 'D', sodaCost);

                //fwatcards[randomize].reset();
                fwatcards[randomize].delivery(watCard);
                counter++;
            }
        }
    }

    //print finished
    prt.print(Printer::Groupoff, 'F');
}

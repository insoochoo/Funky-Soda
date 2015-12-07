#include "printer.h"
#include "watCard.h"
#include "groupoff.h"
#include "MPRNG.h"

extern MPRNG mprng;

Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay) :
  prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay),
  fwatcards(new WATCard::FWATCard [numStudents]), index(0), available(new bool[numStudents]){
    // available keeps track of which students have received a giftcard
    for(unsigned int i = 0; i < numStudents; i++){
        available[i] = true;
    }
};

WATCard::FWATCard Groupoff::giftCard(){
    return fwatcards[index++];
}

Groupoff::~Groupoff(){
    for (unsigned int i = 0; i < numStudents; i++){
        // if watcard has not been deleted, delete it
        if(fwatcards[i].available()) {
            delete fwatcards[i]();
        }
    }
    delete[] fwatcards;
    delete[] available;
}

void Groupoff::main() {
    //print start
    prt.print(Printer::Groupoff, 'S');

    // let all students receive a future-watcard
    for (unsigned int i = 0; i < numStudents; i++){
        _Accept(Groupoff::giftCard);
    }

    int counter = 0;
    while(true) {
        _Accept(~Groupoff) {
            break;
        } _Else {
            // once all students have received a giftcard, terminate
            if(counter == numStudents){
                break;
            }
            // randomly sent giftcards to students
            int randomize = mprng(0,numStudents-1);
            // if student has not been gifted already,
            if(available[randomize]){
                yield(groupoffDelay);
                WATCard *watCard = new WATCard();
                watCard->deposit(sodaCost);

                // print gift amount deposited
                prt.print(Printer::Groupoff, 'D', sodaCost);

                //fwatcards[randomize].reset();
                fwatcards[randomize].delivery(watCard);
                counter++;

                available[randomize] = false;
            }
        }
    }

    //print finished
    prt.print(Printer::Groupoff, 'F');
}

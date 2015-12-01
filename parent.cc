#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Parent::main(){
    //print start

    while(true){
        _Accept(~Parent){
            break;
        } _Else {
            yield(parentalDelay);
            unsigned int amount = mprng(1,3);
            unsigned int id = mprng(0,numStudents-1);
            // print deposit (student id), (amount)
            bank.deposit(id, amount);
        }
    }

    //print finished
}

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay):
    prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay){};

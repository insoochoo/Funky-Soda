#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "MPRNG.h"

extern MPRNG mprng;

void Parent::main(){
    //print start
    prt.print(Printer::Parent, 'S');

    while(true){
        _Accept(~Parent){
            break;
        } _Else {
            yield(parentalDelay);

            // randomly assign money and student
            unsigned int amount = mprng(1,3);
            unsigned int id = mprng(0,numStudents-1);

            // student s receiving gift, amount of gift g
            prt.print(Printer::Parent, 'D', id, amount);

            // deposit money to the student's bank account
            bank.deposit(id, amount);
        }
    }

    //print finished
    prt.print(Printer::Parent, 'F');
}

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay):
    prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay){};

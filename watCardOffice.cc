#include <iostream>
#include "printer.h"
#include "bank.h"
#include "watCardOffice.h"
#include "MPRNG.h"

extern MPRNG mprng;

WATCardOffice::~WATCardOffice(){
    //print finished
    prt.print(Printer::WATCardOffice, 'F');

    for(unsigned int i = 0; i < numCouriers; i++){
        delete couriers[i];
    }

    delete couriers;
}

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) : prt(prt), bank(bank), numCouriers(numCouriers), finished(false){
    couriers = new Courier* [numCouriers];
    for (unsigned int i = 0; i < numCouriers; i++) {
        couriers[i] = new Courier(i, bank, prt, *this);
    }
};

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount){
    WATCard *watCard = new WATCard();

    Args args = {sid, amount, watCard};
    Job *job = new Job(args);

    jobs.push(job);
    //tell blocked tasks to wake up
    jobLock.signal();

    // student s transfer amount a, call completed
    prt.print(Printer::WATCardOffice, 'C', sid, amount);

    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card){
    Args args = {sid, amount, card};
    Job *job = new Job(args);

    jobs.push(job);
    //tell blocked tasks to wake up
    jobLock.signal();

    // print transfer completed: student s, transfer amount a
    prt.print(Printer::WATCardOffice, 'T', sid, amount);
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork(){
    if(jobs.empty()){
        jobLock.wait();
    }

    if(finished){
        return NULL;
    }

    WATCardOffice::Job *temp = jobs.front();
    jobs.pop();

    // print request work complete
    prt.print(Printer::WATCardOffice, 'W');
    return temp;
}

void WATCardOffice::main(){
    //print starting
    prt.print(Printer::WATCardOffice, 'S');
    while(true){
        _Accept(~WATCardOffice){
            finished = true;

            for(unsigned int i = 0; i < numCouriers; i++){
                jobLock.signal();
            }
            break;
        } or _Accept(WATCardOffice::create){} or _Accept(WATCardOffice::transfer){} or _Accept(WATCardOffice::requestWork){}
    }

}

WATCardOffice::Courier::Courier(unsigned int id, Bank &bank, Printer &printer, WATCardOffice &cardOffice)
  : id(id), bank(bank), printer(printer), cardOffice(cardOffice){}


void WATCardOffice::Courier::main(){
    //print start
    printer.print(Printer::Courier, id, 'S');

    while(true){
        _Accept(~Courier){
            break;
        } _Else {
            Job *currJob = cardOffice.requestWork();
            if(currJob == NULL){ //error check
                break;
            }
            // print transfer start: student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 't', currJob->args.id, currJob->args.amount);

            // update
            bank.withdraw(currJob->args.id, currJob->args.amount);
            currJob->args.watCard->deposit(currJob->args.amount);

            // print transfer complete: student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 'T', currJob->args.id, currJob->args.amount);

            if(mprng(1,6) == 1){ //lose watCard :(
                //currJob->result.reset();
                currJob->result.exception(new WATCardOffice::Lost);
                delete currJob->args.watCard;
            }
            else{
                //currJob->result.reset();
                currJob->result.delivery(currJob->args.watCard);
                //print something
            }
            // print transfer finished
            delete currJob;
        }
    }
    //print finished
    printer.print(Printer::Courier, id, 'F');
}

#include <iostream>
#include "printer.h"
#include "bank.h"
#include "watCardOffice.h"
#include "MPRNG.h"

extern MPRNG mprng;

WATCardOffice::~WATCardOffice(){
    // delete all courier tasks allocated
    for(unsigned int i = 0; i < numCouriers; i++){
        delete couriers[i];
    }

    delete[] couriers;
    //print finished
    prt.print(Printer::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) : prt(prt), bank(bank), numCouriers(numCouriers), finished(false){
    // initialize courier tasks
    couriers = new Courier* [numCouriers];
    for (unsigned int i = 0; i < numCouriers; i++) {
        couriers[i] = new Courier(i, bank, prt, *this);
    }
};

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount){
    // create a watcard
    WATCard *watCard = new WATCard();

    Args args = {sid, amount, watCard};
    Job *job = new Job(args);

    // create a job and push it to the job queue
    jobs.push(job);
    // signal blocked tasks to wake up
    jobLock.signal();

    // student s transfer amount a, call completed
    prt.print(Printer::WATCardOffice, 'C', sid, amount);

    // return the future-watcard
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card){
    Args args = {sid, amount, card};
    Job *job = new Job(args);

    // create a job and push it to the job queue
    jobs.push(job);
    //tell blocked tasks to wake up
    jobLock.signal();

    // print transfer completed: student s, transfer amount a
    prt.print(Printer::WATCardOffice, 'T', sid, amount);

    // return the future-watcard
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork(){
    // if no jobs, wait
    if(jobs.empty()){
        jobLock.wait();
    }

    // if watcard office is closed, no need to handle jobs
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
            // wake all waiting tasks during termination
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
            // receive a job from the office
            Job *currJob = cardOffice.requestWork();
            if(currJob == NULL){ // if office is closed, terminate
                break;
            }

            // print transfer start: student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 't', currJob->args.id, currJob->args.amount);

            // process the job (transfer money)
            bank.withdraw(currJob->args.id, currJob->args.amount);
            currJob->args.watCard->deposit(currJob->args.amount);

            // print transfer complete: student s requesting transfer, amount a of transfer
            printer.print(Printer::Courier, id, 'T', currJob->args.id, currJob->args.amount);

            if(mprng(1,6) == 1){ // lose watCard :(
                currJob->result.exception(new WATCardOffice::Lost);
                delete currJob->args.watCard;
            }
            else{
                // deliver the watcard
                currJob->result.delivery(currJob->args.watCard);
            }
            delete currJob;
        }
    }
    //print finished
    printer.print(Printer::Courier, id, 'F');
}

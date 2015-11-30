#include "printer.h"
#include "bank.h"
#include "watCardOffice.h"


WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) : prt(prt), bank(bank), numCouriers(numCouriers){
    for (unsigned int i = 0; i < numCouriers; i++) {
        couriers[i] = new Courier(i);
    }
};

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount){
    //print create

    WATCard *watCard = new WATCard();

    Args args = {sid, amount, watCard};
    Job *job = new Job(args);

    jobs.push(job);
    //tell blocked tasks to wake up
    jobLock.signal();

    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card){
    //print transfer

    Args args = {sid, amount, card};
    Job *job = new Job(args);

    jobs.push(job);
    //tell blocked tasks to wake up
    jobLock.signal();

    return job->result;
}

Job *WATCardOffice::requestWork(){
    //print
    if(jobs.empty()){
        jobLock.wait();
    }

    Job *temp = jobs.front();
    jobs.pop();
    return temp;
}

void WATCardOffice::main(){
    //print starting
    _Accept(~WATCardOffice){
        // do something
    }
    //print finished
}

WATCardOffice::Courier::Courier(unsigned int id, Bank &bank, Printer &printer, WATCardOffice &cardOffice)
  : id(id), bank(bank), printer(printer), office(cardOffice){}


void WATCardOffice::Courier::main(){
    //print start

    while(true){
        _Accept(~Courier){
            break;
        } _Else {
            // print transfer start

            Job *currJob = cardOffice.requestWork();
            if(currJob == null){ //error check
                break;
            }

            // update
            bank.withdraw(currJob->args.id, currJob->args.amount);
            currJob->args.watCard->deposit(currJob->args.amount);

            if(mprng(1,6) == 1){ //lose watCard :(
                // TODO: review this (What is "insert into future"?)
                currJob->result.reset();
                currJob->result.exception(new WATCardOffice::Lost);
                delete currJob->args.watCard;
            }
            else{
                currJob->result.reset();
                currJob->result.delivery(currJob->args.watCard);
                //print something
            }
            // print transfer finished
            delete currJob;
        }
    }
    //print finished
}

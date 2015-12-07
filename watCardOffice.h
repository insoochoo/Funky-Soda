#include <queue>
#include "watCard.h"
#include "bank.h"

_Task WATCardOffice{
    Printer &prt;
    Bank &bank;
    unsigned int numCouriers;

    struct Args{
        unsigned int id;
        unsigned int amount;
        WATCard *watCard;
    };

    struct Job{                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job(Args args) : args(args){}
    };

    _Task Courier{ // communicates with bank
        unsigned int id;
        Bank &bank;
        Printer &printer;
        WATCardOffice &cardOffice;

        void main();
      public:
        Courier(unsigned int id, Bank &bank, Printer &printer, WATCardOffice &cardOffice);
    };

    Courier **couriers;
    std::queue<Job*> jobs; // queue of jobs that couriers will access
    uCondition jobLock; // sync lock for courier tasks
    bool finished;

    void main();
  public:
    _Event Lost{};                        // lost WATCard
    ~WATCardOffice();
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork(); // retrieve a job from jobs queue
};

#include <iostream>
#include <string>

#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watCardOffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"

using namespace std;

MPRNG mprng;


void usage(char *argv) {
    cerr << "Usage: " << argv << "  [ config-file [ random-seed (> 0) ] ]" << endl;
    exit(-1);
}

void uMain::main(){
    int seed = getpid(); // default seed value
    string fname = "config.txt"; // default config file

    try{
        switch(argc) {
            case 3:
                seed = stoi(argv[2]);
                if(seed <= 0){
                    usage(argv[0]);
                }
            case 2:
                fname = argv[1];
            case 1:
                break;
            default:
                usage(argv[0]);
        }
    }
    catch(...){
        usage(argv[0]);
    }
    /*
        SodaCost                2    # Manufacturer Suggested Retail Price (MSRP) per bottle
        NumStudents             2    # number of students to create
        MaxPurchases            8    # maximum number of bottles a student purchases
        NumVendingMachines      3    # number of vending machines
        MaxStockPerFlavour      5    # maximum number of bottles of each flavour in a vending machine
        MaxShippedPerFlavour    3    # maximum number of bottles of each flavour generated by the bottling
                                     #    plant per production run
        TimeBetweenShipments    3    # length of time between shipment pickup
        GroupoffDelay          10    # length of time between new initializing gift cards
        ParentalDelay           2    # length of time between new deposits of funds
        NumCouriers             1    # maximum number of couriers in the pool

        { "SodaCost", false, cparms.sodaCost },
        { "NumStudents", false, cparms.numStudents },
        { "MaxPurchases", false, cparms.maxPurchases },
        { "NumVendingMachines", false, cparms.numVendingMachines },
        { "MaxStockPerFlavour", false, cparms.maxStockPerFlavour },
        { "MaxShippedPerFlavour", false, cparms.maxShippedPerFlavour },
        { "TimeBetweenShipments", false, cparms.timeBetweenShipments },
        { "GroupoffDelay", false, cparms.groupoffDelay },
        { "ParentalDelay", false, cparms.parentalDelay },
        { "NumCouriers", false, cparms.numCouriers },
    */

    // set seed for random function
    mprng.seed(seed);

    // process config variables
    ConfigParms config;
    processConfigFile(fname.c_str(), config);

    // initiate all tasks of the program
    Student *students[config.numStudents];
    VendingMachine *VMs[config.numVendingMachines];

    Printer prt(config.numStudents, config.numVendingMachines, config.numCouriers);
    Bank bank(config.numStudents);
    Parent *parent = new Parent(prt, bank, config.numStudents, config.parentalDelay);
    WATCardOffice watCardOffice(prt, bank, config.numCouriers);
    Groupoff groupoff(prt, config.numStudents, config.sodaCost, config.groupoffDelay);
    NameServer nameServer(prt, config.numVendingMachines, config.numStudents);

    // initialize vending machines
    for(unsigned int i = 0; i < config.numVendingMachines; i++){
        VMs[i] = new VendingMachine(prt, nameServer, i, config.sodaCost, config.maxStockPerFlavour);
    }

    BottlingPlant *bottlingPlant = new BottlingPlant(prt, nameServer, config.numVendingMachines, config.maxShippedPerFlavour, config.maxStockPerFlavour, config.timeBetweenShipments);

    // initialize students
    for(unsigned int i = 0; i < config.numStudents; i++){
        students[i] = new Student(prt, nameServer, watCardOffice, groupoff, i, config.maxPurchases);
    }

    // finish all tasks
    for(unsigned int i = 0; i < config.numStudents; i++){
        delete students[i];
    }
    delete parent;

    delete bottlingPlant;
    for(unsigned int i = 0; i < config.numVendingMachines; i++){
        delete VMs[i];
    }
}

using namespace std;
#include <iostream>
#include "printer.h"

Printer::~Printer() {
    delete bufferStates;
    delete bufferSecond;
    delete bufferThird;
}
Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) :
  totalCount(numStudents + numVendingMachines + numCouriers + 6),
  numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers),
  bufferStates(new int[totalCount]), bufferSecond(new int[totalCount]), bufferThird(new int[totalCount]){
    //Something

    cout << "Parent\t";
    cout << "Gropoff\t";
    cout << "WATOff\t";
    cout << "Names\t";
    cout << "Truck\t";
    cout << "Plant\t";

    for(unsigned i = 0; i < numStudents; i++){
        cout << "Stud" << i;
        cout << "\t";
    }

    for(unsigned i = 0; i < numVendingMachines; i++){
        cout << "Mach" << i;
        cout << "\t";
    }

    for(unsigned i = 0; i < numCouriers; i++){
        cout << "Cour" << i;
        if(i < numCouriers-1){
            cout << "\t";
        }
    }
    cout << endl;

    for(unsigned i = 0; i < totalCount; i++){
        //initialize buffer containers
        bufferStates[i] = -1;
        bufferSecond[i] = -1;
        bufferThird[i] = -1;

        cout << "*******";
        if(i < totalCount-1){
            cout << "\t";
        }
    }
    cout << endl;
}

void Printer::printColumn(){
    // print
    for(unsigned int i = 0; i < totalCount; i++){
        if(bufferStates[i] != -1){
            cout << (char)bufferStates[i];
            if(bufferSecond[i] != -1){
                cout << bufferSecond[i];
                if(bufferThird[i] != -1){
                    cout << "," << bufferThird[i];
                }
            }
        }

        if(i < totalCount-1){
            cout << "\t";
        }

        bufferStates[i] = -1;
        bufferSecond[i] = -1;
        bufferThird[i] = -1;
    }
    cout << endl;
}

void Printer::finalFlush(unsigned id){
    for(unsigned int i = 0; i < totalCount; i++){
        if(id == i){
            cout << "F";
        } else{
            cout << "...";
        }
        if(i == totalCount-1){
            cout << endl;
        } else{
            cout << "\t";
        }

        bufferStates[i] = -1;
        bufferSecond[i] = -1;
        bufferThird[i] = -1;
    }
    cout << endl;
}

unsigned int Printer::getID(Kind kind, unsigned int lid){
    if(kind == Printer::Student){
        return (int)kind + lid;
    }

    if(kind == Printer::Vending){
        return (int)kind + lid + numStudents - 1;
    }

    if(kind == Printer::Courier){
        return (int)kind + lid + numStudents + numVendingMachines - 2;
    }

    return -1; // shouldnt reach here
}

// S, F, W, P
void Printer::print(Kind kind, char state){
    unsigned int id = (int)kind;

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;

    if(state == Printer::Finish){
        finalFlush(id);
    }
}

void Printer::print(Kind kind, char state, int value1){
    unsigned int id = (int)kind;

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;
    bufferSecond[id] = value1;
}

void Printer::print(Kind kind, char state, int value1, int value2){
    unsigned int id = (int)kind;

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;
    bufferSecond[id] = value1;
    bufferThird[id] = value2;
}

// S, F, W, P, L, r, R
void Printer::print(Kind kind, unsigned int lid, char state){
    unsigned int id = getID(kind, lid);

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;

    if(state == Printer::Finish){
        finalFlush(id);
    }
}

void Printer::print(Kind kind, unsigned int lid, char state, int value1){
    unsigned int id = getID(kind, lid);

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;
    bufferSecond[id] = value1;
}

void Printer::print(Kind kind, unsigned int lid, char state, int value1, int value2){
    unsigned int id = getID(kind, lid);

    if(bufferStates[id] != -1){
        printColumn();
    }

    bufferStates[id] = (int)state;
    bufferSecond[id] = value1;
    bufferThird[id] = value2;
}

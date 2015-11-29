#include "bank.h"

Bank::Bank(unsigned int numStudents) : numStudents(numStudents), balance(new int[numStudents]){
    for (unsigned int i = 0; i < numStudents, i++) {
        balance[i] = 0;
    }
}

void Bank::deposit(unsigned int id, unsigned int amount){
    balance[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount){
    while(balace[id] < amount){ // TODO: double check after courier
        _Accept(deposit);
    }
    balance[id] -= amount;
}

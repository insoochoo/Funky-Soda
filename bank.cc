#include "bank.h"

Bank::~Bank(){
    delete balance;
}

Bank::Bank(unsigned int numStudents) : numStudents(numStudents), balance(new int[numStudents]){
    // set initial balance for all students
    for (unsigned int i = 0; i < numStudents; i++) {
        balance[i] = 0;
    }
}

void Bank::deposit(unsigned int id, unsigned int amount){
    balance[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount){
    // when amount to withdraw is bigger than balance, wait until a deposit has happened
    // then check again
    while(balance[id] < amount){
        _Accept(deposit);
    }
    balance[id] -= amount;
}

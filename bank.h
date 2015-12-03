#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
  unsigned int numStudents;
  int *balance;

  public:
    ~Bank();
    Bank(unsigned int numStudents);
    void deposit(unsigned int id, unsigned int amount);
    void withdraw(unsigned int id, unsigned int amount);
};

#endif

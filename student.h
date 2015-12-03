#ifndef __STUDENT_H__
#define __STUDENT_H__

_Task Student {
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;
    unsigned int id;
    unsigned int maxPurchases;
    unsigned int boughtCount;

    void main();
  public:
    ~Student();
    Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases);
};

#endif // __STUDENT_H__

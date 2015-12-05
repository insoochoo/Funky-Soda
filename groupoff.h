_Task Groupoff {
    Printer &prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

    WATCard::FWATCard *fwatcards;

    int index;

    void main();
  public:
    ~Groupoff();
    Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);
    WATCard::FWATCard giftCard();
};

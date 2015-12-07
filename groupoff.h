_Task Groupoff {
    Printer &prt;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

    WATCard::FWATCard *fwatcards;

    int index;
    bool *available;// keep track of which students have received a giftcard

    void main();
  public:
    ~Groupoff();
    Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);
    WATCard::FWATCard giftCard();
};

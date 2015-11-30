_Task Groupoff {
    Printer &prtunsigned;
    int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;

    WATCard::FWATCard *fwatcards;

    void main();
  public:
    Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);
    WATCard::FWATCard giftCard();
};

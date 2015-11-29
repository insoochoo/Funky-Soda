_Monitor Bank {
  unsigned int numStudents;
  int *balance;

  public:
    Bank(unsigned int numStudents);
    void deposit(unsigned int id, unsigned int amount);
    void withdraw(unsigned int id, unsigned int amount);
};

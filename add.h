#ifndef ADD_H
#define ADD_H
typedef struct node
{
    int accNo;
    char name[20];
    char type[10];
    float balance;
    struct node *left;
    struct node *right;
} NODE;
NODE* insertAccount(NODE *root, int accNo, char name[], char type[], float amount);
NODE* deleteAccount(NODE* root, int accNo); 
void deposit(NODE *root, int accNo, float amount);
void withdraw(NODE *root, int accNo, float amount);
void displayAccountDetails(NODE *root, int accNo);
NODE* search(NODE *root, int acc);
void display(NODE *root);
// void generateReports(NODE *root);
// void saveToFile(NODE *root);
typedef struct Loan {
    int loanId;          // unique id for the loan
    int accNo;           // which account this loan belongs to
    char loanType[20];   // "personal", "home", "education"
    float principal;     // original amount
    float outstanding;   // amount still to be paid
    float interestRate;  // e.g. 8.5
    struct Loan *next;   // next loan node in list
} Loan;
Loan* findLoan(Loan *head, int loanId) ;
void addLoan(int accNo, const char *loanType, float amount, float interestRate);
void repayLoan(int loanId, float amount);
void createSampleAccounts();
#endif
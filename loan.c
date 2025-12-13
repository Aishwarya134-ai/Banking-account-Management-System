#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------- ACCOUNT STRUCT --------------------
typedef struct Account {
    int accNo;
    char name[50];
    char type[20];   // "Saving", "Current"
    float balance;
    struct Account *left, *right;   // BST for accounts
} Account;

Account *root = NULL;   // global account tree root

// -------------------- LOAN STRUCT --------------------
typedef struct Loan {
    int loanId;          // unique id for the loan
    int accNo;           // which account this loan belongs to
    char loanType[20];   // "personal", "home", "education"
    float principal;     // original amount
    float outstanding;   // amount still to be paid
    float interestRate;  // e.g. 8.5
    struct Loan *next;   // next loan node in list
} Loan;

Loan *loanHead = NULL;   // global loan list head
int nextLoanId = 1;      // loan ID counter

// -------------------- ACCOUNT FUNCTIONS --------------------
Account* insertAccount(Account *node, int accNo, const char *name, const char *type, float balance) {
    if (node == NULL) {
        Account *newNode = (Account*)malloc(sizeof(Account));
        newNode->accNo = accNo;
        strcpy(newNode->name, name);
        strcpy(newNode->type, type);
        newNode->balance = balance;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (accNo < node->accNo)
        node->left = insertAccount(node->left, accNo, name, type, balance);
    else if (accNo > node->accNo)
        node->right = insertAccount(node->right, accNo, name, type, balance);
    return node;
}

Account* searchAccount(Account *node, int accNo) {
    if (node == NULL || node->accNo == accNo) return node;
    if (accNo < node->accNo) return searchAccount(node->left, accNo);
    return searchAccount(node->right, accNo);
}

void deposit(Account *root, int accNo, float amount) {
    Account *acc = searchAccount(root, accNo);
    if (!acc) { printf("Account not found.\n"); return; }
    acc->balance += amount;
    printf("Deposited %.2f to Account %d. New balance: %.2f\n", amount, accNo, acc->balance);
}

void withdraw(Account *root, int accNo, float amount) {
    Account *acc = searchAccount(root, accNo);
    if (!acc) { printf("Account not found.\n"); return; }
    if (acc->balance < amount) { printf("Insufficient balance.\n"); return; }
    acc->balance -= amount;
    printf("Withdrew %.2f from Account %d. New balance: %.2f\n", amount, accNo, acc->balance);
}

// -------------------- LOAN FUNCTIONS --------------------
Loan* findLoan(Loan *head, int loanId) {
    while (head) {
        if (head->loanId == loanId) return head;
        head = head->next;
    }
    return NULL;
}

void addLoan(int accNo, const char *loanType, float amount, float interestRate) {
    Account *acc = searchAccount(root, accNo);
    if (acc == NULL) {
        printf("Account ID %d not found.\n", accNo);
        return;
    }
    if (amount <= 0) {
        printf("Loan amount must be positive.\n");
        return;
    }

    Loan *node = (Loan*)malloc(sizeof(Loan));
    if (!node) { printf("Memory error\n"); return; }

    node->loanId = nextLoanId++;
    node->accNo  = accNo;
    strncpy(node->loanType, loanType, sizeof(node->loanType)-1);
    node->loanType[sizeof(node->loanType)-1] = '\0';
    node->principal   = amount;
    node->outstanding = amount;
    node->interestRate = interestRate;

    node->next = loanHead;
    loanHead   = node;

    acc->balance += amount;   // credit loan to account

    printf("Loan approved.\n");
    printf("Loan ID   : %d\n", node->loanId);
    printf("Account ID: %d\n", accNo);
    printf("Type      : %s\n", node->loanType);
    printf("Amount    : %.2f\n", amount);
    printf("New account balance: %.2f\n", acc->balance);
}

void repayLoan(int loanId, float amount) {
    Loan *loan = findLoan(loanHead, loanId);
    if (loan == NULL) {
        printf("Loan ID %d not found.\n", loanId);
        return;
    }

    Account *acc = searchAccount(root, loan->accNo);
    if (acc == NULL) {
        printf("Account for this loan no longer exists.\n");
        return;
    }

    if (amount <= 0) {
        printf("Repayment amount must be positive.\n");
        return;
    }
    if (acc->balance < amount) {
        printf("Insufficient balance for repayment.\n");
        printf("Current balance: %.2f\n", acc->balance);
        return;
    }

    if (amount > loan->outstanding)
        amount = loan->outstanding;   // do not overpay

    acc->balance    -= amount;
    loan->outstanding -= amount;

    printf("Repayment successful.\n");
    printf("Loan ID   : %d\n", loan->loanId);
    printf("Paid      : %.2f\n", amount);
    printf("Outstanding: %.2f\n", loan->outstanding);
    printf("New account balance: %.2f\n", acc->balance);

    // If fully paid, remove loan from list
    if (loan->outstanding <= 0.0f) {
        Loan *prev = NULL, *cur = loanHead;
        while (cur && cur != loan) {
            prev = cur;
            cur  = cur->next;
        }
        if (cur) {
            if (prev) prev->next = cur->next;
            else      loanHead = cur->next;
            free(cur);
        }
        printf("Loan %d closed.\n", loanId);
    }
}

// -------------------- SAMPLE DATA --------------------
void createSampleAccounts() {
    root = insertAccount(root, 1001, "Alice",   "Saving",  5000);
    root = insertAccount(root, 1005, "Bob",     "Current", 2500);
    root = insertAccount(root, 1003, "Charlie", "Saving",  3200);
}

// -------------------- MAIN MENU --------------------
int main() {
    int choice;
    int accNo, loanId;
    float amount;
    int loanTypeChoice;
    char loanType[20];
    float rate;
    

    createSampleAccounts();

    while (1) {
        printf("\n--- BANK MENU ---\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Take Loan\n");
        printf("4. Repay Loan\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("Enter Account Number: ");
            scanf("%d", &accNo);
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            deposit(root, accNo, amount);
            break;

        case 2:
            printf("Enter Account Number: ");
            scanf("%d", &accNo);
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            withdraw(root, accNo, amount);
            break;

        case 3:
            printf("Enter Account Number: ");
            scanf("%d", &accNo);
            printf("Enter loan amount: ");
            scanf("%f", &amount);
            printf("Select loan type:\n");
            printf("1. Personal\n2. Home\n3. Education\n");
            scanf("%d", &loanTypeChoice);
            if (loanTypeChoice == 1) strcpy(loanType, "personal");
            else if (loanTypeChoice == 2) strcpy(loanType, "home");
            else if (loanTypeChoice == 3) strcpy(loanType, "education");
            else { printf("Invalid type.\n"); break; }

            printf("Enter interest rate (e.g. 8.5): ");
            scanf("%f", &rate);
            addLoan(accNo, loanType, amount, rate);
            break;

        case 4:
            printf("Enter Loan ID: ");
            scanf("%d", &loanId);
            printf("Enter repayment amount: ");
            scanf("%f", &amount);
            repayLoan(loanId, amount);
            break;

        case 5:
            printf("Goodbye.\n");
            return 0;

        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

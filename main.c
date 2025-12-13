#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int accNo;
    char name[20];
    char type[10];
    float balance;
    struct node *left;
    struct node *right;
} NODE;
NODE *search(NODE *root, int acc)
{
    if (root != NULL)
    {

        if (acc < root->accNo)
            root->left = search(root->left, acc);
        else if (acc > root->accNo)
            root->right = search(root->right, acc);
        else
        {
            printf("\n%d\t%s\t%s\t%f\t", root->accNo, root->name, root->type, root->balance);
            return root;
        }
    }    
    else{ 
        printf("\nResult not found.");
        return NULL;
        }
    
}
// Insert account into BST
NODE* insertAccount(NODE *root, int accNo, char name[], char type[], float amount) {
    if (root == NULL) {
        NODE *newNode = (NODE*) malloc(sizeof(NODE));
        if (!newNode) {
            printf("Memory allocation error");
            return root;
        }
        newNode->accNo = accNo;
        strcpy(newNode->name, name);
        strcpy(newNode->type, type);
        newNode->balance = amount;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    
    if (accNo < root->accNo) {
        root->left = insertAccount(root->left, accNo, name, type, amount);
    } else if (accNo > root->accNo) {
        root->right = insertAccount(root->right, accNo, name, type, amount);
    } else {
        // Duplicate account number, handle appropriately
        printf("Account number %d already exists.", accNo);
    }
    return root;
}

//Helper function to find minimum value node in BST (used in delete)
NODE* minValueNode(NODE* node) {
    NODE* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Delete account from BST by accNo
NODE* deleteAccount(NODE* root, int accNo) {
    if (root == NULL) {
        printf("Account %d not found.", accNo);
        return root;
    }

    if (accNo < root->accNo) {
        root->left = deleteAccount(root->left, accNo);
    } else if (accNo > root->accNo) {
        root->right = deleteAccount(root->right, accNo);
    } else {
        // Found the node to be deleted
        if (root->left == NULL) {
            NODE* temp = root->right;
            free(root);
            printf("Account %d deleted successfully.", accNo);
            return temp;
        } else if (root->right == NULL) {
            NODE* temp = root->left;
            free(root);
            printf("Account %d deleted successfully.", accNo);
            return temp;
        }

        // Node with two children: Get inorder successor (smallest in right subtree)
        NODE* temp = minValueNode(root->right);

        // Copy inorder successor's content to this node
        root->accNo = temp->accNo;
        strcpy(root->name, temp->name);
        strcpy(root->type, temp->type);
        root->balance = temp->balance;

        // Delete inorder successor
        root->right = deleteAccount(root->right, temp->accNo);
    }
    return root;
}
// Search prototype (already in your code)


// Deposit amount into account
void deposit(NODE *root, int accNo, float amount) {
    NODE *acc = search(root, accNo);
    if (acc == NULL) {
        printf("Account ID %d not found.", accNo);
        return;
    }

    acc->balance += amount;

    printf("Amount deposited successfully.");
    printf("Amount deposited: %.2f", amount);
    printf("New balance for account %d: %.2f", acc->accNo, acc->balance);
}

// Withdraw amount from account
void withdraw(NODE *root, int accNo, float amount) {
    NODE *acc = search(root, accNo);
    if (acc == NULL) {
        printf("Account ID %d not found.", accNo);
        return;
    }

    if (acc->balance < amount) {
        printf("Insufficient balance.");
        printf("Current balance for account %d: %.2f", acc->accNo, acc->balance);
        return;
    }

    acc->balance -= amount;
    printf("Amount withdrawn successfully.");
    printf("Amount withdrawn: %.2f", amount);
    printf("Remaining balance for account %d: %.2f", acc->accNo, acc->balance);
}
//Display account details
void displayAccountDetails(NODE *root, int accNo) {
    NODE *acc = search(root, accNo);
    if (acc == NULL) {
        printf("Account ID %d not found.", accNo);
        return;
    }

    printf("\nAccount Details:\n");
    printf("Account No: %d\n", acc->accNo);
    printf("Name: %s\n", acc->name);
    printf("Type: %s\n", acc->type);
    printf("Balance: %.2f\n", acc->balance);
}
// void generateReports(NODE *root) {
//     // Placeholder for report generation logic
//     printf("Generating reports...\n");
//     // Implement logic for highest balance, low-balance alerts, type filter
//}

// void saveToFile(NODE *root) {
//     // Placeholder for saving BST to file
//     printf("Saving accounts to file...\n");
//     // Implement file I/O logic here
// }
void display(NODE *root) {
    if (root != NULL) {
        display(root->left);
        printf("Account No: %d, Name: %s, Type: %s, Balance: %.2f\n", root->accNo, root->name, root->type, root->balance);
        display(root->right);
    }
}



int main()
{
    int choice, accNo;
    float amount;
    char name[50], type[10];

    // 1. Initialize: Load accounts from file into BST
    // loadFromFile(&root);  // Populates global root [web:19][web:24]
    NODE *root = NULL;
    while (1)
    {
        // 2. Display menu (clear screen, printf options 1-10)
        printf("\n=== BANK MANAGEMENT SYSTEM ===\n");
        printf("1. Insert an Account\n2. Deposit\n3. Withdraw\n5.Search\n4. Balance Enquiry\n");
        printf("6. Delete Account\n7. Mini Statement\n8.Display\n7. Reports\n8. Apply Interest\n");
        printf("9. Transfer Money\n0. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // Create Account
            printf("Account No: ");
            scanf("%d", &accNo);
            printf("Name: ");
            scanf("%s", name);
            printf("Type (SAVING/CURRENT): ");
            scanf("%s", type);
            printf("Initial Deposit: ");
            scanf("%f", &amount);
            root = insertAccount(root, accNo, name, type, amount);
            break;

        
        case 2: // Deposit/Withdraw
            printf("Account No: ");
            scanf("%d", &accNo);
            printf("Amount: ");
            scanf("%f", &amount);
            deposit(root, accNo, amount);
            break;
        case 3:
            printf("Account No: ");
            scanf("%d", &accNo);
            printf("Amount: ");
            scanf("%f", &amount);
            withdraw(root, accNo, amount);
            break;

        case 4: // Balance Enquiry
            printf("Account No: ");
            scanf("%d", &accNo);
            displayAccountDetails(root, accNo);
            break;
        case 5: // Search
            printf("Account No: ");
            scanf("%d", &accNo);
            search(root, accNo);
            break;

        case 6: // Delete Account
            printf("Account No: ");
            scanf("%d", &accNo);
            root = deleteAccount(root, accNo);
            break;

        case 7: // Mini Statement (unique feature)
            printf("Account No: ");
            scanf("%d", &accNo);
            displayAccountDetails(root, accNo); // Extended to show transactions
            break;
           case 8:
            display(root);
            break;
        // case 7:                    // Reports (unique feature)
        //     generateReports(root); // Highest balance, low-balance alerts, type filter
        //     break;

        // case 8: // Apply Interest (unique)
        //     applyAnnualInterest(root);
        //     break;

        // case 9: // Transfer (unique)
        //     // Get source, dest accounts and amount, call performTransaction twice
        //     printf("From Account: ");
        //     scanf("%d", &accNo);
        //     int destAcc;
        //     printf("To Account: ");
        //     scanf("%d", &destAcc);
        //     printf("Amount: ");
        //     scanf("%f", &amount);
        //     performTransaction(root, accNo, 2, amount);   // withdraw
        //     performTransaction(root, destAcc, 1, amount); // deposit
        //     break;

        case 0:               // Exit
            // saveToFile(root); // Persist BST to file
            printf("Thank you for using Bank Management System!\n");
            return 0;
        }
    }
    return 0;
}
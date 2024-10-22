#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold account information
struct Account {
    int accountNumber;
    char name[50];
    float balance;
};

// Function prototypes
void createAccount();
void viewAccounts();
void updateAccount();
void deleteAccount();
void menu();

// File to store account data
const char *filename = "accounts.dat";

// Function to create a new account
void createAccount() {
    struct Account account;
    FILE *file = fopen(filename, "ab"); // Open file in append-binary mode

    if (!file) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &account.accountNumber);
    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", account.name);
    printf("Enter Initial Balance: ");
    scanf("%f", &account.balance);

    fwrite(&account, sizeof(struct Account), 1, file); // Write account to file
    fclose(file);
    printf("Account created successfully.\n");
}

// Function to view all accounts
void viewAccounts() {
    struct Account account;
    FILE *file = fopen(filename, "rb"); // Open file in read-binary mode

    if (!file) {
        printf("Unable to open file.\n");
        return;
    }

    printf("\nAccount Details:\n");
    printf("---------------------------------------------------\n");
    printf("Account Number\tName\t\tBalance\n");
    printf("---------------------------------------------------\n");
    while (fread(&account, sizeof(struct Account), 1, file)) {
        printf("%d\t\t%s\t\t%.2f\n", account.accountNumber, account.name, account.balance);
    }
    fclose(file);
}

// Function to update an account
void updateAccount() {
    struct Account account;
    int accountNumber, found = 0;
    FILE *file = fopen(filename, "rb+"); // Open file in read-write mode

    if (!file) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter Account Number to Update: ");
    scanf("%d", &accountNumber);

    while (fread(&account, sizeof(struct Account), 1, file)) {
        if (account.accountNumber == accountNumber) {
            printf("Current Name: %s, Current Balance: %.2f\n", account.name, account.balance);
            printf("Enter New Name: ");
            scanf(" %[^\n]", account.name);
            printf("Enter New Balance: ");
            scanf("%f", &account.balance);
            fseek(file, -sizeof(struct Account), SEEK_CUR); // Move file pointer to overwrite
            fwrite(&account, sizeof(struct Account), 1, file); // Write modified data
            found = 1;
            break;
        }
    }

    if (found) {
        printf("Account updated successfully.\n");
    } else {
        printf("Account not found.\n");
    }
    fclose(file);
}

// Function to delete an account
void deleteAccount() {
    struct Account account;
    int accountNumber, found = 0;
    FILE *file = fopen(filename, "rb"); // Open file in read mode
    FILE *tempFile = fopen("temp.dat", "wb"); // Temporary file for storing non-deleted accounts

    if (!file || !tempFile) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter Account Number to Delete: ");
    scanf("%d", &accountNumber);

    while (fread(&account, sizeof(struct Account), 1, file)) {
        if (account.accountNumber != accountNumber) {
            fwrite(&account, sizeof(struct Account), 1, tempFile); // Write non-deleted accounts to temp file
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename); // Delete the old file
    rename("temp.dat", filename); // Rename the temporary file

    if (found) {
        printf("Account deleted successfully.\n");
    } else {
        printf("Account not found.\n");
    }
}

// Function to display the menu
void menu() {
    int choice;

    while (1) {
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Update Account\n");
        printf("4. Delete Account\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccounts();
                break;
            case 3:
                updateAccount();
                break;
            case 4:
                deleteAccount();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

// Main function
int main() {
    menu();
    return 0;
}

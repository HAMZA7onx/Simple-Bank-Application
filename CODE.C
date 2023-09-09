/* Basic log bank application */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { 
    char username[20]; 
    long password;
    long loan; 
    long dipposit;
} Account;

// A function that clear the buffer stream input until it returns the new line character '\n':
void clear_buffer() {
    while(getchar() != '\n');
}

// A function that display to the user if he already have an account or not, And exit option also:
int menu() {
    printf("-------------------------------------SIMPLE BANK APP MANAGEMENT:---------------------------------------\n");
    int choise_menu;
    do {
        printf("To Sign up enter(1):\n");
        printf("To Sign in enter(2):\n");
        printf("To Close the application enter(3):\n"); 
        printf("> ");
        if(scanf("%d", &choise_menu) != 1) choise_menu = 0;
        clear_buffer();
    } while (choise_menu < 1 || choise_menu > 3);

    return choise_menu;
}

// Back to the menu function: 
int back_menu() {
    char back_menu; 
    do {
        printf("Enter (Y/y) to back to the menu: "); 
        if (scanf(" %c", &back_menu) != 1) back_menu = 'n';
        clear_buffer();
    } while (back_menu != 'Y' && back_menu != 'y');
    return menu();
}

// Sign up function
void sign_up(Account *account, int *account_index, int *account_size) {
    // reallocation the memory spaces that account pointing on in the heap in case of filling the account array: 
    if (*account_index == *account_size) {
        *account_size += 50;
        account = (Account *)realloc(account, *account_size * sizeof(Account));
        if (account == NULL) {
            // Handle reallocation failure
            printf("Your memory fill up\n"); 
            exit(0);
        }
    }

    char *username  = account[*account_index].username; 
    long *password = &account[*account_index].password; 
    long *loan     = &account[*account_index].loan; 
    long *dipposit = &account[*account_index].dipposit;

    // Handling the username: 
    char string[20] = {};
    do {
        printf("Enter the username (must include at least 1 character and 4 digits): "); 
        fgets(string, 20, stdin); 
        if (string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0';
        else clear_buffer();
    } while (atoi(string) != 0 || strlen(string) < 5); 

    // Checking if that username exist on the account array as struct element or Not?
    int checker = 0;
    for (int i = 0; i < *account_index; i++) {
        if (strcmp(string, account[i].username) == 0) checker = 1;
    }

    if (checker == 1) { 
        printf("This username already exist, Try again!\n");
    }
    else {
        // Assigning the value of string characters array to username:
        strcpy(username, string); 

        // Handling the password:
        do {
            printf("Enter the password (must include just digits at least 9 digits): "); 
            if (scanf("%lu", password) != 1) *password = 0;
            clear_buffer();
        } while (*password / 100000000 < 1);

        // Incrementing the the account_index by 1:
        ++(*account_index);
    }
}

// Send money function 
void send_money(Account *account, int *account_index, int find_index) {
    // Entering the username:
    char string[20] = {};
    do {
        printf("Enter the username of the account that you will send the money into it (must include at least 1 character and 4 digits): "); 
        fgets(string, 20, stdin); 
        if (string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0';
        else clear_buffer();
    } while (atoi(string) != 0 || strlen(string) < 5); 
    
    int receive_acc_i;
    int checker = 0;
    for (int i = 0; i < *account_index; i++) {
        if (strcmp(string, account[i].username) == 0 && strcmp(string, account[find_index].username) != 0) {
            receive_acc_i = i;
            checker = 1;
        }
    }
    
    if (checker == 1) {
        long send_money;
        do { 
            printf("\t\tHow much money you will send (min value: 20$, max value: 1000$): "); 
            if (scanf("%lu", &send_money) != 1) send_money = 0;
            clear_buffer();
        } while (send_money < 20 || send_money > 1000);

        if (send_money < account[find_index].dipposit) {
            account[find_index].dipposit -= send_money; 
            account[receive_acc_i].dipposit += send_money; 
            account[receive_acc_i].loan += send_money; 
        }else {
            printf("\t\tYou don't have enough Money to do this process\n.");
        }

    }else { 
        printf("\t\tInvalid proccess, Check the account that you will send money!\n");
    }

}

// Sign in function
void sign_in (Account *account, int *account_index) {
    // Entering the username:
    char string[20] = {};
    do {
        printf("Enter the username (must include at least 1 character and 4 digits): "); 
        fgets(string, 20, stdin); 
        if (string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0';
        else clear_buffer();
    } while (atoi(string) != 0 || strlen(string) < 5); 

    // Entering the password:
    long password;
    do {
        printf("Enter the password (must include just digits at least 9 digits): "); 
        if (scanf("%lu", &password) != 1) password = 0;
        clear_buffer();
    } while (password / 100000000 < 1);

    // Checking if it is exist on the product array as a struct element or Not?
    int user_checker = 0;
    int find_index;
    for (int i = 0; i < *account_index; i++) {
        if (strcmp(string, account[i].username) == 0 && password == account[i].password) {
            user_checker = 1;
            find_index = i;
        }
    }
    
    if (user_checker == 1) {
        printf("-username: %s\t-password: %lu\t-dipposit: %lu\t-loan: %lu\n", account[find_index].username, account[find_index].password, account[find_index].dipposit, account[find_index].loan);

        // a functionnality that let the user send money to another account on the same product array:
        char answer;
        do {
            printf("\t\tDo You Want Send Money (Y / N)?"); 
            if (scanf(" %c", &answer) != 1) answer = '\0';
            clear_buffer();
        } while (answer != 'Y' && answer != 'N' && answer != 'y' && answer != 'n');

        if (answer == 'y') {
            send_money(account, account_index, find_index);
        }
    }else {
        printf("You did enter a wrong information, Check the username or the password!\n");
    }
}

int main() {
    int account_index = 2;
    int account_size = 100;
    Account *account; 
    account = (Account *)calloc(account_size, sizeof(Account));
    
    // initial data:
    strcpy(account[0].username, "Hamza Meski");
    account[0].password = 123456789; 
    account[0].loan     = 200; 
    account[0].dipposit = 4000;

    strcpy(account[1].username, "Taha 1234");
    account[1].password = 987654321; 
    account[1].loan     = 0; 
    account[1].dipposit = 500;

    int choise_menu;
    int close_app = 1;
    int cnd = 0;
    do {
        if (cnd == 0) {
            choise_menu = menu();
            cnd = 1;
        }else {
            choise_menu = back_menu();
        }
        switch(choise_menu) {
            case 1: sign_up(account, &account_index, &account_size); 
            break; 
            case 2: sign_in(account, &account_index); 
            break;
            case 3: close_app = 0;
            break;
        }
    } while (close_app);

    free(account);

    return 0;
}

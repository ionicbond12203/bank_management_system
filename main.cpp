#include <iostream>
#include <unistd.h>//for sleep
#include "auth.h"
#include "user.h"
#include "account.h"
#include "settings.h"
#include <windows.h>


using namespace std;

void preHome(string user_name, string password, string balance);
void displayHomeScreen(string user_name, string balance, string password);
int main() {
    SetConsoleOutputCP(CP_UTF8);//use to put emoji
    string user_name, password, balance;
    preHome(user_name, password, balance);
    return 0;
}

void preHome(string user_name, string password, string balance) {
    Auth auth;
    User user;
    Account account;
    Settings settings;

    int choice;
    do {
        cout << "Welcome to the Bank System\n";
        cout << "1. Login\n2. Register\n3. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter username: ";
            cin >> user_name;
            cout << "Enter password: ";
            cin >> password;

            if (auth.isAccountLocked(user_name)) {
                cout << "Your account is locked. Please try later.\n";
                return;
            }

            if (auth.signIn(user_name, password)) {
                cout << "Login successful!\n";
                auth.resetFailedAttempts(user_name);
                displayHomeScreen(user_name, balance, password);
            } else {
                cout << "Invalid username or password.\n";
                auth.recordFailedAttempt(user_name);
            }
        } else if (choice == 2) {
            cout << "Enter username: ";
            cin >> user_name;

            if (auth.usernameExists(user_name)) {
                cout << "Username already exists. Please choose another.\n";
                continue;
            }

            cout << "Enter password: ";
            cin >> password;

            if (auth.registerUser(user_name, password)) {
                cout << "Registration successful!\n";
                user.userInfoFillIn(user_name);
                user.createBalanceFile(user_name);
            } else {
                cout << "Registration failed.\n";
            }
        } else if (choice == 3) {
            cout << "Exiting the application.\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);
}

void displayHomeScreen(string user_name, string balance, string password) {
    Account account;
    Settings settings;
    int option;

    do {
        system("CLS");
        cout << "Welcome to the Home Screen, " << "\033[32m" << user_name << "\033[0m!" << endl;
        cout << "1. 🎯Check Account\n2. 🔁Transfer\n3. 💳Top Up\n4. 📜View Transaction History\n5. ⚙️Settings\n6. Delete Account🗑️\n7. 🔚Logout" << endl;
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1:
                account.check_account(user_name);
                break;
            case 2:
                account.transfer(user_name);
                break;
            case 3:
                account.top_up(user_name);
                break;
            case 4:
                account.view_transaction_history(user_name);
                break;
            case 5:
                settings.settings_interface(user_name);
                break;
            case 6:
                account.delete_account(user_name);
            case 7:
                system("CLS");
                cout << "Logging out." << endl;
                sleep(1);
                system("CLS");
                cout << "Logging out.." << endl;
                sleep(1);
                system("CLS");
                cout << "Logging out..." << endl;
                system("pause");
                return; // Ends the home screen loop to return to preHome
            default:
                cout << "\033[31m❌Invalid Input, try again❌\033[0m\n" << endl;
                break;
        }
    } while (true);
}

#include "user.h"
#include "utility.h"
#include <iostream>
#include <fstream>
using namespace std;

void User::userInfoFillIn(const string& user_name) {
    string Fname, Lname, email, address, IC;
    int day, month, year;

    cin.ignore();
    cout << "Please enter your first name: ";
    getline(cin, Fname);

    cout << "Please enter your last name: ";
    getline(cin, Lname);

    while (true) {
        cout << "Please enter your IC number (12 digits): ";
        getline(cin, IC);
        if (IC.length() == 12 && Utils().isAllDigits(IC)) {
            break;
        } else {
            cout << "\033[31m❌Invalid IC number. Please enter exactly 12 digits.\033[0m" << endl;
        }
    }

    while (true) {
        cout << "Please enter your birthday (day): ";
        cin >> day;
        cout << "Please enter your birthday (month): ";
        cin >> month;
        cout << "Please enter your birthday (year): ";
        cin >> year;
        if (day >= 1 && day <= 31 && month >= 1 && month <= 12) {
            break;
        } else {
            cout << "\033[31m❌Please enter a valid day or month.\033[0m" << endl;
        }
    }

    cin.ignore();
    while (true) {
        cout << "Please enter your email address: ";
        getline(cin, email);
        if (email.find('@') != string::npos && email.find(".com") != string::npos) {
            break;
        } else {
            cout << "\033[31m❌Invalid email. Email must contain '@' and '.com'.\033[0m" << endl;
        }
    }

    cout << "Please enter your home address: ";
    getline(cin, address);

    int age = Utils().calculateAge(year, month, day);
    createUserFile(user_name, age, Fname, Lname, IC, to_string(day) + "/" + to_string(month) + "/" + to_string(year), email, address);
}

void User::createUserFile(const string& user_name, const int& age, const string& Fname, const string& Lname, const string& IC, const string& birthday, const string& email, const string& address) {
    Utils().ensureDirectory(user_name);
    string fileName = user_name + "/info.txt";
    ofstream file(fileName);

    if (file.is_open()) {
        file << "User First Name: " << Fname << "\n";
        file << "User Last Name: " << Lname << "\n";
        file << "User Age: " << age << "\n";
        file << "User IC Number: " << IC << "\n";
        file << "User Birthday: " << birthday << "\n";
        file << "User Email: " << email << "\n";
        file << "User Address: " << address << "\n";
    }
}

void User::createBalanceFile(const string& user_name) {
    Utils().ensureDirectory(user_name);
    string fileName = user_name + "/balance.txt";
    ofstream file(fileName);
    if (file.is_open()) {
        file << "Balance: 0\n";
    }
}

#include "account.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

void Account::check_account(const string& user_name) {
    string filePath = user_name + "/" + "balance.txt";
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "\033[31m❌Error opening account file.❌\033[0m" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("Balance") != string::npos) {
            cout << "\033[32m" << line << "\033[0m" << endl; // Display account balance only
        }
    }
    file.close();
    system("pause");
    system("CLS");
}

void Account::transfer(const string& sender_name) {
    string recipient_name;
    double amount;

    cout << "Enter recipient username 👤: ";
    cin >> recipient_name;
    cout << "Enter amount to transfer 💲: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "\033[31m❌Invalid amount.❌\033[0m" << endl;
        return;
    }

    string sender_file = sender_name + "/balance.txt";
    string recipient_file = recipient_name + "/balance.txt";

    ifstream sender_account(sender_file);
    if (!sender_account.is_open()) {
        cerr << "\033[31m❌Sender account not found: " << sender_file << "❌\033[0m" << endl;
        return;
    }

    double sender_balance = 0;
    string line;
    while (getline(sender_account, line)) {
        if (line.find("Balance") != string::npos) {
            sender_balance = stod(line.substr(line.find(":") + 2));
            break;
        }
    }
    sender_account.close();

    if (sender_balance < amount) {
        cout << "\033[31m❌Insufficient balance!❌\033[0m" << endl;
        return;
    }

    ifstream recipient_account(recipient_file);
    if (!recipient_account.is_open()) {
        cerr << "\033[31m❌Recipient account not found: " << recipient_file << "❌\033[0m" << endl;
        return;
    }

    double recipient_balance = 0;
    while (getline(recipient_account, line)) {
        if (line.find("Balance") != string::npos) {
            recipient_balance = stod(line.substr(line.find(":") + 2));
            break;
        }
    }
    recipient_account.close();

    sender_balance -= amount;
    recipient_balance += amount;

    string sender_desc = "Transferred RM" + to_string(amount) + " to " + recipient_name;
    string recipient_desc = "Received RM" + to_string(amount) + " from " + sender_name;

    add_transaction(recipient_name, sender_name + " Transfer RM" + to_string(amount) + " Description: " + sender_desc);
    add_transaction(sender_name, recipient_name + " Receive RM" + to_string(amount) + " Description: " + recipient_desc);

    cout << "\033[32m✅ Transfer successful! ✅\033[0m" << endl;
    system("pause");
    system("CLS");
}

void Account::update_balance(const string& user_name, double new_balance) {
    string filePath = user_name + "/balance.txt";
    ofstream file(filePath);
    if (file.is_open()) {
        file << "Balance: " << new_balance << endl;
    } else {
        cerr << "Error updating balance." << endl;
    }
}

void Account::add_transaction(const string& user_name, const string& transaction_details) {
    string transaction_file = user_name + "/transactions.txt";

    ofstream file(transaction_file, ios::app); // Append mode
    if (!file.is_open()) {
        cerr << "Error: Unable to record transaction for " << user_name << "." << endl;
        return;
    }

    // Append transaction with timestamp
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ltm);

    file << "[" << time_str << "] " << transaction_details << "\n";
    file.close();
}

void Account::top_up(const string& user_name) {
    double amount;
    cout << "Enter amount to top-up: ";
    cin >> amount;

    if (amount <= 0 || amount > 20000) {
        cout << "Invalid amount." << endl;
        return;
    }

    string filePath = user_name + "/" + "balance.txt";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    double balance;
    while (getline(file, line)) {
        if (line.find("Balance") != string::npos) {
            balance = stod(line.substr(line.find(":") + 2));
        }
    }
    file.close();

    balance += amount;
    update_balance(user_name, balance);

    add_transaction(user_name, "Top Up RM" + to_string(amount) + " using ATM");

    cout << "\033[32m✅ Top-up successful! ✅\033[0m" << endl;
    cout << "New Balance: RM" << balance << endl;
    system("pause");
    system("CLS");
}

// Transaction sorting functions
Account::Transaction Account::parseTransactionLine(const string& line) {
    size_t dateEnd = line.find("]");
    string dateTime = line.substr(1, dateEnd - 1);
    string description = line.substr(dateEnd + 2);

    size_t spacePos = dateTime.find(" ");
    string date = dateTime.substr(0, spacePos);
    string time = dateTime.substr(spacePos + 1);

    double amount = 0;
    string sender;

    size_t rmPos = description.find("RM");
    if (rmPos != string::npos) {
        size_t endPos = description.find(" ", rmPos + 2);
        amount = stod(description.substr(rmPos + 2, endPos - rmPos - 2));
    }

    size_t fromPos = description.find("from");
    if (fromPos != string::npos) {
        sender = description.substr(fromPos + 5);
    }

    return {date, time, description, amount, sender};
}

vector<Account::Transaction> Account::readTransactions(const string& filePath) {
    ifstream file(filePath);
    vector<Transaction> transactions;

    if (!file.is_open()) {
        cerr << "Error opening transaction history file for sorting." << endl;
        return transactions;
    }

    string line;
    while (getline(file, line)) {
        transactions.push_back(parseTransactionLine(line));
    }
    file.close();
    return transactions;
}

void Account::sortAndDisplayTransactions(const string& user_name, const string& criteria, const string& order) {
    string filePath = user_name + "/transactions.txt";
    vector<Transaction> transactions = readTransactions(filePath);

    if (criteria == "date") {
        sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
            return order == "asc" ? a.date < b.date : a.date > b.date;
        });
    } else if (criteria == "time") {
        sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
            return order == "asc" ? a.time < b.time : a.time > b.time;
        });
    } else if (criteria == "amount") {
        sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
            return order == "asc" ? a.amount < b.amount : a.amount > b.amount;
        });
    } else if (criteria == "sender") {
        sort(transactions.begin(), transactions.end(), [&](const Transaction& a, const Transaction& b) {
            return order == "asc" ? a.sender < b.sender : a.sender > b.sender;
        });
    } else {
        cerr << "Invalid sorting criteria." << endl;
        return;
    }

    for (const auto& transaction : transactions) {
        cout << "[" << transaction.date << " " << transaction.time << "] " << transaction.description << endl;
    }
    system("pause");
    system("CLS");
}

void Account::view_transaction_history(const string& user_name) {
    cout << "Sort by (date/time/amount/sender): ";
    string criteria;
    cin >> criteria;

    cout << "Order (asc/desc): ";
    string order;
    cin >> order;

    sortAndDisplayTransactions(user_name, criteria, order);
}

void Account::delete_account(const string& user_name) {
    // Delete user directory
    string command = "rmdir /s /q " + user_name; // Windows command to delete the folder and its contents
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to delete user directory for: " << user_name << endl;
        return;
    }

    // Update the users.txt file to exclude the deleted user
    ifstream users_file("users.txt");
    ofstream temp_file("temp_users.txt");
    string file_username, file_password, file_account_number;

    if (!users_file.is_open() || !temp_file.is_open()) {
        cerr << "Error opening users file or creating a temporary file." << endl;
        return;
    }

    // Copy all users except the one to be deleted into the temporary file
    while (users_file >> file_username >> file_password >> file_account_number) {
        if (file_username != user_name) {
            temp_file << file_username << " " << file_password << " " << file_account_number << "\n";
        }
    }

    users_file.close();
    temp_file.close();

    // Replace the original users.txt with the updated temp file
    if (remove("users.txt") != 0 || rename("temp_users.txt", "users.txt") != 0) {
        cerr << "Error updating users.txt after deleting the account." << endl;
    } else {
        cout << "Account and associated data for user '" << user_name << "' deleted successfully." << endl;
    }
}


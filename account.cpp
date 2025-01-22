#include "account.h"
#include <fstream>
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

    // Validate input to ensure the amount is a valid number
    while (!(cin >> amount) || amount <= 0) {
        cout << "\033[31m❌Invalid amount. Please enter a positive number: ❌\033[0m";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string sender_file = sender_name + "/balance.txt";
    string recipient_file = recipient_name + "/balance.txt";

    // Check sender account
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

    // Check recipient account
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

    // Deduct amount from sender and add to recipient
    sender_balance -= amount;
    recipient_balance += amount;

    // Update balances
    update_balance(sender_name, sender_balance);
    update_balance(recipient_name, recipient_balance);

    // Log the transaction
    string sender_desc = "Transferred RM" + to_string(amount) + " to " + recipient_name;
    string recipient_desc = "Received RM" + to_string(amount) + " from " + sender_name;

    add_transaction(sender_name, sender_desc);
    add_transaction(recipient_name, recipient_desc);

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

    // Input validation loop
    while (!(cin >> amount) || amount <= 0 || amount > 20000) {
        cout << "\033[31mInvalid amount. Please enter a number between 0 and 20,000\033[0m";
        cin.clear();                // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
    }

    string filePath = user_name + "/" + "balance.txt";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;
    double balance = 0.0; // Default balance to 0
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

// Helper function to parse each transaction line
Account::Transaction Account::parseTransactionLine(const string& line) {
    Transaction transaction;

    size_t datePos = line.find("[");
    size_t endDatePos = line.find("]");
    size_t amountPos = line.find("Amount: RM");
    size_t senderPos = line.find("Sender: ");

    if (datePos != string::npos && endDatePos != string::npos) {
        transaction.date = line.substr(datePos + 1, endDatePos - datePos - 1);
    }

    size_t descriptionEnd = amountPos == string::npos ? senderPos : amountPos;
    transaction.description = line.substr(endDatePos + 2, descriptionEnd - endDatePos - 2);

    if (amountPos != string::npos) {
        size_t amountEndPos = line.find(" ", amountPos + 10);
        transaction.amount = stod(line.substr(amountPos + 10, amountEndPos - amountPos - 10));
    }

    if (senderPos != string::npos) {
        transaction.sender = line.substr(senderPos + 8);
    }

    return transaction;
}

// Read transactions from file and parse each line
vector<Account::Transaction> Account::readTransactions(const string& filePath) {
    vector<Transaction> transactions;
    ifstream file(filePath);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening transaction file: " << filePath << endl;
        return transactions;
    }

    while (getline(file, line)) {
        if (!line.empty()) {
            Transaction transaction = parseTransactionLine(line);
            transactions.push_back(transaction);
        }
    }

    file.close();
    return transactions;
}

// QuickSort implementation
void Account::quickSort(vector<Transaction>& transactions, int low, int high, const string& criteria, const string& order) {
    if (low < high) {
        int pivotIndex = partition(transactions, low, high, criteria, order);
        quickSort(transactions, low, pivotIndex - 1, criteria, order);
        quickSort(transactions, pivotIndex + 1, high, criteria, order);
    }
}

int Account::partition(vector<Transaction>& transactions, int low, int high, const string& criteria, const string& order) {
    Transaction pivot = transactions[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        bool condition = false;

        if (criteria == "date") {
            condition = (order == "asc") ? (transactions[j].date < pivot.date) : (transactions[j].date > pivot.date);
        } else if (criteria == "time") {
            condition = (order == "asc") ? (transactions[j].time < pivot.time) : (transactions[j].time > pivot.time);
        } else if (criteria == "amount") {
            condition = (order == "asc") ? (transactions[j].amount < pivot.amount) : (transactions[j].amount > pivot.amount);
        } else if (criteria == "sender") {
            condition = (order == "asc") ? (transactions[j].sender < pivot.sender) : (transactions[j].sender > pivot.sender);
        }

        if (condition) {
            i++;
            swap(transactions[i], transactions[j]);
        }
    }
    swap(transactions[i + 1], transactions[high]);
    return i + 1;
}

// Display transactions after sorting
void Account::sortAndDisplayTransactions(const string& user_name, const string& criteria, const string& order) {
    string filePath = user_name + "/transactions.txt";
    vector<Transaction> transactions = readTransactions(filePath);

    quickSort(transactions, 0, transactions.size() - 1, criteria, order);

    // Only display date, time, and description
    for (const auto& transaction : transactions) {
        cout << "[" << transaction.date << "] " << transaction.description << endl;
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
    string command = "rmdir /s /q " + user_name; // Windows command to delete the folder and its contents
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to delete user directory for: " << user_name << endl;
        return;
    }

    ifstream users_file("users.txt");
    ofstream temp_file("temp_users.txt");
    string line;

    while (getline(users_file, line)) {
        if (line != user_name) {
            temp_file << line << endl;
        }
    }

    users_file.close();
    temp_file.close();

    remove("users.txt");
    rename("temp_users.txt", "users.txt");

    cout << "\033[32m✅ Account deleted successfully. ✅\033[0m" << endl;
}

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
using namespace std;

class Account {
public:
    // Public member functions
    void check_account(const string& user_name);
    void transfer(const string& sender_name);
    void update_balance(const string& user_name, double new_balance);
    void add_transaction(const string& user_name, const string& transaction_details);
    void top_up(const string& user_name);
    void view_transaction_history(const string& user_name);  // Function declaration
    void delete_account(const string& user_name);  // Function declaration

private:
    // Structure to store transaction details
    struct Transaction {
        string date;
        string time;
        string description;
        double amount;
        string sender;
    };

    // Private helper functions for transactions
    vector<Transaction> readTransactions(const string& filePath);
    void sortAndDisplayTransactions(const string& user_name, const string& criteria, const string& order);

    // QuickSort and partitioning functions
    void quickSort(vector<Transaction>& transactions, int low, int high, const string& criteria, const string& order);
    int partition(vector<Transaction>& transactions, int low, int high, const string& criteria, const string& order);

    // Helper function to parse each transaction line
    Transaction parseTransactionLine(const string& line);
};

#endif // ACCOUNT_H

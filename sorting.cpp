// sorting.cpp
#include "sorting.h"
#include <iostream>
#include <vector>
#include <string>

Transaction::Transaction(std::string timestamp, std::string type, std::string description)
    : timestamp(timestamp), type(type), description(description) {}

// Insertion Sort function
void insertionSort(std::vector<Transaction>& transactions, bool (*compare)(const Transaction&, const Transaction&)) {
    for (size_t i = 1; i < transactions.size(); ++i) {
        Transaction key = transactions[i];
        size_t j = i - 1;

        // Shift elements of transactions[0..i-1] that are greater than key to one position ahead
        while (j >= 0 && compare(transactions[j], key)) {
            transactions[j + 1] = transactions[j];
            if (j == 0) break;  // Prevent underflow
            --j;
        }
        transactions[j + 1] = key;
    }
}

// Comparison function for sorting by date (lexicographical comparison of timestamp)
bool compareByDate(const Transaction& t1, const Transaction& t2) {
    return t1.timestamp < t2.timestamp;
}

// Comparison function for sorting by transaction type (lexicographical comparison of type)
bool compareByType(const Transaction& t1, const Transaction& t2) {
    return t1.type < t2.type;
}

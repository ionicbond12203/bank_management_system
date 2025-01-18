// sorting.h
#ifndef SORTING_H
#define SORTING_H

#include <string>
#include <vector>

// Define the Transaction struct
struct Transaction {
    std::string timestamp;
    std::string type;
    std::string description;

    Transaction(std::string timestamp, std::string type, std::string description);
};

// Function to sort transactions by either date or type using insertion sort
void insertionSort(std::vector<Transaction>& transactions, bool (*compare)(const Transaction&, const Transaction&));

// Comparison functions for sorting
bool compareByDate(const Transaction& t1, const Transaction& t2);
bool compareByType(const Transaction& t1, const Transaction& t2);

#endif // SORTING_H


#ifndef UTILITY_H
#define UTILITY_H

#include <string>
using namespace std;

class Utils {
public:
    bool isAllDigits(const string& str);
    int calculateAge(int birthYear, int birthMonth, int birthDay);
    void ensureDirectory(const string& folder_name);
};

#endif // UTILITY_H

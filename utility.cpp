#include "utility.h"
#include <iostream>
#include <sys/stat.h>
#include <ctime>
using namespace std;

bool Utils::isAllDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int Utils::calculateAge(int birthYear, int birthMonth, int birthDay) {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    int age = currentYear - birthYear;
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}

void Utils::ensureDirectory(const string& folder_name) {
    struct stat st;
    if (stat(folder_name.c_str(), &st) != 0) {
        mkdir(folder_name.c_str());
    }
}

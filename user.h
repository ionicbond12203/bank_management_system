#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
public:
    void userInfoFillIn(const string& user_name);
    void createUserFile(const string& user_name, const int& age, const string& Fname, const string& Lname, const string& IC, const string& birthday, const string& email, const string& address);
    void createBalanceFile(const string& user_name);
};

#endif // USER_H

#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <unordered_map>//using hashtable
#include <queue>
using namespace std;

class Auth {
private:
    unordered_map<string, pair<string, string>> users; // username -> (password, account_number)
public:
    bool signIn(const string& user_name, const string& password);
    bool registerUser(const string& user_name, const string& password);
    bool usernameExists(const string& user_name);
    bool isAccountLocked(const string& user_name);//queue
    void recordFailedAttempt(const string& user_name);//queue
    void resetFailedAttempts(const string& user_name);
    string generateAccountNumber();
};

#endif // AUTH_H

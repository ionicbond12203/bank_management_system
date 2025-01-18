#include "auth.h"
#include "utility.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unistd.h>//for sleep
#include <unordered_map>//hashtable
#include <queue>
using namespace std;

bool Auth::signIn(const string& user_name, const string& password) {
    ifstream users_file("users.txt");
    string file_username, file_password;

    if (!users_file.is_open()) {
        cerr << "Error opening users file." << endl;
        return false;
    }

    while (users_file >> file_username >> file_password) {
        if (file_username == user_name && file_password == password) {
            return true;
        }
    }

    return false;
}

string Auth::generateAccountNumber() {
    string account_file = "account_number.txt";
    ifstream file_in(account_file);
    int account_number = 0;

    // Read the last account number
    if (file_in.is_open()) {
        file_in >> account_number;
        file_in.close();
    }

    // Increment the account number
    account_number++;

    // Write the updated account number back to the file
    ofstream file_out(account_file);
    if (file_out.is_open()) {
        file_out << account_number;
        file_out.close();
    }

    // Format the account number to 8 digits
    stringstream ss;
    ss << setw(8) << setfill('0') << account_number;
    return ss.str();
}



bool Auth::registerUser(const string& user_name, const string& password) {
    // Check if the username already exists in the hashtable
    if (users.find(user_name) != users.end()) {
        cerr << "Username already exists." << endl;
        return false;
    }

    // Generate a new account number
    string account_number = generateAccountNumber();

    // Add the new user to the hashtable
    users[user_name] = make_pair(password, account_number);

    // Append the new user to the users.txt file
    ofstream users_file("users.txt", ios::app);
    if (!users_file.is_open()) {
        cerr << "Error opening users file." << endl;
        return false;
    }
    users_file << user_name << " " << password << " " << account_number << "\n";
    users_file.close();

    // Print confirmation for the user
    cout << "Your account number is: " << account_number << endl;

    return true;
}

//queue
bool Auth::isAccountLocked(const string& user_name) {
    string attemptsFile = "attempt_record/" + user_name + "_attempts.txt";
    ifstream file(attemptsFile);

    std::queue<time_t> failed_attempts_queue;
    time_t currentTime = time(0);

    // Read the failed attempts from the file
    time_t timestamp;
    while (file >> timestamp) {
        failed_attempts_queue.push(timestamp);
    }

    // Remove failed attempts older than the lockout duration
    while (!failed_attempts_queue.empty() && difftime(currentTime, failed_attempts_queue.front()) >= 3600) {
        failed_attempts_queue.pop();
    }

    // If the queue size is greater than or equal to the maximum login attempts, lock the account
    if (failed_attempts_queue.size() >= 3) {
        cout << "Account is locked. Try again later." << endl;
        return true;
    }

    return false;  // Account is not locked
}

//queue
void Auth::recordFailedAttempt(const string& user_name) {
    const int MAX_LOGIN_ATTEMPTS = 3;
    const int LOCKOUT_DURATION = 3600; // Lockout duration in seconds (1 hour)

    Utils().ensureDirectory("attempt_record");

    string attemptsFile = "attempt_record/" + user_name + "_attempts.txt";
    ifstream file_in(attemptsFile);

    std::queue<time_t> failed_attempts_queue;  // Queue to store timestamps of failed attempts
    time_t currentTime = time(0);  // Get current time (timestamp)

    // Read the failed attempts from the file (queue will store timestamps)
    time_t timestamp;
    while (file_in >> timestamp) {
        failed_attempts_queue.push(timestamp);
    }

    // Remove failed attempts that are older than the lockout duration
    while (!failed_attempts_queue.empty() && difftime(currentTime, failed_attempts_queue.front()) >= LOCKOUT_DURATION) {
        failed_attempts_queue.pop();
    }

    // Add the new failed attempt's timestamp to the queue
    failed_attempts_queue.push(currentTime);

    // If the queue reaches the MAX_LOGIN_ATTEMPTS limit, lock the account
    if (failed_attempts_queue.size() >= MAX_LOGIN_ATTEMPTS) {
        // Lock the account
        time_t lockout_time = currentTime;

        // Write the failed attempts and lockout time to the file
        ofstream file_out(attemptsFile);
        while (!failed_attempts_queue.empty()) {
            file_out << failed_attempts_queue.front() << "\n";
            failed_attempts_queue.pop();
        }
        file_out << lockout_time;  // Add lockout time at the end

        cout << "Account locked due to too many failed attempts. Try again later." << endl;
    } else {
        // Write the failed attempts and update file without lockout
        ofstream file_out(attemptsFile);
        while (!failed_attempts_queue.empty()) {
            file_out << failed_attempts_queue.front() << "\n";
            failed_attempts_queue.pop();
        }
        file_out.close();
    }
}


// Reset failed attempts after successful login
void Auth::resetFailedAttempts(const string& user_name) {
    Utils().ensureDirectory("attempt_record");

    string attemptsFile = "attempt_record/" + user_name + "_attempts.txt";
    ofstream file(attemptsFile);
    file << "0 0";  // Reset failed attempts and lockout time
}

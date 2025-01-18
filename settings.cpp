#include "settings.h"
#include "auth.h"
#include "utility.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>//for sleep
using namespace std;

void Settings::settings_interface(string &user_name){
    do{
    int choice;
    system("CLS");
    cout << "Settings"<<endl;
    cout << "1. Change username\n2. View user information\n3. Edit password\n4. Exit settings"<<endl;
    cout << "Enter your choice: ";
    cin >> choice;
     switch (choice) {
            case 1:
                system("CLS");
                rename(user_name);
                break;
            case 2:
                system("CLS");
                view_user_info(user_name);
                break;
            case 3:
                system("CLS");
                changePassword(user_name);
            case 4:
                cout<< "Exiting settings."<<endl;
                sleep(2);
                return;
            default:
                cout << "Invalid choice";
                sleep(1);
     }
   }while (true);
}

void Settings::rename(string &user_name) {
    Auth auth;

    string new_user_name;
    cout <<"Enter new username: ";
    cin >>new_user_name;

    if (auth.usernameExists(new_user_name)){
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }

    //update user folder
    string old_folder = user_name;
    string new_folder = new_user_name;

    // Renaming folder
    if (std::rename(old_folder.c_str(), new_folder.c_str()) != 0) {
        cerr << "Error renaming folder." << endl;
        return;
    }

    //update users.txt file
    ifstream users_file_in("users.txt");
    ofstream users_file_out("users_temp.txt");
    string file_username,file_password;

    while (users_file_in >> file_username >> file_password) {
        if (file_username == user_name) {
            users_file_out << new_user_name << " " << file_password << "\n";
        } else {
            users_file_out << file_username << " " << file_password << "\n";
        }
    }

    //replace old users.txt with updated one
    users_file_in.close();
    users_file_out.close();

    // Replace old users.txt with the updated one
    remove("users.txt");
    std::rename("users_temp.txt", "users.txt");

    user_name = new_user_name;
    cout << "Username successfully changed to " << new_user_name << endl;
    system("pause");
    system("CLS");
}

void Settings::changePassword(string &user_name)
{
    string new_password, confirm_password;
    cout << "Enter new password: ";
    cin >> new_password;

    cout << "Confirm new password: ";
    cin >> confirm_password;

    // Check if the passwords match
    if (new_password != confirm_password) {
        cout << "Passwords do not match. Please try again." << endl;
        return;
    }

    // Open the existing users file
    ifstream users_file_in("users.txt");
    ofstream users_file_out("users_temp.txt");
    string file_username, file_password;

    if (!users_file_in.is_open() || !users_file_out.is_open()) {
        cerr << "Error opening users file." << endl;
        return;
    }

    // Update the user's password in the temporary file
    bool password_updated = false;
    while (users_file_in >> file_username >> file_password) {
        if (file_username == user_name) {
            users_file_out << file_username << " " << new_password << "\n";
            password_updated = true;
        } else {
            users_file_out << file_username << " " << file_password << "\n";
        }
    }

    users_file_in.close();
    users_file_out.close();

    // Replace old users.txt with the updated one
    if (password_updated) {
        remove("users.txt");
        std::rename("users_temp.txt", "users.txt");
        cout << "Password successfully updated for user: " << user_name << endl;
    } else {
        cout << "Failed to update password. User not found." << endl;
        remove("users_temp.txt"); // Clean up temporary file
    }
    system("pause");
    system("CLS");
}

void Settings::view_user_info(string& user_name)
{
    Utils().ensureDirectory(user_name);
    int input;
    string filePath = user_name + "/" + "info.txt";
    ifstream file(filePath);

    // Ensure file exists
    if (!file.is_open()) {
        cerr << "Error opening file for user: " << user_name << endl;
        system("pause");
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl; // Display each transaction
    }
    file.close();

    cout << "did you want to edit profile?\n";
    cout << "1 = yes, 2 = no\n";
    cin >> input;

    if(input = '1')
    {
        system("CLS");
        editUserInfo(user_name);
    }else if (input = '2')
    {
        system("pause");
        system("CLS");
    }else
    {
        cout << "invalid input\n";
        system("pause");
        system("CLS");
    }
    settings_interface(user_name);

}

void Settings::editUserInfo(const string& user_name) {
    int input;
    string folderName = user_name;
    string fileName = folderName + "/"  + " info.txt";

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Failed to open user info file." << endl;
        return;
    }

    // Read existing user info
    string line, Fname, Lname, age, IC, birthday, email, address;
    while (getline(file, line)) {
        if (line.find("User First Name:") != string::npos) {
            Fname = line.substr(line.find(":") + 2);
        } else if (line.find("User Last Name:") != string::npos) {
            Lname = line.substr(line.find(":") + 2);
        } else if (line.find("User age:") != string::npos) {
            age = line.substr(line.find(":") + 2);
        } else if (line.find("User IC Number:") != string::npos) {
            IC = line.substr(line.find(":") + 2);
        } else if (line.find("User Birthday:") != string::npos) {
            birthday = line.substr(line.find(":") + 2);
        } else if (line.find("User Email:") != string::npos) {
            email = line.substr(line.find(":") + 2);
        } else if (line.find("User Address:") != string::npos) {
            address = line.substr(line.find(":") + 2);
        }
    }
    file.close();

    // Display current info to the user
    cout << "Current information:" << endl;
    cout << "User First Name: " << Fname << endl;
    cout << "User Last Name: " << Lname << endl;
    cout << "User Age: " << age << endl;
    cout << "User IC Number: " << IC << endl;
    cout << "User Birthday: " << birthday << endl;
    cout << "User Email: " << email << endl;
    cout << "User Address: " << address << endl;
    cout << "Only allow to edit Email and Address only" << endl;


    // Prompt for updated email
    string new_email, new_address;

    while (true)
        {
        cout << "Please enter your email address: ";
        getline(cin, new_email);

        // Check if the email contains both '@' and '.com'
        if (new_email.find('@') != string::npos && new_email.find(".com") != string::npos) {
            break; // Email is valid
        } else {
            cout << "Invalid email. Email must contain '@' and '.com'." << endl;
        }
    }

    // Prompt for updated address
    cout << "Enter new address: ";
    getline(cin, new_address);


    // Update the file with new email or address
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << "User First Name: " << Fname << "\n";
        outFile << "User Last Name: " << Lname << "\n";
        outFile << "User age: " << age << "\n";
        outFile << "User IC Number: " << IC << "\n";
        outFile << "User Birthday: " << birthday << "\n";
        outFile << "User Email: " << new_email << "\n";
        outFile << "User Address: " << new_address << "\n";
        outFile.close();

        cout << "\nInformation updated successfully!" << endl;
        system("pause");
    } else {
        cerr << "Failed to update user info file." << endl;
        system("pause");
    }
}

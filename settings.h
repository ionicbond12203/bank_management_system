#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
using namespace std;

class Settings {
public:
    void rename(string& user_name);
    void changePassword(string& user_name);
    void settings_interface(string& user_name);
    void view_user_info(string& user_name);
    void editUserInfo(const string& user_name);
};

#endif // SETTINGS_H

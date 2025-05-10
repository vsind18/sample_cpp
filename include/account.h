#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <openssl/evp.h>
#include <filesystem>
#include <regex>
#include <utility>
#include <fstream>

class accountInfo{
    private:
        std::string username;
        std::string password;
        std::string fullName;
        std::string email;
        std::string phoneNumber;
        std::string role;
    
    public: 
        std::string hashPassword(const std::string& pwd);

        accountInfo(const std::string &uname, const std::string& pwd,
                const std::string &fname, const std::string &e,
                const std::string &phone, const std::string &r);
        
        void displayInfo() const;

        std::string getPassword();

        void setUsername(const std::string& uname);
        void setPassword(const std::string& pwd);
        void setFullName(const std::string& fname);
        void setEmail(const std::string& e);
        void setPhoneNumber(const std::string& phone);
        void setRole(const std::string& r);
};

bool isValidEmail(const std::string& email);
bool isValidPhoneNumber(const std::string& phone);
bool isValidRole(const std::string& role);

std::pair<std::string, std::string> signIn();
void signUp(const std::string& uname = "", const std::string& pwd = "", 
            const std::string& fname = "", const std::string& mail = "", 
            const std::string& phone = "", const std::string& role = "",
            bool changePassFlag = false);

#endif //ACCOUNT_H
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <openssl/evp.h>
#include <filesystem>

struct userInfo{
    std::string userName;
    std::string email;
    std::string fullName;
    std::string phoneNumber;
    std::string role;
};

class accountInfo{
    private:
        std::string userID;
        std::string username;
        std::string password;
        std::string fullName;
        std::string email;
        std::string phoneNumber;
        std::string role;
    
    public: 
        std::string hashPassword(const std::string& pwd);

        accountInfo(const std::string &uID, const std::string &uname, const std::string& pwd,
                const std::string &fname, const std::string &e,
                const std::string &phone, const std::string &r);
        
        userInfo getUserByID(const std::string uID);
        int signIn(const std::string uName, const std::string pwd);
        int signUp(const std::string &uID = "", const std::string& uname = "" , const std::string& pwd = "", 
            const std::string& fname = "", const std::string& mail = "", 
            const std::string& phone = "", const std::string& role = "",
            const std::string& roleCreator = "", bool changePassFlag = false);
};

std::string getNextUIDFromCSV(const std::string& filename = "list_account.csv");
#endif //ACCOUNT_H
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <openssl/evp.h>

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
};

#endif //ACCOUNT_H
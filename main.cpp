#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include "include/account.h"

/*  g++ -o main main.cpp src/account.cpp -lssl -lcrypto */
bool isValidEmail(const std::string& email) {
    std::regex emailRegex("^[a-zA-Z0-9._%+-]+@gmail\\.com$");
    return std::regex_match(email, emailRegex);
}

bool isValidPhoneNumber(const std::string& phone) {
    std::regex phoneRegex("^[0-9]+$"); 
    return std::regex_match(phone, phoneRegex);
}

bool isValidRole(const std::string& role) {
    return role == "user" || role == "admin";
}

std::string signIn(){
    std::cout << "\n =====Sign in====\n";
    std::string usname, pwd;
    std::cout << "Username: ";
    std::getline(std::cin, usname);
    std::cout << "Password: ";
    std::getline(std::cin, pwd);

    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.\n";
        return "";
    }

    std::string line;
    std::getline(infile, line); // skip header

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string username, password, fullName, email, phone, role;

        std::getline(iss, username, ',');
        std::getline(iss, password, ',');
        std::getline(iss, fullName, ',');
        std::getline(iss, email, ',');
        std::getline(iss, phone, ',');
        std::getline(iss, role, ',');

        accountInfo tmpAcc(username, pwd, fullName, email, phone, role);
        std::string hashPass = tmpAcc.hashPassword(pwd);

        if (username == usname && password == hashPass) {
            std::cout << "Login successful! Role: " << role << "\n";
            return role;
        }
    }

    std::cout << "Invalid username or password.\n";
    return "";
}

void signUp(const std::string& uname = "", const std::string& pwd = "", 
            const std::string& fname = "", const std::string& mail = "", 
            const std::string& phone = "", const std::string& role = "") {
    std::string username = uname, password = pwd, fullName = fname, email = mail, phoneNumber = phone, userRole = role;

    if (username.empty()) {
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
    }
    if (password.empty()) {
        std::cout << "Enter password: ";
        std::getline(std::cin, password);
    }
    if (fullName.empty()) {
        std::cout << "Enter full name: ";
        std::getline(std::cin, fullName);
    }
    do {
        if (email.empty()) {
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
        }
        if (!isValidEmail(email)) {
            std::cout << "Invalid email. Please enter a valid Gmail address.\n";
            email.clear();
        }
    } while (email.empty() || !isValidEmail(email));

    do {
        if (phoneNumber.empty()) {
            std::cout << "Enter phone number: ";
            std::getline(std::cin, phoneNumber);
        }
        if (!isValidPhoneNumber(phoneNumber)) {
            std::cout << "Invalid phone number. Please enter only numbers.\n";
            phoneNumber.clear();
        }
    } while (phoneNumber.empty() || !isValidPhoneNumber(phoneNumber));

    do {
        if (userRole.empty()) {
            std::cout << "Enter role (user/admin): ";
            std::getline(std::cin, userRole);
        }
        if (!isValidRole(userRole)) {
            std::cout << "Invalid role. Please enter 'user' or 'admin'.\n";
            userRole.clear();
        }
    } while (userRole.empty() || !isValidRole(userRole));

    accountInfo newAcc(username, password, fullName, email, phoneNumber, userRole);
    std::string hashPass = newAcc.hashPassword(password);

    bool fileExists = std::filesystem::exists("list_account.csv");

    std::ofstream outfile("list_account.csv", std::ios::app);
    if (outfile.is_open()) {
        if (!fileExists) {
            outfile << "username,password,fullName,email,phoneNumber,role\n";
        }
        outfile << username << "," << hashPass << "," << fullName << "," << email << "," << phoneNumber << "," << userRole << "\n";
        outfile.close();
        std::cout << "Account saved to file successfully!\n";
    } else {
        std::cerr << "Error: Cannot open file for writing.\n";
    }

    std::cout << "Account created successfully!\n";
}



int main(){
    std::cout << "\n =====Reward wallet====\n";
    signUp("admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin");
    while (true){
        std::cout <<"1. Sign in\n 2. Sign up\n 3. Exit\n Chosse: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            signIn();
        } else if (choice == 2) {
            signUp();
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }

    }
    return 0;
}
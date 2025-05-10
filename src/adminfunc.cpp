#include "../include/adminfunc.h"

void userInformation(const std::string& userName){
    std::cout << "\n =====Information====\n";
    std::ifstream infile("list_account.csv");
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

        if (username == userName) {
            accountInfo acc(username, password, fullName, email, phone, role);
            acc.displayInfo();
            break;
        }
    }
}
void changePassword(const std::string& userName) {
    std::cout << "\n ===== Change your password =====\n";
    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Cannot open file!\n";
        return;
    }

    std::ostringstream updatedData;  
    std::string line;
    std::getline(infile, line); 
    updatedData << line << "\n";  

    bool found = false;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string username, password, fullName, email, phone, role, currPass, newPass, confirmPass, changePassFlag;
        std::getline(iss, username, ',');
        std::getline(iss, password, ',');
        std::getline(iss, fullName, ',');
        std::getline(iss, email, ',');
        std::getline(iss, phone, ',');
        std::getline(iss, role, ',');
        std::getline(iss, changePassFlag, ',');

        if (username == userName) {
            accountInfo acc(username, password, fullName, email, phone, role);

            std::cout << "Enter current password: ";
            std::getline(std::cin, currPass);

            if (acc.hashPassword(currPass) != password) {
                std::cout << "Incorrect current password!\n";
                updatedData << line << "\n"; 
                continue;
            }

            std::cout << "Enter new password: ";
            std::getline(std::cin, newPass);

            std::cout << "Confirm new password: ";
            std::getline(std::cin, confirmPass);

            if (newPass != confirmPass) {
                std::cout << "Passwords do not match!\n";
                updatedData << line << "\n";  
                continue;
            }

            acc.setPassword(newPass); 
            password = acc.getPassword();

            found = true;
            std::cout << "Password changed successfully!\n";
            changePassFlag = "0";
        }
        updatedData << username << "," << password << "," << fullName << "," 
                    << email << "," << phone << "," << role << "," << changePassFlag << "," << "\n";
    }
    infile.close();

    if (!found) {
        std::cout << "User not found!\n";
        return;
    }

    std::ofstream outfile("list_account.csv", std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Cannot write file!\n";
        return;
    }

    outfile << updatedData.str();
    outfile.close();
}

std::string randomPass(int length){
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string password;
    std::random_device rd;                         // random seed
    std::mt19937 gen(rd());                       // Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, chars.size() - 1);  // uniform distribution

    for (int i = 0; i < length; ++i) {
        password += chars[dist(gen)];
    }

    return password;
}

void createNewAcc(){
    std::cout << "\n =====Create a new account====\n";
    std::string role;
    std::cout << "Enter role (user/admin): ";
    std::getline(std::cin, role);
    std::cout << "Enter username: ";
    std::string username;
    std::getline(std::cin, username);
    std::string pass = randomPass();
    bool changePassFlag = true;
    signUp(username, pass , "", "", "", role, changePassFlag);
    std::cout<< "\nUsername: " << username <<"\nPassword: "<< pass <<"\n";
}

void userListInfo() {
    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Cannot open file!\n";
        return;
    }

    std::string line;
    bool isHeader = true;

    std::cout << "\n===== User List Information =====\n";

    while (std::getline(infile, line)) {
        if (isHeader) {
            std::cout << line << "\n"; 
            isHeader = false;
        } else {
            std::istringstream iss(line);
            std::string username, password, fullName, email, phone, role;
            std::getline(iss, username, ',');
            std::getline(iss, password, ',');
            std::getline(iss, fullName, ',');
            std::getline(iss, email, ',');
            std::getline(iss, phone, ',');
            std::getline(iss, role, ',');

            std::cout <<  username << "\t"<< fullName << "\t"
                      << email << "\t" << phone << "\t"
                      << role << "\n"
                      << "-----------------------------\n";
        }
    }

    infile.close();
}


void adminManagement(const std::string& userName){
    std::cout << "\n =====Create a new account====\n";
    while(true){
        std::cout << "\n =====Admin Management====\n";
        std::cout <<"1. Information\n2. Change your password\n3. Create a new user\n4. User list information\n5.Cancel\nChosse: ";
        int adminChoice;
        std::cin >> adminChoice;
        std::cin.ignore();
        switch (adminChoice) {
            case 1:
                userInformation(userName);
                break;

            case 2:
                changePassword(userName);
                break;

            case 3:
                createNewAcc();
                break;

            case 4:
                userListInfo();
                break;

            case 5:
                return; 

            default:
                std::cout << "Invalid choice.\n";
                break;
        }

    }
}
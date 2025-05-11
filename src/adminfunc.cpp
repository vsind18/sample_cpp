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

void editUserInfo(const std::string& username) {
    std::string inputPassword;
    std::cout << "Enter password to confirm your identity: ";
    std::getline(std::cin, inputPassword);

    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.\n";
        return;
    }

    std::ostringstream updatedData;
    std::string line;
    std::getline(infile, line); // skip header
    updatedData << line << "\n";

    bool found = false;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string usname, storedHash, fullName, email, phone, role, changePassFlagStr;

        std::getline(iss, usname, ',');
        std::getline(iss, storedHash, ',');
        std::getline(iss, fullName, ',');
        std::getline(iss, email, ',');
        std::getline(iss, phone, ',');
        std::getline(iss, role, ',');
        std::getline(iss, changePassFlagStr, ',');

        if (username == usname) {
            // Hash the input password
            accountInfo tmpAcc(username, inputPassword, fullName, email, phone, role);
            std::string hashPass = tmpAcc.hashPassword(inputPassword);

            if (storedHash == hashPass) {
                std::cout << "Confirmed edit information!!\n";
                accountInfo acc(usname, inputPassword, fullName, email, phone, role);

                if (role == "admin") {
                    std::cout << "Editing admin's personal information.\n";
                    std::string newUsername, newFullName, newEmail, newPhone;
                    std::cout << "Enter new username: ";
                    std::getline(std::cin, newUsername);
                    std::cout << "Enter new full name: ";
                    std::getline(std::cin, newFullName);
                    std::cout << "Enter new email: ";
                    std::getline(std::cin, newEmail);
                    std::cout << "Enter new phone number: ";
                    std::getline(std::cin, newPhone);

                    acc.setUsername(newUsername);
                    acc.setFullName(newFullName);
                    acc.setEmail(newEmail);
                    acc.setPhoneNumber(newPhone);

                    std::cout << "Admin information is changed successfully!\n";
                } else {
                    std::cout << "Editing your personal information.\n";
                    std::string newUserName, newFullName;
                    std::cout << "Enter new username: ";
                    std::getline(std::cin, newUserName);
                    std::cout << "Enter new full name: ";
                    std::getline(std::cin, newFullName);

                    acc.setUsername(newUserName);
                    acc.setFullName(newFullName);

                    std::cout << "User information is changed successfully!\n";
                }

                // Ghi dòng đã cập nhật
                updatedData << acc.getUsername() << "," << acc.hashPassword(acc.getPassword()) << ","
                            << acc.getFullname() << "," << acc.getEmail() << "," << acc.getPhoneNumber() << ","
                            << acc.getRole() << "," << changePassFlagStr << "\n";
                found = true;
                continue; // bỏ qua ghi lại dòng cũ
            }
        }

        // Ghi lại dòng cũ nếu không phải tài khoản được chỉnh sửa
        updatedData << usname << "," << storedHash << "," << fullName << "," 
                    << email << "," << phone << "," << role << "," << changePassFlagStr << "\n";
    }

    infile.close();

    if (!found) {
        std::cout << "User not found or incorrect password!\n";
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

void adminEditUser() {
    std::string targetUsername;
    std::cout << "Enter the username of the user to edit: ";
    std::getline(std::cin, targetUsername);

    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.\n";
        return;
    }

    std::ostringstream updatedData;
    std::string line;
    std::getline(infile, line); // header
    updatedData << line << "\n";

    bool found = false;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string usname, storedHash, fullName, email, phone, role, changePassFlagStr;

        std::getline(iss, usname, ',');
        std::getline(iss, storedHash, ',');
        std::getline(iss, fullName, ',');
        std::getline(iss, email, ',');
        std::getline(iss, phone, ',');
        std::getline(iss, role, ',');
        std::getline(iss, changePassFlagStr, ',');

        if (usname == targetUsername) {
            std::cout << "Editing information for user: " << usname << "\n";
            std::string newFullName, newEmail, newPhone;

            std::cout << "Enter new full name: ";
            std::getline(std::cin, newFullName);
            std::cout << "Enter new email: ";
            std::getline(std::cin, newEmail);
            std::cout << "Enter new phone number: ";
            std::getline(std::cin, newPhone);

            updatedData << usname << "," << storedHash << "," << newFullName << ","
                        << newEmail << "," << newPhone << "," << role << "," << changePassFlagStr << "\n";
            found = true;
        } else {
            updatedData << usname << "," << storedHash << "," << fullName << ","
                        << email << "," << phone << "," << role << "," << changePassFlagStr << "\n";
        }
    }

    infile.close();

    if (!found) {
        std::cout << "User not found.\n";
        return;
    }

    std::ofstream outfile("list_account.csv", std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Cannot write file.\n";
        return;
    }

    outfile << updatedData.str();
    outfile.close();

    std::cout << "User information updated successfully.\n";
}


void adminManagement(const std::string& userName){
    std::cout << "\n =====Create a new account====\n";
    while(true){
        std::cout << "\n =====Admin Management====\n";
        std::cout <<"1. Manage your information\n2. Manage users information\n3. Cancel\nChosse: ";
        int adminChoice;
        std::cin >> adminChoice;
        std::cin.ignore();

        int case1choice,case2choice;

        switch (adminChoice) {
            case 1:
                while(case1choice != 4){
                    std::cout << "\n =====Manage your information====\n";
                    std::cout <<"1. View your information\n2. Edit your information\n3. Change your password\n4. Cancel\nChoose: ";
                    std::cin >> case1choice;
                    std::cin.ignore();
                    if (case1choice == 1){
                        userInformation(userName);
                    } else if (case1choice == 2) {
                        editUserInfo(userName);
                        std::cout << "Must sign in again!\n";
                        return;
                    } else if (case1choice == 3){
                        changePassword(userName);
                    } 
                }
                break;
            case 2:
                while(case2choice != 4){
                    std::cout << "\n =====Manage users information====\n";
                    std::cout <<"1. View your information\n2. Edit user information\n3. Create a new account\n4. Cancel\nChoose: ";
                    std::cin >> case2choice;
                    std::cin.ignore();
                    if (case2choice == 1){
                        userListInfo();
                    } else if (case2choice == 2) {
                        adminEditUser();

                    } else if (case2choice == 3) {
                        createNewAcc();
                    }
                }
                break;
            
            case 3:
    
                return;

            default:
                std::cout << "Invalid choice.\n";
                break;
        }

    }
}
#include "../include/adminfunc.h"

void userInformation(const std::string& userName){
    std::cout << "\n =====Information====\n";
                {
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
}

void adminManagement(const std::string& userName){
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
                std::cout << "\n =====Change your password====\n";
                break;

            case 3:
                std::cout << "\n =====Create a new account====\n";
                break;

            case 4:
                std::cout << "\n =====User list information====\n";
                break;

            case 5:
                return; 

            default:
                std::cout << "Invalid choice.\n";
                break;
        }

    }
}
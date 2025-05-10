#include "include/account.h"
#include "include/adminfunc.h"


/*  g++ -o main main.cpp src/account.cpp src/adminfunc.cpp -lssl -lcrypto */

int main(){
    std::cout << "\n =====Reward wallet====\n";
    signUp("admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin", false);
    while (true){
        std::cout <<"1. Sign in\n2. Sign up\n3. Exit\nChosse: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            auto [userName, userRole] = signIn();
            if (userRole == "admin"){
                    adminManagement(userName);
            }
        } else if (choice == 2) {
            signUp("", "", "", "" ,"", "", false);
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }

    }
    return 0;
}
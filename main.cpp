#include "include/account.h"
#define ADMIN_ID_DEFAULT "1"

/*  g++ -o main main.cpp src/account.cpp -lssl -lcrypto */
int loginMenu(){
    //lựa chọn 1 signin 2signup 3cancel

    return 0;
}

int main(){
    //default admin
    accountInfo admin(ADMIN_ID_DEFAULT, "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin");
    int result_admin = admin.signUp(ADMIN_ID_DEFAULT, "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin","admin", false);

    accountInfo acc("", "","", "","", "","");

    while (true){
        if (loginMenu() == 1){
            std::string username, password;

            int result_signin = acc.signIn(username, password);
        } else if (loginMenu() == 2){
            std::string uID, username, password, fullname, email, phone, role;
            
            uID = getNextUIDFromCSV();
            
            int result = acc.signUp(uID, username, password, fullname, email, phone, role, "user", false);
        } else {
            return -1;
        }


    }
    return 0;
}
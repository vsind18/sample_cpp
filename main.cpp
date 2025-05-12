#include "include/account.h"
#define ADMIN_ID_DEFAULT "1"

/*  g++ -o main main.cpp src/account.cpp -lssl -lcrypto */

int loginMenu(){
    //return 1; //khi chon sign in
    //return 2; //khi chon sign up
    //return 3; //khi cancel
}


int main(){
    //default admin
    accountInfo admin("1", "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin");
    admin.signUp(ADMIN_ID_DEFAULT, "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin");


    while (true){
        
    }
    return 0;
}
#include "include/ui.h"
#include "include/user_service.h"
#include "include/wallet_service.h"
#include <iostream>

// g++ -o main main.cpp src/otp.cpp src/ui.cpp src/user_service.cpp src/wallet_service.cpp src/models/transaction.cpp src/models/user.cpp src/models/wallet.cpp  -lssl -lcrypto

void initializeSystem()
{
    // Tạo tài khoản admin mặc định nếu chưa tồn tại
    if (!UserService::userExists("admin"))
    {
        User adminUser("admin", UserService::hashPassword("admin2025"), "admin", "admin");
        UserService::saveUser(adminUser);
        std::cout << "Default admin account created.\n";
    }

    // Tạo ví tổng nếu chưa có
    Wallet masterWallet;
    if (!WalletService::loadWallet("master_wallet", masterWallet))
    {
        WalletService::saveWallet(Wallet("master_wallet", 5000.0));
        std::cout << "Master wallet initialized with 5000 points.\n";
    }
}

int main()
{
    initializeSystem();
    UI::showMainMenu();
    return 0;
}

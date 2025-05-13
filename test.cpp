#include "include/account.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define ADMIN_ID_DEFAULT "1"

// Hàm kiểm tra đăng nhập từ file CSV
void testSignIn(const std::string& filePath) {
    // Mở file input
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Bỏ qua dòng tiêu đề

    // Đọc từng dòng trong file và thử đăng nhập
    while (std::getline(file, line)) {
        std::cout << "Read line: " << line << "\n";
        std::istringstream ss(line);
        std::string uID, username, password, fullname, email, phone, role;

        // Đọc thông tin từ dòng CSV
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, fullname, ',');
        std::getline(ss, email, ',');
        std::getline(ss, phone, ',');

        // Tạo đối tượng tài khoản và gọi hàm signIn
        accountInfo acc(uID, username, password, fullname, email, phone, role);

        int result = acc.signIn(username, password);

        // In kết quả đăng nhập
        std::cout << "SignIn for user '" << username << "': ";
        if (result == 0) {
            std::cout << "Success\n";
        } else if (result == -1) {
            std::cout << "Failed: Incorrect username or password\n";
        } else if (result == -2) {
            std::cout << "Failed: Could not open file\n";
        }
    }

    file.close();
}
// Hàm kiểm tra đăng ký từ file CSV
void testSignUp(const std::string& filePath) {
    // Khởi tạo admin mặc định
    accountInfo admin(ADMIN_ID_DEFAULT, "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin");
    int result_admin = admin.signUp(ADMIN_ID_DEFAULT, "admin", "admin2025", "admin", "admin@gmail.com", "0931654687", "admin", "admin", false);

    // Mở file input
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Bỏ dòng tiêu đề

    // Đọc từng dòng trong file và thử đăng ký
    while (std::getline(file, line)) {
        std::cout << "Read line: " << line << "\n";
        std::istringstream ss(line);
        std::string uID, username, password, fullname, email, phone, role;

        // Đọc thông tin từ dòng CSV
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, fullname, ',');
        std::getline(ss, email, ',');
        std::getline(ss, phone, ',');

        // Tạo uID mới
        uID = getNextUIDFromCSV();

        // Tạo đối tượng tài khoản và gọi hàm signUp
        accountInfo acc("", "", "", "", "", "", "");
        int result = acc.signUp(uID, username, password, fullname, email, phone, role, "user", false);

        // In kết quả đăng ký
        std::cout << "Signup for user '" << username << "': ";
        if (result == 0) {
            std::cout << "Success\n";
        } else {
            std::cout << "Failed (code " << result << ")\n";
        }
    }

    file.close();
}

int main() {
    // Gọi function testSignUp và truyền đường dẫn tới file CSV
    testSignIn("input_signup.csv");
    testSignUp("input_signup.csv");

    return 0;
}

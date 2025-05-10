#include "../include/account.h"
#include "../include/adminfunc.h"

std::string accountInfo::hashPassword(const std::string& pwd) {
    // Tạo context cho việc tính toán SHA256
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == nullptr) {
        return "";  // Xử lý lỗi nếu không tạo được context
    }

    // Tạo SHA256 context
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";  // Xử lý lỗi nếu không khởi tạo SHA256
    }

    // Cập nhật với chuỗi password
    if (EVP_DigestUpdate(ctx, pwd.c_str(), pwd.size()) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";  // Xử lý lỗi nếu không cập nhật được chuỗi
    }

    // Lấy kết quả hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";  // Xử lý lỗi nếu không lấy được kết quả hash
    }

    // Giải phóng context
    EVP_MD_CTX_free(ctx);

    // Chuyển kết quả hash thành chuỗi hex
    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

accountInfo::accountInfo(const std::string &uname, const std::string& pwd,
                const std::string &fname, const std::string &e,
                const std::string &phone, const std::string &r)
                :username(uname), password(pwd), fullName(fname), 
                email(e), phoneNumber(phone), role(r) {}

void accountInfo::displayInfo() const {
    std::cout << "Username: " << username << "\n"
              << "Full Name: " << fullName << "\n"
              << "Email: " << email << "\n"
              << "Phone: " << phoneNumber << "\n"
              << "Role: " << role << "\n";
}

std::string accountInfo::getPassword(){
    return password;
}

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

void accountInfo::setUsername(const std::string& uname) { username = uname; }
void accountInfo::setPassword(const std::string& pwd) { password = hashPassword(pwd); }
void accountInfo::setFullName(const std::string& fname) { fullName = fname; }
void accountInfo::setEmail(const std::string& e) { email = e; }
void accountInfo::setPhoneNumber(const std::string& phone) { phoneNumber = phone; }
void accountInfo::setRole(const std::string& r) { role = r; }

std::pair<std::string, std::string> signIn() {
    std::cout << "\n===== Sign in =====\n";
    std::string usname, pwd;
    std::cout << "Username: ";
    std::getline(std::cin, usname);
    std::cout << "Password: ";
    std::getline(std::cin, pwd);

    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.\n";
        return {"", ""};
    }

    std::string line;
    std::getline(infile, line); // skip header

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string username, storedHash, fullName, email, phone, role, changePassFlagStr;

        std::getline(iss, username, ',');
        std::getline(iss, storedHash, ',');
        std::getline(iss, fullName, ',');
        std::getline(iss, email, ',');
        std::getline(iss, phone, ',');
        std::getline(iss, role, ',');
        std::getline(iss, changePassFlagStr, ',');

        // Băm mật khẩu người dùng nhập vào
        accountInfo tmpAcc(username, pwd, fullName, email, phone, role);
        std::string hashPass = tmpAcc.hashPassword(pwd);

        // So sánh username và hash mật khẩu
        if (username == usname && storedHash == hashPass) {
            std::cout << "Login successful! Role: " << role << "\n";
            if (changePassFlagStr == "1"){
                changePassword(username);
        }
            return {username, role};
        }
    }

    std::cout << "Invalid username or password.\n";
    return {"", ""};
}

void signUp(const std::string& uname , const std::string& pwd , 
            const std::string& fname , const std::string& mail , 
            const std::string& phone , const std::string& role ,
            bool changePassFlag) {
    std::string username = uname, password = pwd, fullName = fname, email = mail, phoneNumber = phone, userRole = role;
    bool flag = changePassFlag;

    if (username.empty()) {
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
    }
    if (password.empty() && role != "admin") {
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

    if (userRole != "admin") {
        userRole = "user";
        std::cout << "set default user role\n";
    }

    bool fileExists = std::filesystem::exists("list_account.csv");
    bool adminExists = false;
    bool usernameExists = false;
    std::ostringstream updatedData;

    if (fileExists) {
        std::ifstream infile("list_account.csv");
        std::string line;
        std::getline(infile, line); // header
        updatedData << line << "\n";

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string existingUsername;
            std::getline(iss, existingUsername, ',');

            if (existingUsername == "admin") {
                adminExists = true;
            }
            if (existingUsername == username) {
                usernameExists = true;
            }
            updatedData << line << "\n";
        }
        infile.close();
    } else {
        updatedData << "username,password,fullName,email,phoneNumber,role,changePasswordFlag\n";
    }

    if (username == "admin" && adminExists) {
        std::cout << "Admin account already exists. Skipping creation.\n";
        return;
    }

    if (usernameExists) {
        std::cout << "Username already exists. Please choose another username.\n";
        return;
    }

    accountInfo newAcc(username, password, fullName, email, phoneNumber, userRole);
    std::string hashPass = newAcc.hashPassword(password);
    updatedData << username << "," << hashPass << "," << fullName << "," << email << "," << phoneNumber << "," << userRole << "," << flag << "," << "\n";
    std::ofstream outfile("list_account.csv", std::ios::trunc);
    if (outfile.is_open()) {
        outfile << updatedData.str();
        outfile.close();
        std::cout << "Account saved to file successfully!\n";
    } else {
        std::cerr << "Error: Cannot open file for writing.\n";
    }

    std::cout << "Account created successfully!\n";
}

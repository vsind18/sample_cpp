#include "../include/account.h"

std::string accountInfo::hashPassword(const std::string& pwd) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == nullptr) {
        return "";
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        return "";
    }
    if (EVP_DigestUpdate(ctx, pwd.c_str(), pwd.size()) != 1) {
        EVP_MD_CTX_free(ctx);
        return ""; 
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

accountInfo::accountInfo(const std::string &uID, const std::string &uname, const std::string& pwd,
                const std::string &fname, const std::string &e,
                const std::string &phone, const std::string &r)
                :username(uname), password(pwd), fullName(fname), 
                email(e), phoneNumber(phone), role(r) {}

int accountInfo::signIn(const std::string uName, const std::string pwd){
    std::ifstream infile("list_account.csv");
    if (!infile.is_open()) {
        return -1;  // Trả về -1 nếu không thể mở file
    }

    std::string line;
    std::getline(infile, line); // Bỏ qua dòng tiêu đề

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

        accountInfo tmpAcc("userID", username, pwd, fullName, email, phone, role);
        std::string hashPass = tmpAcc.hashPassword(pwd); 

        if (username == uName && storedHash == hashPass) {
            // Nếu đăng nhập thành công
            return 0;  // Trả về 0 nếu đăng nhập thành công
        }
    }
}

int accountInfo::signUp(const std::string &uID, const std::string& uname, const std::string& pwd, 
                        const std::string& fname, const std::string& mail, 
                        const std::string& phone, const std::string& role,
                        bool isChangePass) {

    bool fileExists = std::filesystem::exists("list_account.csv");
    bool adminExists = false;
    bool usernameExists = false;
    std::ostringstream updatedData;

    if (fileExists) {
        std::ifstream infile("list_account.csv");
        std::string line;
        std::getline(infile, line); 
        updatedData << line << "\n"; 

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string existingUsername;
            std::getline(iss, existingUsername, ',');  // Lấy tên người dùng

            if (existingUsername == "admin") {
                adminExists = true;
            }

            if (existingUsername == uname) {
                usernameExists = true;
            }

            updatedData << line << "\n"; 
        }
        infile.close();
    } else {
        updatedData << "userid,username,password,fullName,email,phoneNumber,role,ischangepassword\n";
    }

    if (uname == "admin" && adminExists) {
        return -2;  // Trả về -2 nếu đã có admin
    }

    if (usernameExists) {
        std::cout << "Username already exists. Please choose another username.\n";
        return -3;  // Trả về -3 nếu tên đăng nhập đã có
    }

    accountInfo newAcc(uID, uname, pwd, fname, mail, phone, role);
    std::string hashPass = newAcc.hashPassword(pwd); 
    updatedData << uname << "," << hashPass << "," << fname << "," << mail << "," 
                << phone << "," << role << "," << isChangePass << "\n";

    std::ofstream outfile("list_account.csv", std::ios::trunc);
    if (outfile.is_open()) {
        outfile << updatedData.str(); 
        outfile.close();
        return 0;  
    } else {
        return -1;  // Trả về -1 nếu không thể mở file
    }
}

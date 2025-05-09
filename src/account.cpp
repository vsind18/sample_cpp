#include "../include/account.h"

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

#include "../../include/models/user.h"

User::User(const std::string &username, const std::string &hashedPassword, const std::string &fullName, const std::string &role)
    : username(username), hashedPassword(hashedPassword), fullName(fullName), role(role) {}

const std::string &User::getUsername() const { return username; }
const std::string &User::getHashedPassword() const { return hashedPassword; }
const std::string &User::getFullName() const { return fullName; }
const std::string &User::getRole() const { return role; }

void User::setFullName(const std::string &name) { fullName = name; }
void User::setHashedPassword(const std::string &hashed) { hashedPassword = hashed; }

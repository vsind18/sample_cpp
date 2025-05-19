#include "../../include/models/user.h"

User::User() : username(""), hashedPassword(""), fullName(""), role("user"), mustChangePassword(false) {}

User::User(const std::string &username, const std::string &hashedPassword,
           const std::string &fullName, const std::string &role, bool mustChangePassword)
    : username(username), hashedPassword(hashedPassword), fullName(fullName),
      role(role), mustChangePassword(mustChangePassword) {}

std::string User::getUsername() const { return username; }
std::string User::getHashedPassword() const { return hashedPassword; }
std::string User::getFullName() const { return fullName; }
std::string User::getRole() const { return role; }
bool User::getMustChangePassword() const { return mustChangePassword; }

void User::setPasswordHash(const std::string &newHash) { hashedPassword = newHash; }
void User::setFullName(const std::string &newName) { fullName = newName; }
void User::setRole(const std::string &newRole) { role = newRole; }
void User::setMustChangePassword(bool val) { mustChangePassword = val; }

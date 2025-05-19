#pragma once
#include <string>

class User
{
private:
  std::string username;
  std::string hashedPassword;
  std::string fullName;
  std::string role;
  bool mustChangePassword;

public:
  User();
  User(const std::string &username, const std::string &hashedPassword, const std::string &fullName,
       const std::string &role, bool mustChangePassword = false);

  std::string getUsername() const;
  std::string getHashedPassword() const;
  std::string getFullName() const;
  std::string getRole() const;
  bool getMustChangePassword() const;

  void setPasswordHash(const std::string &newHash);
  void setFullName(const std::string &newName);
  void setRole(const std::string &newRole);
  void setMustChangePassword(bool val);
};

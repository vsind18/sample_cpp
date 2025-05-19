#pragma once
#include <string>

class User
{
public:
  User(const std::string &username, const std::string &hashedPassword, const std::string &fullName, const std::string &role);

  const std::string &getUsername() const;
  const std::string &getHashedPassword() const;
  const std::string &getFullName() const;
  const std::string &getRole() const;

  void setFullName(const std::string &name);
  void setHashedPassword(const std::string &hashed);

private:
  std::string username;
  std::string hashedPassword;
  std::string fullName;
  std::string role; // "user" hoặc "admin"
};

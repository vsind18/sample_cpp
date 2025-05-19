#include "../include/user_service.h"
#include "../include/wallet_service.h"
#include "../include/config.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <iostream>

const std::string USER_DB = USER_DB_PATH;

std::string hash(const std::string &input)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)input.c_str(), input.size(), hash);
  std::stringstream ss;
  for (unsigned char byte : hash)
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
  return ss.str();
}

namespace UserService
{
  bool userExists(const std::string &username)
  {
    std::ifstream file(USER_DB);
    std::string line;
    while (getline(file, line))
    {
      std::stringstream ss(line);
      std::string uname;
      ss >> uname;
      if (uname == username)
      {
        return true;
      }
    }
    return false;
  }

  bool saveUser(const User &user)
  {
    std::ofstream file("data/users.db", std::ios::app);
    if (!file)
      return false;
    file << user.getUsername() << "," << user.getHashedPassword() << "," << user.getFullName() << "," << user.getRole() << "\n";
    return true;
  }

  bool loadAllUsers(std::vector<User> &users)
  {
    users.clear();
    std::ifstream file("data/users.db");
    std::string line;
    while (std::getline(file, line))
    {
      std::stringstream ss(line);
      std::string username, hash, fullname, role;
      std::getline(ss, username, ',');
      std::getline(ss, hash, ',');
      std::getline(ss, fullname, ',');
      std::getline(ss, role, ',');
      users.emplace_back(username, hash, fullname, role);
    }
    return true;
  }

  bool findUserByUsername(const std::string &username, User &foundUser)
  {
    std::vector<User> users;
    loadAllUsers(users);
    for (auto &user : users)
    {
      if (user.getUsername() == username)
      {
        foundUser = user;
        return true;
      }
    }
    return false;
  }

  bool registerUser(User &outUser, bool isAdmin)
  {
    std::string fullName, username, password, role;
    std::cout << "Full name: ";
    std::cin.ignore();
    std::getline(std::cin, fullName);
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (isAdmin)
    {
      std::cout << "Role (user/admin): ";
      std::cin >> role;
    }
    else
    {
      role = "user";
    }

    std::string hashed = hash(password);
    User user(username, hashed, fullName, role);
    outUser = user;

    if (!saveUser(user))
      return false;

    if (role == "user")
    {
      // Tạo ví cho user và chuyển 100 điểm từ ví tổng
      WalletService::createWalletForUser(user.getUsername());
      if (!WalletService::transferPoints("master_wallet", user.getUsername(), 100.0))
      {
        std::cerr << "Failed to transfer 100 points from master wallet.\n";
      }
    }

    return true;
  }

  bool loginUser(User &userOut)
  {
    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    User tempUser("", "", "", "");
    if (findUserByUsername(username, tempUser))
    {
      if (hash(password) == tempUser.getHashedPassword())
      {
        userOut = tempUser;
        return true;
      }
    }
    return false;
  }

  std::string hashPassword(const std::string &password)
  {
    return hash(password);
  }
}
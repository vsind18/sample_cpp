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
    // Load all users
    std::vector<User> users;
    loadAllUsers(users);

    // Overwrite or append
    bool updated = false;
    for (auto &u : users)
    {
      if (u.getUsername() == user.getUsername())
      {
        u = user;
        updated = true;
        break;
      }
    }
    if (!updated)
      users.push_back(user);

    std::ofstream file(USER_DB, std::ios::trunc);
    for (const auto &u : users)
    {
      file << u.getUsername() << ","
           << u.getHashedPassword() << ","
           << u.getFullName() << ","
           << u.getRole() << ","
           << u.getMustChangePassword() << "\n";
    }
    return true;
  }

  bool loadAllUsers(std::vector<User> &users)
  {
    users.clear();
    std::ifstream file(USER_DB);
    std::string line;
    while (std::getline(file, line))
    {
      std::stringstream ss(line);
      std::string username, hash, fullname, role, mustChangeStr;
      std::getline(ss, username, ',');
      std::getline(ss, hash, ',');
      std::getline(ss, fullname, ',');
      std::getline(ss, role, ',');
      std::getline(ss, mustChangeStr, ',');

      bool mustChange = (mustChangeStr == "1" || mustChangeStr == "true");
      users.emplace_back(username, hash, fullname, role, mustChange);
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

    if (isAdmin)
    {
      std::cout << "Role (user/admin): ";
      std::cin >> role;

      if (role == "user")
      {
        password = "init123";
        std::cout << "[INFO] Default password for new user is: init123\n";
      }
      else
      {
        std::cout << "Password: ";
        std::cin >> password;
      }
    }
    else
    {
      role = "user";
      std::cout << "Password: ";
      std::cin >> password;
    }

    std::string hashed = hash(password);
    bool mustChange = isAdmin && role == "user";
    User user(username, hashed, fullName, role, mustChange);
    outUser = user;

    saveUser(user);

    if (role == "user")
    {
      WalletService::createWalletForUser(username);
      WalletService::transferPoints("master_wallet", username, 100.0);
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

    User tempUser;
    if (findUserByUsername(username, tempUser))
    {
      if (hash(password) == tempUser.getHashedPassword())
      {
        if (tempUser.getMustChangePassword())
        {
          std::string newPass;
          std::cout << "First-time login detected. You must change your password.\n";
          std::cout << "New password: ";
          std::cin >> newPass;

          tempUser.setPasswordHash(hash(newPass));
          tempUser.setMustChangePassword(false);
          saveUser(tempUser);

          std::cout << "Password changed successfully.\n";
        }

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
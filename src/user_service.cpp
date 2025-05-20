#include "../include/user_service.h"
#include "../include/wallet_service.h"
#include "../include/config.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include <filesystem>

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
  void backup()
  {
    std::filesystem::create_directory("backup");

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&now_time);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", tm);

    std::string backupFilename = "backup/users_" + std::string(buffer) + ".db";

    std::ifstream src(USER_DB, std::ios::binary);
    std::ofstream dst(backupFilename, std::ios::binary);
    dst << src.rdbuf();

    std::cout << "[INFO] Backup saved to: " << backupFilename << "\n";
  }

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

  std::string randomPass(int length){
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string password;
    std::random_device rd;                         // random seed
    std::mt19937 gen(rd());                       // Mersenne Twister engine
    std::uniform_int_distribution<> dist(0, chars.size() - 1);  // uniform distribution

    for (int i = 0; i < length; ++i) {
        password += chars[dist(gen)];
    }

    return password;
  }

  bool registerUser(User &outUser, bool isAdmin)
  {
    std::string fullName, username, password, role;
    std::cout << "\n--- SIGN UP ---\n";
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
        password = randomPass(10);
        std::cout << "[INFO] Password for new user is: " << password << " \n";
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

    backup();
  

    if (role == "user")
    {
      WalletService::createWalletForUser(username);
      WalletService::transferPoints("master_wallet", username, 100.0);
    }

    return true;
  }

  bool loginUser(User &userOut)
  {
    std::cout << "\n--- SIGN IN ---\n";
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
          backup();
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
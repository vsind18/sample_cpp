#pragma once
#include "../include/models/user.h"
#include <vector>

namespace UserService
{
  bool userExists(const std::string &username);
  bool registerUser(User &outUser, bool isAdmin = false);
  bool loginUser(User &userOut);
  bool saveUser(const User &user);
  bool loadAllUsers(std::vector<User> &users);
  bool findUserByUsername(const std::string &username, User &foundUser);

  std::string hashPassword(const std::string &password);
  std::string randomPassword(int length = 10);
  void backup();
}

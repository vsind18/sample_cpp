#pragma once
#include "../include/models/user.h"

namespace UI
{
  void showMainMenu();
  void showLoginMenu();
  void showUserMenu(User &user);
  void showAdminMenu(User &admin);

  // Thông tin cá nhân
  void showManagerInfoMenu(User &user, bool isAdminEdit = false);

  // Quản lý ví
  void showWalletMenu(User &user);

  // Quản lý người dùng (admin)
  void showUserManagerMenu(User &admin);

  // Tiện ích
  void alert(const std::string &message);
}

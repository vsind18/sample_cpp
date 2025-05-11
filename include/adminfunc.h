#ifndef ADMINFUNC_H
#define ADMINFUNC_H

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>

#include "account.h"

void changePassword(const std::string& userName);
void userInformation(const std::string& userName);
void userListInformation();
void editUserInfo();

std::string randomPass(int length = 10);
void createNewAcc();
void userListInfo();
void editUserInfo(const std::string& username);
void adminEditUser();

void adminManagement(const std::string& userName);

#endif //ADMINFUNC_H
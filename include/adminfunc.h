#ifndef ADMINFUNC_H
#define ADMINFUNC_H

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "account.h"

std::string changePassword();
void userInformation();
void adminManagement(const std::string& userName);

#endif //ADMINFUNC_H
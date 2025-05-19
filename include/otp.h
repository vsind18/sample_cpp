#pragma once
#include <string>

namespace OTP
{
  std::string generateOTP();
  bool verifyOTP(const std::string &actual, const std::string &input);
}

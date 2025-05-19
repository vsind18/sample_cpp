#include "../include/otp.h"
#include <cstdlib>
#include <ctime>

namespace OTP
{
  std::string generateOTP()
  {
    srand(time(nullptr));
    int otp = 100000 + rand() % 900000;
    return std::to_string(otp);
  }

  bool verifyOTP(const std::string &actual, const std::string &input)
  {
    return actual == input;
  }
}

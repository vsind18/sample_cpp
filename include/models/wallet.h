#pragma once
#include <string>

class Wallet
{
public:
  Wallet();
  Wallet(const std::string &ownerUsername, double balance = 0.0);

  std::string getOwner() const;
  double getBalance() const;

  void setBalance(double newBalance);

private:
  std::string ownerUsername;
  double balance;
};

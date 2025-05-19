#pragma once
#include <string>
#include <ctime>

class Transaction
{
public:
  Transaction(const std::string &from, const std::string &to, double amount, time_t timestamp);

  std::string getFrom() const;
  std::string getTo() const;
  double getAmount() const;
  time_t getTimestamp() const;

private:
  std::string fromWallet;
  std::string toWallet;
  double amount;
  time_t timestamp;
};

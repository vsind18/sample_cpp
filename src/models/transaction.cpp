#include "../../include/models/transaction.h"

Transaction::Transaction(const std::string &from, const std::string &to, double amount, time_t timestamp)
    : fromWallet(from), toWallet(to), amount(amount), timestamp(timestamp) {}

std::string Transaction::getFrom() const
{
  return fromWallet;
}

std::string Transaction::getTo() const
{
  return toWallet;
}

double Transaction::getAmount() const
{
  return amount;
}

time_t Transaction::getTimestamp() const
{
  return timestamp;
}

#include "../../include/models/wallet.h"

Wallet::Wallet() : ownerUsername(""), balance(0.0) {}

Wallet::Wallet(const std::string &ownerUsername, double balance)
    : ownerUsername(ownerUsername), balance(balance) {}

std::string Wallet::getOwner() const
{
  return ownerUsername;
}

double Wallet::getBalance() const
{
  return balance;
}

void Wallet::setBalance(double newBalance)
{
  balance = newBalance;
}

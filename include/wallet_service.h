#pragma once
#include "models/wallet.h"
#include "models/transaction.h"
#include <vector>

namespace WalletService
{
  bool createWalletForUser(const std::string &username);
  bool loadWallet(const std::string &username, Wallet &wallet);
  bool saveWallet(const Wallet &wallet);
  bool transferPoints(const std::string &fromUser, const std::string &toUser, double amount);

  bool saveTransaction(const Transaction &tx);
  bool getTransactionHistory(const std::string &username, std::vector<Transaction> &history);
}

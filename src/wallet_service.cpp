#include "../include/wallet_service.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>

std::string walletFile = "data/wallets.db";
std::string txFile = "data/transactions.db";

namespace WalletService
{

  bool createWalletForUser(const std::string &username)
  {
    Wallet wallet(username, 0.0);
    return saveWallet(wallet);
  }

  bool loadWallet(const std::string &username, Wallet &walletOut)
  {
    std::ifstream file(walletFile);
    std::string line;
    while (getline(file, line))
    {
      std::stringstream ss(line);
      std::string name;
      double balance;
      ss >> name >> balance;
      if (name == username)
      {
        walletOut = Wallet(name, balance);
        return true;
      }
    }
    return false;
  }

  bool saveWallet(const Wallet &wallet)
  {
    std::vector<Wallet> wallets;
    Wallet temp("", 0);
    std::ifstream file(walletFile);
    std::string line;
    while (getline(file, line))
    {
      std::stringstream ss(line);
      std::string name;
      double balance;
      ss >> name >> balance;
      wallets.emplace_back(name, balance);
    }
    file.close();

    // update or append
    bool found = false;
    for (auto &w : wallets)
    {
      if (w.getOwner() == wallet.getOwner())
      {
        w.setBalance(wallet.getBalance());
        found = true;
        break;
      }
    }
    if (!found)
      wallets.push_back(wallet);

    std::ofstream out(walletFile);
    for (auto &w : wallets)
    {
      out << w.getOwner() << " " << w.getBalance() << "\n";
    }
    return true;
  }

  bool saveTransaction(const Transaction &tx)
  {
    std::ofstream file(txFile, std::ios::app);
    file << tx.getFrom() << " " << tx.getTo() << " " << tx.getAmount() << " " << tx.getTimestamp() << "\n";
    return true;
  }

  bool getTransactionHistory(const std::string &username, std::vector<Transaction> &history)
  {
    std::ifstream file(txFile);
    std::string from, to;
    double amount;
    time_t ts;
    while (file >> from >> to >> amount >> ts)
    {
      if (from == username || to == username)
      {
        history.emplace_back(from, to, amount, ts);
      }
    }
    return true;
  }

  bool transferPoints(const std::string &fromUser, const std::string &toUser, double amount)
  {
    if (amount <= 0)
      return false;

    Wallet fromWallet("", 0), toWallet("", 0);
    if (!loadWallet(fromUser, fromWallet))
    {
      std::cerr << "Sender wallet not found.\n";
      return false;
    }
    if (!loadWallet(toUser, toWallet))
    {
      std::cerr << "Recipient wallet not found.\n";
      return false;
    }

    if (fromWallet.getBalance() < amount)
    {
      std::cerr << "Insufficient funds.\n";
      return false;
    }

    // Atomic update
    fromWallet.setBalance(fromWallet.getBalance() - amount);
    toWallet.setBalance(toWallet.getBalance() + amount);

    saveWallet(fromWallet);
    saveWallet(toWallet);

    Transaction tx(fromUser, toUser, amount, time(nullptr));
    saveTransaction(tx);

    return true;
  }

}

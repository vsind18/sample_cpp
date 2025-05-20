#include "../include/ui.h"
#include "../include/user_service.h"
#include "../include/otp.h"
#include "../include/wallet_service.h"
#include <iostream>
#include <limits>

using namespace std;

template <typename T>
T safeInput(const string &prompt = "Choice: ")
{
  T value;
  while (true)
  {
    cout << prompt;
    cin >> value;

    if (cin.fail())
    {
      cin.clear();                                         // Xóa trạng thái lỗi
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa dữ liệu sai
      cout << "[ERROR] Invalid input. Please try again.\n";
    }
    else
    {
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa phần dư trên dòng
      return value;
    }
  }
}

namespace UI
{

  void alert(const string &message)
  {
    cout << "[INFO] " << message << endl;
  }

  void showMainMenu()
  {
    int choice;
    do
    {
      cout << "\n===== MAIN MENU =====\n";
      cout << "1. Sign In\n";
      cout << "2. Sign Up\n";
      cout << "3. Exit\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
        showLoginMenu();
        break;
      case 2:
      {
        User newUser("", "", "", "");
        if (UserService::registerUser(newUser))
        {
          alert("Account created successfully!");
        }
        else
        {
          alert("Registration failed.");
        }
        break;
      }
      case 3:
        alert("Goodbye!");
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 3);
  }

  void showLoginMenu()
  {
    User currentUser("", "", "", "");
    if (UserService::loginUser(currentUser))
    {
      alert("Login successful!");
      if (currentUser.getRole() == "admin")
        showAdminMenu(currentUser);
      else
        showUserMenu(currentUser);
    }
    else
    {
      alert("Login failed. Please try again.");
    }
  }

  void showUserMenu(User &user)
  {
    int choice;
    do
    {
      cout << "\n===== USER MENU =====\n";
      cout << "Welcome, " << user.getUsername() << "\n";
      cout << "1. Manage Personal Info\n";
      cout << "2. Manage Wallet\n";
      cout << "3. Logout\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
        showManagerInfoMenu(user);
        break;
      case 2:
        showWalletMenu(user);
        break;
      case 3:
        alert("Logging out...");
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 3);
  }

  void showAdminMenu(User &admin)
  {
    int choice;
    do
    {
      cout << "\n===== ADMIN MENU =====\n";
      cout << "Welcome, " << admin.getUsername() << "\n";
      cout << "1. Manage Personal Info\n";
      cout << "2. Manage Users\n";
      cout << "3. Logout\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
        showManagerInfoMenu(admin);
        break;
      case 2:
        showUserManagerMenu(admin);
        break;
      case 3:
        alert("Logging out...");
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 3);
  }

  // --------------------- MANAGE INFO ---------------------

  void showManagerInfoMenu(User &user, bool isAdminEdit)
  {
    int choice;
    do
    {
      if (isAdminEdit)
      {
        cout << "\nManger " << user.getUsername() << "'s information\n";
      }
      else
      {
        cout << "\n===== Manage Information =====\n";
      }
      cout << "1. View Profile\n";
      cout << "2. Edit Full Name\n";
      cout << "3. Change Password\n";
      cout << "4. Back\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
        cout << "\n===== Your Information =====\n";
        cout << "Username: " << user.getUsername() << "\n";
        cout << "Full Name: " << user.getFullName() << "\n";
        cout << "Role: " << user.getRole() << "\n";
        break;

      case 2:
      {
        cout << "\n===== Edit Full Name =====\n";
        cout << "New Full Name: ";
        string newName;
        if (std::cin.rdbuf()->in_avail() > 0)
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, newName);

        string otp = OTP::generateOTP();
        cout << "[OTP]: " << otp << "\nEnter OTP to confirm: ";
        string input;
        cin >> input;
        if (OTP::verifyOTP(otp, input))
        {
          user.setFullName(newName);
          UserService::saveUser(user);
          alert("Name updated.");
          UserService::backup();
        }
        else
        {
          alert("Invalid OTP.");
        }
        break;
      }

      case 3:
      {
        cout << "\n===== Change Password =====\n";
        cout << "New Password: ";
        string newPass;
        cin >> newPass;

        string otp = OTP::generateOTP();
        cout << "[OTP]: " << otp << "\nEnter OTP to confirm: ";
        string input;
        cin >> input;
        if (OTP::verifyOTP(otp, input))
        {
          user.setPasswordHash(UserService::hashPassword(newPass));
          UserService::saveUser(user);
          alert("Password updated.");
          UserService::backup();
        }
        else
        {
          alert("Invalid OTP.");
        }
        break;
      }

      case 4:
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 4);
  }

  // --------------------- WALLET MENU (placeholder) ---------------------

  void showWalletMenu(User &user)
  {
    int choice;
    do
    {
      Wallet wallet("", 0);
      if (!WalletService::loadWallet(user.getUsername(), wallet))
      {
        alert("Wallet not found.");
        return;
      }

      cout << "\n===== Wallet Management =====\n";
      cout << "Wallet ID: WALLET_" << wallet.getOwner() << "\n";
      cout << "Balance: " << wallet.getBalance() << " points\n";
      cout << "--------------------------\n\n";
      cout << "1. Transfer Points\n";
      cout << "2. View Transaction History\n";
      cout << "3. Back\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
      {
        cout << "\n===== Transfer Points =====\n";
        cout << "Enter recipient username: ";
        string toUser;
        cin >> toUser;

        cout << "Enter amount to transfer: ";
        double amount;
        cin >> amount;

        string otp = OTP::generateOTP();
        cout << "[OTP]: " << otp << "\nEnter OTP to confirm: ";
        string input;
        cin >> input;
        if (OTP::verifyOTP(otp, input))
        {
          if (WalletService::transferPoints(user.getUsername(), toUser, amount))
          {
            alert("Transfer successful.");
          }
          else
          {
            alert("Transfer failed.");
          }
        }
        else
        {
          alert("Invalid OTP.");
        }
        break;
      }

      case 2:
      {
        vector<Transaction> history;
        WalletService::getTransactionHistory(user.getUsername(), history);
        cout << "\n===== Transaction History =====\n";
        for (const auto &tx : history)
        {
          time_t ts = tx.getTimestamp();
          cout << (tx.getFrom() == user.getUsername() ? "Sent" : "Received")
               << " " << tx.getAmount() << " points "
               << (tx.getFrom() == user.getUsername() ? "to " : "from ")
               << (tx.getFrom() == user.getUsername() ? tx.getTo() : tx.getFrom())
               << " at " << ctime(&ts);
        }
        break;
      }

      case 3:
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 3);
  }

  // --------------------- ADMIN: USER MANAGEMENT ---------------------

  void showUserManagerMenu(User &admin)
  {
    int choice;
    do
    {
      cout << "\n===== User Management =====\n";
      cout << "1. View All Users\n";
      cout << "2. Edit User Info\n";
      cout << "3. Change User Password\n";
      cout << "4. Create New User\n";
      cout << "5. Back\n";
      choice = safeInput<int>("Choice: ");

      switch (choice)
      {
      case 1:
      {
        vector<User> users;
        UserService::loadAllUsers(users);
        cout << "\n===== User List =====\n";
        cout << "Username - Full name - Role\n";
        for (auto &u : users)
          cout << u.getUsername() << " - " << u.getFullName() << " - " << u.getRole() << "\n";
        break;
      }

      case 2:
      {
        cout << "Enter username to edit: ";
        string uname;
        cin >> uname;
        User target("", "", "", "");
        if (UserService::findUserByUsername(uname, target))
        {
          showManagerInfoMenu(target, true);
        }
        else
        {
          alert("User not found.");
        }
        break;
      }

      case 3:
      {
        cout << "Enter username to change password: ";
        string uname;
        cin >> uname;
        User target("", "", "", "");
        if (UserService::findUserByUsername(uname, target))
        {
          cout << "New Password: ";
          string newPass;
          cin >> newPass;
          string otp = OTP::generateOTP();
          cout << "[OTP]: " << otp << "\nEnter OTP to confirm: ";
          string input;
          cin >> input;
          if (OTP::verifyOTP(otp, input))
          {
            target.setPasswordHash(UserService::hashPassword(newPass));
            UserService::saveUser(target);
            alert("Password updated.");
            UserService::backup();
          }
          else
          {
            alert("Invalid OTP.");
          }
        }
        else
        {
          alert("User not found.");
        }
        break;
      }

      case 4:
      {
        User newUser("", "", "", "");
        if (UserService::registerUser(newUser, true))
        {
          alert("User created.");
        }
        else
        {
          alert("Failed.");
        }
        break;
      }

      case 5:
        return;
      default:
        alert("Invalid choice.");
      }
    } while (choice != 5);
  }

}

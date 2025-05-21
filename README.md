# 📌 Hệ thống Quản lý Đăng nhập, Tài khoản và Ví điểm thưởng

## 📖 Giới thiệu dự án

Dự án mô phỏng một **hệ thống quản lý ví điểm thưởng** kèm theo chức năng **đăng ký / đăng nhập**, **quản lý tài khoản**, và **giao dịch điểm thưởng giữa các ví**.

Mỗi người dùng có một ví điểm, trong đó số điểm có thể được chuyển đổi (giao dịch) giữa các người dùng trong hệ thống. Một tài khoản quản trị trung tâm đóng vai trò phân phối điểm ban đầu (ví tổng). Các điểm này không thể sinh thêm — tổng điểm trong hệ thống là một hằng số.

Hệ thống cung cấp các chức năng:

- Đăng ký và đăng nhập tài khoản (bảo mật qua mật khẩu & OTP)
- Cập nhật thông tin cá nhân
- Chuyển điểm giữa các ví (đảm bảo tính toàn vẹn ACID)
- Lưu trữ và backup dữ liệu người dùng
- Quản lý giao dịch và hiển thị lịch sử

---

## 👥 Thành viên tham gia và công việc

| Họ tên            | Mã SV      | Vai trò & Công việc                                 | Github        |
| ----------------- | ---------- | --------------------------------------------------- | ------------- |
| Nguyễn Trà My     | K24DTCN526 | Giao diện dòng lệnh, kiểm thử chương trình          | [ntt247]      |
| Nguyễn Quang Minh | K24DTCN524 | Xử lý đăng nhập, sinh password tự động, data backup | [vsind18]     |
| Lê Thành Tiến     | K24DTCN539 | Trưởng nhóm, thiết kế hệ thống, quản lý ví          | [imthanhtien] |

---

## 📋 Đặc tả chức năng và phân tích hệ thống

### A. Quản lý tài khoản người dùng

#### 1. Đăng ký tài khoản

- Người dùng tự đăng ký, hoặc do admin đăng ký hộ.
- Mỗi tài khoản bao gồm: `username`, `full name`, `mật khẩu (hash)`, `vai trò (user/admin)`, `mật khẩu tự sinh (bool)`,...

#### 2. Lưu trữ & Backup

- Dữ liệu người dùng lưu trong **một tập tin chung** (`users.db`) giúp dễ tra cứu và cập nhật.
- Mật khẩu được lưu **dưới dạng hash** (SHA256).
- Dữ liệu backup tự động định kỳ sang thư mục `backup/` với timestamp.

#### 3. Đăng nhập và bảo mật

- Hỗ trợ mật khẩu tự sinh nếu tài khoản do admin tạo.
- Mỗi lần đăng nhập đầu nếu dùng mật khẩu tự sinh, hệ thống **bắt buộc đổi mật khẩu**.
- OTP (mã xác thực một lần) gửi qua giao diện giả lập (command line) khi người dùng thay đổi thông tin quan trọng.

### B. Phân loại người dùng

#### 1. Người dùng thường:

- Xem & chỉnh sửa thông tin cá nhân (`full name`, `mật khẩu`)
- OTP xác nhận các thay đổi quan trọng
- Mỗi người dùng có một ví điểm thưởng

#### 2. Người quản lý:

- Thêm tài khoản mới
- Thay đổi thông tin tài khoản (cần OTP người dùng)
- Xem danh sách người dùng

### C. Quản lý ví điểm thưởng

#### Cấu trúc:

- Mỗi ví có `wallet_id`, `balance`, `owner_username`
- Giao dịch là **atomic**: nếu thất bại ở bất kỳ bước nào, dữ liệu được khôi phục

#### Giao dịch:

1. Kiểm tra số dư ví nguồn
2. Trừ điểm ví nguồn, cộng điểm ví đích
3. Lưu vào **transaction log**
4. Gửi OTP để xác nhận giao dịch

---

## 💻 Hướng dẫn cài đặt và biên dịch chương trình

### 🧾 Yêu cầu hệ thống:

- Hệ điều hành: Windows / Linux
- Trình biên dịch: `g++` (hỗ trợ C++17 trở lên)
- Thư viện: Chỉ sử dụng thư viện chuẩn

### 🔽 Tải mã nguồn:

```bash
git clone https://github.com/vsind18/sample_cpp.git
cd sample_cpp
```

### 🔧 Biên dịch:

g++ -o main main.cpp src/otp.cpp src/ui.cpp src/user_service.cpp src/wallet_service.cpp src/models/transaction.cpp src/models/user.cpp src/models/wallet.cpp -lssl -lcrypto

### ⚙️ Chạy chương trình:

./main.exe

---

## 🛠️ Cấu trúc thư mục
```
├── main.cpp
├── src/
│ ├── models/
│ │ ├── user.cpp
│ │ ├── wallet.cpp
│ │ └── transaction.cpp
│ ├── user_service.cpp
│ ├── wallet_service.cpp
│ ├── otp.cpp
│ ├── ui.cpp
│ └── config.cpp
├── include/
│ ├── models/
│ │ ├── user.h
│ │ ├── wallet.h
│ │ └── transaction.h
│ ├── user_service.h
│ ├── wallet_service.h
│ ├── otp.h
│ └── ui.h
├── data/
│ ├── users.db
│ ├── wallets.db
│ └── transactions.db
├── backup/
└── README.md
```

---

## 📖 Tài liệu tham khảo

1. Tài liệu gốc yêu cầu đồ án (https://docs.google.com/document/d/16kdtLrrRWZdKO7p58bilSueyilMu4aE5a33gN7v9dVw/edit?pli=1&tab=t.0)
2. ACID là gì? (https://200lab.io/blog/acid-la-gi)

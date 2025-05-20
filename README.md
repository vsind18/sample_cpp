# 📌 Hệ thống quản lý ví tiền

## 📖 Giới thiệu dự án
Dự án mô phỏng một hệ thống quản lý ví điện tử đơn giản, trong đó nguồn tiền trong toàn hệ thống luôn là một hằng số cố định.

Hệ thống có một tài khoản quản lý trung tâm (tương tự "chủ ngân hàng") sở hữu một lượng tiền cố định ban đầu (const). Khi tạo người dùng mới, tài khoản quản lý này sẽ phân phối một phần tiền của mình sang ví của tài khoản đó theo số lượng tùy ý.

Sau khi được tạo, các người dùng có thể chuyển tiền cho nhau trong hệ thống. Tổng số tiền trong hệ thống không thay đổi, chỉ chuyển dịch giữa các ví cá nhân.

Mục tiêu của dự án là xây dựng một ứng dụng dòng lệnh giúp:

- Quản lý tài khoản người dùng

- Thực hiện thao tác chuyển tiền giữa các tài khoản

- Hiển thị thông tin số dư và lịch sử giao dịch

---

## 👥 Thành viên tham gia và công việc

| Họ tên            |Mã sinh viên | Vai trò / Công việc chính                                    | Github |
|-------------------|-------------|--------------------------------------------------------------|--------|
| Nguyễn Trà My       | K24DTCN526 | Trưởng nhóm, thiết kế kiến trúc hệ thống, xử lý backend     | ntt247 |
| Nguyễn Quang Minh         | K24DTCN524 | Phân tích yêu cầu, viết đặc tả, kiểm thử                    | vsind18 |
| Lê Thanh Tiến           | K24DTCN539 | Xây dựng giao diện người dùng, xử lý input/output          | imthanhtien |

> **Chi tiết phân công xem thêm tại mục [C. Phân công công việc]**

---

## 📋 Đặc tả chức năng và phân tích hệ thống

Dự án bao gồm các chức năng chính sau:

1. **Đăng ký người dùng**  
2. **Đăng nhập**  
3. **Cập nhật thông tin cá nhân**  
4. **Tìm kiếm và quản lý người dùng (với quyền admin)**  

Các yêu cầu, use case, và sơ đồ luồng hoạt động được mô tả chi tiết trong tệp `docs/specification.md`.

---

## 💻 Hướng dẫn cài đặt và biên dịch chương trình

### 🧾 Yêu cầu:
- Hệ điều hành: Windows/Linux
- Trình biên dịch: `g++` hoặc tương đương
- Thư viện: không sử dụng thư viện ngoài (chỉ C++ tiêu chuẩn)

### 🔽 Tải dự án:

```bash
git clone https://github.com/<tên-tài-khoản>/<tên-dự-án>.git
cd <tên-dự-án>
```
### 📖 Tài liệu tham khảo: 


# BÁO CÁO ĐỒ ÁN PBL2: LẬP TRÌNH HƯỚNG ĐỐI TƯỢNG (C++)
# ĐỀ TÀI: HỆ THỐNG QUẢN LÝ NHÂN VIÊN QUÁN CÀ PHÊ
### (Coffee Shop Employee Management System - Qt6 & CLI)

---

## MỤC LỤC
1. [THÔNG TIN CHUNG ĐỒ ÁN](#1-thông-tin-chung-đồ-án)
2. [ĐẶT VẤN ĐỀ VÀ MỤC TIÊU ĐỀ TÀI](#2-đặt-vấn-đề-và-mục-tiêu-đề-tài)
3. [KIẾN TRÚC HƯỚNG ĐỐI TƯỢNG (OOP DESIGN)](#3-kiến-trúc-hướng-đối-tượng-oop-design)
   - 3.1. Tính Trừu tượng (Abstraction)
   - 3.2. Tính Kế thừa (Inheritance)
   - 3.3. Tính Đóng gói (Encapsulation)
   - 3.4. Tính Đa hình (Polymorphism)
   - 3.5. Mẫu thiết kế Factory Method & Kiến trúc MVC
4. [THIẾT KẾ CƠ SỞ DỮ LIỆU & LƯU TRỮ TỆP](#4-thiết-kế-cơ-sở-dữ-liệu--lưu-trữ-tệp)
   - 4.1. Cơ chế Flat-file Database kết hợp In-Memory Cache
   - 4.2. Sơ đồ liên kết thực thể (Relational Schema)
   - 4.3. Quy tắc chuẩn hóa và an toàn dữ liệu
5. [CÁC PHÂN HỆ VÀ TÍNH NĂNG NỔI BẬT](#5-các-phân-hệ-và-tính-năng-nổi-bật)
   - 5.1. Phân hệ Quản trị viên (Admin)
   - 5.2. Phân hệ Nhân viên (Employee)
   - 5.3. Các cải tiến kỹ thuật đặc sắc
6. [HƯỚNG DẪN CÀI ĐẶT & VẬN HÀNH](#6-hướng-dẫn-cài-đặt--vận-hành)
7. [BỘ CÂU HỎI VẤN ĐÁP BẢO VỆ ĐỒ ÁN THƯỜNG GẶP](#7-bộ-câu-hỏi-vấn-đáp-bảo-vệ-đồ-án-thường-gặp)

---

## 1. THÔNG TIN CHUNG ĐỒ ÁN
* **Tên đề tài:** Hệ Thống Quản Lý Nhân Viên Quán Cà Phê
* **Học phần:** PBL2 - Dự án Cơ sở Lập trình hướng đối tượng
* **Ngôn ngữ phát triển:** C++ (tiêu chuẩn C++17 hiện đại)
* **Giao diện người dùng:** 
  - **Desktop GUI:** Qt6 Framework (QtCore, QtGui, QtWidgets)
  - **Console CLI:** Giao diện dòng lệnh tương tác trực tiếp
* **Công cụ xây dựng (Toolchain):** MSYS2 UCRT64, GCC 13.x/15.x, CMake 3.20+, Ninja Build
* **Kho mã nguồn GitHub:** [https://github.com/thophat15815800/quanlycafe](https://github.com/thophat15815800/quanlycafe)

---

## 2. ĐẶT VẤN ĐỀ VÀ MỤC TIÊU ĐỀ TÀI

### 2.1. Đặt vấn đề
Trong hoạt động kinh doanh quán cà phê (F&B), việc quản lý nhân sự gặp nhiều thách thức:
- **Đa dạng vị trí công việc:** Pha chế, Thu ngân, Phục vụ, Bảo vệ, Quản lý với chế độ phụ cấp và tính lương khác nhau.
- **Hình thức làm việc linh hoạt:** Kết hợp nhân viên Full-time (lương tháng cố định) và Part-time (tính lương theo giờ làm việc thực tế).
- **Phân ca phức tạp:** Thường gồm 3 ca mỗi ngày (Sáng, Chiều, Tối), nhân viên có thể xoay ca, tăng ca hoặc làm nhiều ca trong cùng một ngày.
- **Rủi ro quản lý thủ công:** Ghi chép sổ sách hoặc Excel rời rạc dễ gây trùng lịch, va chạm khung giờ, sai sót trong chấm công và tính lương.

### 2.2. Mục tiêu đề tài
1. Xây dựng một ứng dụng hoàn chỉnh, chuyên nghiệp giúp tự động hóa toàn bộ quy trình: Quản lý hồ sơ nhân sự $\rightarrow$ Xếp lịch làm việc $\rightarrow$ Chấm công thời gian thực $\rightarrow$ Tính bảng lương tự động $\rightarrow$ Phân quyền tài khoản bảo mật.
2. Ứng dụng triệt để và làm chủ **4 nguyên lý cốt lõi của Lập trình hướng đối tượng (OOP)**.
3. Làm chủ kỹ thuật lập trình C++ hiện đại: Smart Pointers (`std::unique_ptr`), Concurrency safety, File Streams, và lập trình giao diện Desktop với Qt6.

---

## 3. KIẾN TRÚC HƯỚNG ĐỐI TƯỢNG (OOP DESIGN)

```
                    ┌─────────────────────────┐
                    │    <<Abstract Class>>   │
                    │        NhanVien         │
                    ├─────────────────────────┤
                    │ - maNV, hoTen, sdt, ... │
                    │ - luongCoBan, trangThai │
                    │ - avatar                │
                    ├─────────────────────────┤
                    │ + tinhLuong() = 0       │
                    │ + loaiNhanVien() = 0    │
                    │ + toCSV() = 0           │
                    └────────────┬────────────┘
                                 │
     ┌──────────────┬────────────┼────────────┬──────────────┐
     │              │            │            │              │
┌────▼─────┐  ┌─────▼────┐ ┌─────▼────┐ ┌─────▼────┐  ┌──────▼─────┐
│  PhaChe  │  │ ThuNgan  │ │  QuanLy  │ │  PhuVu   │  │   BaoVe    │
├──────────┤  ├──────────┤ ├──────────┤ ├──────────┤  ├────────────┤
│ (Barista)│  │+phuCapTN │ │+phuCapQL │ │ (Server) │  │ (Security) │
└──────────┘  └──────────┘ └──────────┘ └──────────┘  └────────────┘
```

### 3.1. Tính Trừu tượng (Abstraction)
- Lớp cơ sở trừu tượng `NhanVien` mô hình hóa các thuộc tính và hành vi cốt lõi của một nhân sự quán cà phê.
- Khai báo các **phương thức thuần ảo (Pure Virtual Functions)**:
  ```cpp
  virtual double tinhLuong(double soGioLam = 0) const = 0;
  virtual std::string loaiNhanVien() const = 0;
  virtual std::string toCSV() const = 0;
  ```
- Nhờ đó, lớp `NhanVien` không thể khởi tạo trực tiếp mà đóng vai trò là bản giao ước (Interface/Contract) cho các lớp dẫn xuất.

### 3.2. Tính Kế thừa (Inheritance)
- 5 lớp con kế thừa công khai (`public NhanVien`):
  1. `PhaChe`: Chịu trách nhiệm pha chế đồ uống, tính lương theo giờ/tháng.
  2. `ThuNgan`: Kế thừa các thuộc tính chung và mở rộng thuộc tính riêng `phuCapTrachNhiem` (quản lý quỹ tiền mặt).
  3. `QuanLy`: Kế thừa và mở rộng thuộc tính `phuCapQuanLy` (điều hành ca và xử lý sự vụ).
  4. `PhuVu`: Nhân viên phục vụ bàn, order món.
  5. `BaoVe`: Trông giữ phương tiện của khách, bảo đảm an ninh quán.
- Tái sử dụng mã nguồn tối đa: Các thuộc tính chung (`maNV`, `hoTen`, `sdt`, `email`, `ngayVaoLam`, `luongCoBan`, `trangThai`, `avatar`) được kế thừa hoàn toàn từ lớp cha.

### 3.3. Tính Đóng gói (Encapsulation)
- Thuộc tính được đặt trong vùng `protected` hoặc `private` nhằm ngăn chặn can thiệp trực tiếp từ bên ngoài.
- Dữ liệu chỉ được truy cập và cập nhật thông qua các phương thức `Getter` và `Setter` có cơ chế kiểm tra tính hợp lệ dữ liệu (Validation).
- Trạng thái nhân viên được định nghĩa chặt chẽ bằng `enum class TrangThaiNV { DANG_LAM_VIEC, TAM_NGHI, DA_NGHI_VIEC }`.

### 3.4. Tính Đa hình (Polymorphism)
- **Đa hình động (Runtime Polymorphism):**
  - Quản lý danh sách nhân viên bằng con trỏ lớp cơ sở thông minh: `std::vector<std::unique_ptr<NhanVien>>`.
  - Khi tính lương hoặc lưu tệp, hệ thống chỉ cần duyệt danh sách và gọi:
    ```cpp
    for (const auto& nv : danhSachNV) {
        double luong = nv->tinhLuong(soGio); // Tự động gọi đúng hàm tính lương của từng vị trí
        f << nv->toCSV() << "\n";            // Tự động gọi đúng định dạng CSV của từng lớp con
    }
    ```
  - Cơ chế bảng ảo `vtable` và con trỏ ảo `vptr` của C++ quyết định chính xác phương thức của lớp con được thực thi tại thời điểm chạy.
- **Đa hình tĩnh (Compile-time Polymorphism):** Nạp chồng phương thức (Function Overloading), nạp chồng toán tử so sánh `operator<` phục vụ các thuật toán sắp xếp của STL.

### 3.5. Mẫu thiết kế Factory Method & Kiến trúc MVC
- **Factory Method ([`FileManager::taoTuDong`](file:///D:/PBL2/quanlycafe-main/src/FileManager.cpp#L24-L58)):** Khi đọc từng dòng tệp `nhanvien.csv`, hàm đọc trường `loai` để tự động khởi tạo đúng đối tượng lớp con tương ứng bằng `std::make_unique`.
- **Kiến trúc MVC (Model - View - Controller):**
  - **Model:** `NhanVien` (và các lớp con), `CaLamViec`, `ChamCong`, `BangLuong`, `TaiKhoan`.
  - **Controller / Service:** `QuanLyQuanCafe` (điều phối nghiệp vụ trung tâm), `QuanLyTaiKhoan`, `FileManager`.
  - **View:** Giao diện đồ họa Qt6 (`MainWindow`, `LoginDialog`, `AddEmployeeDialog`, `EditEmployeeDialog`) và giao diện dòng lệnh `ConsoleUI`.

---

## 4. THIẾT KẾ CƠ SỞ DỮ LIỆU & LƯU TRỮ TỆP

### 4.1. Cơ chế Flat-file Database kết hợp In-Memory Cache
- Dữ liệu được lưu trữ bền vững dưới dạng các tệp CSV chuẩn hóa trong thư mục `data/`.
- Khi khởi chạy phần mềm: Dữ liệu được đọc và ánh xạ vào RAM trong các cấu trúc dữ liệu STL tối ưu (`vector`, `map`, `unordered_set`).
- Mọi thao tác tìm kiếm, kiểm tra, phân loại diễn ra trên RAM với tốc độ cực cao, sau đó đồng bộ ghi đè an toàn xuống ổ đĩa.

### 4.2. Sơ đồ liên kết thực thể (Relational Schema)

```
       ┌────────────────────────┐
       │      taikhoan.csv      │
       │ (maNV -> Khóa ngoại)   │
       └───────────┬────────────┘
                   │ 1 - 1
                   ▼
       ┌────────────────────────┐
       │      nhanvien.csv      │◄── (Bảng trung tâm: maNV là Khóa chính)
       └─────┬────────────┬─────┘
             │ 1 - n      │ 1 - n
             ▼            ▼
┌──────────────────┐  ┌──────────────────┐
│   chamcong.csv   │  │    luong.csv     │
│ (maNV: Khóa ngoài)│  │(maNV: Khóa ngoài)│
└──────────────────┘  └──────────────────┘
             ▲
             │ n - n
┌────────────────────────┐
│    lichlamviec.csv     │
│(dsMaNV: Khóa ngoại)    │
└────────────────────────┘
```

* **[data/nhanvien.csv](file:///D:/PBL2/quanlycafe-main/data/nhanvien.csv):** Lưu thông tin hồ sơ nhân viên (`maNV,hoTen,sdt,email,ngayVaoLam,luongCoBan,trangThai,loai,phuCap,avatar`).
* **[data/taikhoan.csv](file:///D:/PBL2/quanlycafe-main/data/taikhoan.csv):** Lưu thông tin tài khoản đăng nhập (`tenDangNhap,matKhau,vaiTro,maNV`).
* **[data/lichlamviec.csv](file:///D:/PBL2/quanlycafe-main/data/lichlamviec.csv):** Lưu lịch các ca làm việc (`maCa;ngay;tenCa;gioBatDau;gioKetThuc;dsMaNV`).
* **[data/chamcong.csv](file:///D:/PBL2/quanlycafe-main/data/chamcong.csv):** Lưu nhật ký ra/vào ca (`maNV,ngay,gioVao,gioRa`).
* **[data/luong.csv](file:///D:/PBL2/quanlycafe-main/data/luong.csv):** Lưu bảng lương đã kết toán theo tháng (`maNV,thangNam,soGioLam,thucLinh`).
* **Thư mục [data/avatars/](file:///D:/PBL2/quanlycafe-main/data/avatars):** Lưu trữ tệp hình ảnh đại diện của nhân viên (`NV1.jpg`, `NV2.jpg`,...).

### 4.3. Quy tắc chuẩn hóa và an toàn dữ liệu
- **Khử trùng lặp ca và nhân viên:** Hàm `chuanHoaLich()` bảo đảm trong cùng một ngày không có 2 ca trùng mã, và trong một ca không có nhân viên bị lặp mã.
- **Chống va chạm giờ làm việc (`bTrungCa`):** Nhân viên có thể làm nhiều ca trong ngày (Sáng 07-12h, Chiều 12-17h, Tối 17-22h), nhưng hệ thống tự động từ chối nếu xếp ca có khung giờ chéo nhau.
- **Ghi tệp an toàn (Idempotent Save):** Dù bấm Lưu dữ liệu nhiều lần liên tục, hệ thống đều chuẩn hóa trước khi ghi và dùng chế độ ghi toàn vẹn, cam kết không bao giờ bị nhân đôi dòng dữ liệu.

---

## 5. CÁC PHÂN HỆ VÀ TÍNH NĂNG NỔI BẬT

### 5.1. Phân hệ Quản trị viên (Admin)
1. **Dashboard điều hành:** Thống kê tổng số nhân sự, số lượng đang làm/tạm nghỉ/đã nghỉ, số ca trực hôm nay và bảng nhật ký thao tác thời gian thực.
2. **Quản lý nhân viên toàn diện:**
   - Thêm mới nhân viên với đầy đủ chức vụ, ảnh đại diện, lương cơ bản và phụ cấp.
   - Sửa thông tin trực quan qua hộp thoại `EditEmployeeDialog`.
   - Chuyển trạng thái nhân sự (Đang làm, Tạm nghỉ, Cho nghỉ việc).
   - Bộ lọc đa tiêu chí (theo từ khóa tên/mã, theo loại chức vụ, theo trạng thái).
   - Sắp xếp linh hoạt (theo Mã NV, Họ tên A-Z, Mức lương).
3. **Quản lý Lịch làm việc & Phân ca:**
   - Luôn tự động duy trì 3 ca chuẩn mỗi ngày: Ca Sáng (07:00-12:00), Ca Chiều (12:00-17:00), Ca Tối (17:00-22:00).
   - Thêm ca bổ sung linh hoạt và xóa ca bổ sung khi không còn nhu cầu.
   - Chỉ định nhân viên vào ca qua danh sách chọn, tự động kiểm tra chống trùng giờ.
4. **Quản lý Chấm công & Tính lương:**
   - Quản trị viên có thể hỗ trợ check-in / check-out trực tiếp cho nhân viên.
   - Tự động tính toán số giờ làm việc thực tế trong tháng và xuất bảng lương tổng quỹ lương của quán.
5. **Quản lý Tài khoản & Phân quyền:**
   - Cấp tài khoản đăng nhập cho nhân viên mới.
   - Reset mật khẩu về mặc định (`123456`).
   - Xóa tài khoản (bảo vệ tài khoản đang đăng nhập không bị xóa nhầm).

### 5.2. Phân hệ Nhân viên (Employee)
1. **Hồ sơ cá nhân:** Xem thông tin công việc, ngày vào làm, và tính năng tự cập nhật ảnh đại diện cá nhân.
2. **Lịch làm của tôi:** Tra cứu toàn bộ ca làm được phân công, tự động sắp xếp khoa học theo đúng dòng thời gian (Năm $\rightarrow$ Tháng $\rightarrow$ Ngày $\rightarrow$ Giờ bắt đầu).
3. **Chấm công cá nhân:** Tự bấm Check-in khi vào ca và Check-out khi hết ca với trạng thái hiển thị trực quan.
4. **Bảng lương cá nhân:** Tra cứu tổng số giờ làm và tiền lương thực lĩnh theo từng tháng.

### 5.3. Các cải tiến kỹ thuật đặc sắc
- **Avatar Helper ([`gui/AvatarHelper.h`](file:///D:/PBL2/quanlycafe-main/gui/AvatarHelper.h)):**
  - Tự động cắt tròn ảnh đại diện (Circular crop) bằng `QPainter` và `Antialiasing` chống răng cưa.
  - Tự động sinh Avatar chữ cái mặc định thông minh (dựa trên chữ cái đầu của tên và bảng màu pastel sang trọng) khi nhân viên chưa có ảnh.
  - Tự động sao chép và chuẩn hóa ảnh vào thư mục `data/avatars/`.
- **Bộ lọc sự kiện chống trượt chuột (`WheelBlocker`):**
  - Chặn triệt để sự kiện cuộn con lăn chuột trên các ô `QDateEdit`, loại bỏ hoàn toàn lỗi vô tình nhảy năm ảo (như sang năm 2027, 2028) thường gặp trên Qt.
- **Quản lý tài nguyên an toàn:**
  - Áp dụng nguyên lý RAII (Resource Acquisition Is Initialization).
  - Sử dụng con trỏ thông minh `std::unique_ptr` triệt tiêu nguy cơ rò rỉ bộ nhớ (Zero Memory Leak).

---

## 6. HƯỚNG DẪN CÀI ĐẶT & VẬN HÀNH

### 6.1. Yêu cầu môi trường
- Hệ điều hành: Windows 10/11 (64-bit).
- Đã cài đặt MSYS2 UCRT64 với các gói: `gcc`, `cmake`, `ninja`, `qt6-base`.

### 6.2. Các bước biên dịch và chạy (1-Click Scripts)
- **Cách 1 (Khuyên dùng):** Nhấp đúp vào tệp **[`Chay_App_GUI.bat`](file:///D:/PBL2/quanlycafe-main/Chay_App_GUI.bat)**. Script sẽ tự động kiểm tra, biên dịch ra `quanlycafe_gui.exe` (nếu chưa có) và khởi chạy ngay lập tức.
- **Cách 2:** Nhấp đúp vào **[`Build_GUI.bat`](file:///D:/PBL2/quanlycafe-main/Build_GUI.bat)** để thực hiện biên dịch lại dự án từ đầu.
- **Đẩy code lên GitHub:** Nhấp đúp vào **[`Day_Len_Github.bat`](file:///D:/PBL2/quanlycafe-main/Day_Len_Github.bat)** để tự động đồng bộ mã nguồn lên repository.

### 6.3. Tài khoản đăng nhập kiểm thử
| Vai trò | Tên đăng nhập | Mật khẩu | Ghi chú |
|---|---|---|---|
| **Quản trị viên (Admin)** | `admin` | `admin123` | Toàn quyền quản trị hệ thống |
| **Nhân viên (Employee)** | `NV1` | `123456` | Pha chế (Nguyễn Văn An) |
| **Nhân viên (Employee)** | `NV2` | `123456` | Thu ngân (Trần Thị Bích) |
| **Nhân viên (Employee)** | `NV3` | `123456` | Quản lý (Lê Hoàng Nam) |
| **Nhân viên (Employee)** | `NV4` | `123456` | Phục vụ (Phạm Thu Hà) |
| **Nhân viên (Employee)** | `NV5` | `123456` | Bảo vệ (Hoàng Văn Hải) |

---

## 7. BỘ CÂU HỎI VẤN ĐÁP BẢO VỆ ĐỒ ÁN THƯỜNG GẶP

### Câu 1: Tại sao đề tài không dùng MySQL/SQL Server mà dùng tệp CSV?
* **Trả lời:** Mục tiêu trọng tâm của đồ án PBL2 là thể hiện năng lực lập trình hướng đối tượng, làm chủ cấu trúc dữ liệu và kỹ năng xử lý tệp trong C++. Việc dùng Flat-file kết hợp In-Memory Cache giúp ứng dụng gọn nhẹ, độc lập tuyệt đối (chạy được ngay trên mọi máy mà không cần cài server CSDL), và tốc độ truy xuất trên RAM cực nhanh. Ngoài ra, lớp `FileManager` đã được thiết kế tách biệt, nếu sau này cần chuyển sang SQLite hay MySQL chỉ cần thay đổi lớp này mà không ảnh hưởng đến toàn bộ mã nguồn nghiệp vụ.

### Câu 2: Tính đa hình (Polymorphism) được thể hiện ở những vị trí nào trong code?
* **Trả lời:** Thể hiện rõ nhất ở lớp trừu tượng `NhanVien` với các phương thức thuần ảo `tinhLuong()`, `loaiNhanVien()`, `toCSV()`. Khi quản lý danh sách nhân viên bằng `std::vector<std::unique_ptr<NhanVien>>`, chương trình chỉ cần gọi `nv->tinhLuong()` hay `nv->toCSV()` thì cơ chế Dynamic Binding (thông qua bảng ảo `vtable`) sẽ tự động điều hướng thực thi đúng phương thức của lớp con (`PhaChe`, `ThuNgan`, `QuanLy`,...).

### Câu 3: Mẫu thiết kế Factory Method được áp dụng ra sao khi nạp dữ liệu?
* **Trả lời:** Được hiện thực trong hàm `FileManager::taoTuDong(const std::string& line)`. Khi đọc từng dòng tệp CSV, hàm bóc tách cột `loai` (như `PHA_CHE`, `THU_NGAN`...). Tùy vào giá trị chuỗi này, hàm sẽ gọi `std::make_unique<T>` để khởi tạo đúng kiểu lớp con tương ứng và trả về con trỏ đa hình `std::unique_ptr<NhanVien>`.

### Câu 4: Làm thế nào chương trình đảm bảo an toàn bộ nhớ, tránh rò rỉ (Memory Leak)?
* **Trả lời:** Toàn bộ chương trình áp dụng nguyên lý RAII và sử dụng con trỏ thông minh chuẩn C++11/C++17 (`std::unique_ptr` cho quyền sở hữu độc quyền danh sách nhân viên, `std::shared_ptr` khi cần chia sẻ). Khi vector bị hủy hoặc đối tượng ra khỏi phạm vi (scope), bộ nhớ tự động được giải phóng mà không cần gọi `delete` thủ công.

### Câu 5: Thuật toán kiểm tra trùng lịch làm việc (`bTrungCa`) hoạt động như thế nào?
* **Trả lời:** Hàm `Utils::gioToPhut()` đổi các mốc thời gian dạng `HH:MM` ra số phút trong ngày (0 đến 1439). Khi kiểm tra ca mới `[batDauMoi, ketThucMoi]` với ca cũ `[b, k]`, điều kiện không trùng là `ketThucMoi <= b || batDauMoi >= k`. Nếu phủ định điều kiện trên, hệ thống xác định 2 ca có giao thoa thời gian và trả về kết quả trùng ca để cảnh báo.

---
*Tài liệu được chuẩn bị hoàn chỉnh phục vụ báo cáo và bảo vệ đồ án PBL2.*

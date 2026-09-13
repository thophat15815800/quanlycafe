# He Thong Quan Ly Quan Cafe (Coffee Shop Management System)

He thong quan ly nhan su, ca lam viec, cham cong va tinh luong cho quan ca phe, duoc phat trien bang **C++ (C++17)** voi kien truc chuan **MVC**, ho tro ca hai giao dien: **Console CLI** va **Desktop GUI bang Qt6**.

---

## Cac Tinh Nang Chinh

### 1. Quan Ly Nhan Vien
- Them, sua, cap nhat thong tin nhan vien (Full-time & Part-time).
- Ho tro quan ly trang thai nhan vien (Dang lam viec, Tam nghi, Da nghi viec).
- Bo loc & tim kiem da tieu chi (theo ten, ma, vi tri, trang thai).
- Sap xep danh sach nhan vien theo Ma, Ho ten, Luong co ban.

### 2. Quan Ly Lich Lam Viec & Chia Ca
- **3 ca lam viec co dinh moi ngay:**
  - Ca Sang: `07:00 - 12:00`
  - Ca Chieu: `12:00 - 17:00`
  - Ca Toi: `17:00 - 22:00`
- Chi dinh nhan vien vao ca thuan tien qua danh sach chon (ComboBox).
- Kiem tra chong trung ca lam viec trong cung mot ngay cho nhan vien.
- Tao ca bo sung rieng (ca dac biet, cuoi tuan) va xoa ca bo sung khi khong su dung.

### 3. Cham Cong & Tinh Luong
- Cham cong Check-in / Check-out theo thoi gian thuc.
- Tu dong tinh gio lam viec thuc te.
- Bang luong tu dong theo thang cho nhan vien Full-time (luong thang) va Part-time (theo so gio lam).

### 4. Phan Quyen & Tai Khoan
- **Admin:** Quan tri toan quyen (Nhan su, Lich lam, Cham cong, Tinh luong, Tai khoan he thong).
- **Employee (Nhan vien):** Xem ho so ca nhan, xem lich lam viec ca nhan, tu check-in/check-out va tra cuu bang luong.

---

## Cau Truc Thu Muc

```
quanlycafe/
├── data/                  # Du lieu luu tru CSV (nhanvien.csv, lichlamviec.csv, ...)
├── include/               # Header files (.h) kien truc Model & Controller
├── src/                   # Source code C++ (.cpp) cho Logic nghiep vu
├── gui/                   # Ma nguon giao dien do hoa Qt6 (MainWindow, LoginDialog, ...)
├── CMakeLists.txt         # Cau hinh build CMake cho GUI Qt6
├── Makefile               # Makefile cho phien ban Console CLI
├── Chay_App_GUI.bat       # Script khoi chay nhanh phien ban GUI
└── README.md
```

---

## Huong Dan Cai Dat & Bien Dich

### Yeu cau he thong
- Trinh bien dich C++ ho tro C++17 (GCC / MinGW-w64 / MSYS2 UCRT64).
- Qt6 Base (phien ban Qt 6.x cho ban GUI).
- CMake 3.16 tro len & Ninja/Make.

### 1. Bien dich phien ban Console (CLI)
```bash
make
./quanlycafe.exe
```

### 2. Bien dich phien ban Desktop GUI (Qt6)
```powershell
cmake -B build_gui -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build_gui
```
Hoac chay truc tiep file `Chay_App_GUI.bat` tren Windows.

---

## Tai Khoan Mac Dinh

- **Quan tri vien (Admin):** `admin` / `admin123`
- **Nhan vien:** `NV1` den `NV5` / `123456`

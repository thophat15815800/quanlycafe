# He Thong Quan Ly Quan Cafe (Coffee Shop Management System)

He thong quan ly nhan su, ca lam viec, cham cong va tinh luong cho quan ca phe, duoc phat trien bang **C++ (C++17)** voi kien truc chuan **MVC**, ho tro ca hai giao dien: **Console CLI** va **Desktop GUI bang Qt6**.

---

## ⚡ Danh Cho Dong Doi Moi Clone / Tai Du An Tu GitHub

Khi tai ma nguon ve tu GitHub, cac file `.exe` mac dinh khong duoc day len de giu dung chuan repo sach. Dong doi chi can lam theo 2 buoc sau:

### Buoc 1: Cai dat moi truong (Neu may chua co MSYS2 UCRT64 va Qt6)
1. Cai dat [MSYS2](https://www.msys2.org/) (mac dinh tai `C:\msys64`).
2. Mo **MSYS2 UCRT64** (tim trong Menu Start) va chay duy nhat 1 dong lenh sau:
   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-qt6-base
   ```

### Buoc 2: Bien dich ra file `.exe`
- Nhap dup chuot vao file **`Build_GUI.bat`** (hoac nhap dup **`Chay_App_GUI.bat`** - script se tu dong build neu chua co file `.exe`).
- Ung dung se tu dong sinh ra file `quanlycafe_gui.exe` va khoi chay ngay lap tuc!

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
├── Build_GUI.bat          # Script bien dich tu dong ra file quanlycafe_gui.exe
├── Chay_App_GUI.bat       # Script khoi chay nhanh phien ban GUI
└── README.md
```

---

## Tai Khoan Mac Dinh

- **Quan tri vien (Admin):** `admin` / `admin123`
- **Nhan vien:** `NV1` den `NV5` / `123456`

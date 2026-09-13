#include "ConsoleUI.h"
#include "PhaChe.h"
#include "ThuNgan.h"
#include "PhuVu.h"
#include "BaoVe.h"
#include "QuanLy.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>

#ifdef _WIN32
  #include <direct.h>
  #include <conio.h>    // _getch() để ẩn mật khẩu
  #include <io.h>       // _isatty, _fileno
#else
  #include <sys/stat.h>
  #include <termios.h>
  #include <unistd.h>
#endif

static void taoThuMucNeuChua(const std::string& duongDan) {
#ifdef _WIN32
    _mkdir(duongDan.c_str());
#else
    mkdir(duongDan.c_str(), 0755);
#endif
}

ConsoleUI::ConsoleUI() {
    taoThuMucNeuChua(thuMucDuLieu);
    app.taiTatCa(thuMucDuLieu);
    quanLyTK.taiFile(thuMucDuLieu + "/taikhoan.csv");
    // Đồng bộ tài khoản cho các nhân viên đã có sẵn trong danh sách
    std::vector<std::string> dsMaNV;
    for (const auto& nv : app.layDanhSachNV()) {
        dsMaNV.push_back(nv->getMaNV());
    }
    quanLyTK.dongBoTaiKhoanNhanVien(dsMaNV);
    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
}

// ================= Hàm nhập liệu tiện ích, có kiểm tra lỗi =================
int ConsoleUI::nhapSoNguyen(const std::string& goiY) {
    int x;
    while (true) {
        std::cout << goiY;
        if (std::cin >> x) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return x; }
        if (std::cin.eof()) {
            std::cout << "\n(Het du lieu dau vao - luu va thoat chuong trinh)\n";
            app.luuTatCa(thuMucDuLieu);
            quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  -> Vui long nhap so nguyen hop le!\n";
    }
}

double ConsoleUI::nhapSoThuc(const std::string& goiY) {
    double x;
    while (true) {
        std::cout << goiY;
        if (std::cin >> x) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return x; }
        if (std::cin.eof()) {
            std::cout << "\n(Het du lieu dau vao - luu va thoat chuong trinh)\n";
            app.luuTatCa(thuMucDuLieu);
            quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
            std::exit(0);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  -> Vui long nhap so hop le!\n";
    }
}

std::string ConsoleUI::nhapChuoi(const std::string& goiY) {
    std::string s;
    std::cout << goiY;
    std::getline(std::cin, s);
    return Utils::trim(s);
}

// Nhập mật khẩu (ẩn ký tự bằng * khi dùng terminal tương tác)
std::string ConsoleUI::nhapMatKhau(const std::string& goiY) {
    std::string matKhau;
    std::cout << goiY;
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) {
        // Nếu input bị redirect (pipe / test script)
        std::getline(std::cin, matKhau);
        return Utils::trim(matKhau);
    }
    char ch;
    while ((ch = _getch()) != '\r') { // Enter = '\r' trên Windows
        if (ch == '\b') { // Backspace
            if (!matKhau.empty()) {
                matKhau.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch >= 32) { // Ký tự in được
            matKhau += ch;
            std::cout << '*';
        }
    }
    std::cout << "\n";
#else
    if (!isatty(fileno(stdin))) {
        std::getline(std::cin, matKhau);
        return Utils::trim(matKhau);
    }
    // Linux/Mac: tắt echo
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, matKhau);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n";
#endif
    return matKhau;
}

// ================= Hàm nhập có validate =================
std::string ConsoleUI::nhapSDT(const std::string& goiY) {
    while (true) {
        std::string sdt = nhapChuoi(goiY);
        if (Utils::isValidSDT(sdt)) return sdt;
        std::cout << "  -> SDT khong hop le! Phai co 10-11 chu so, bat dau bang 0.\n";
    }
}

std::string ConsoleUI::nhapEmail(const std::string& goiY) {
    while (true) {
        std::string email = nhapChuoi(goiY);
        if (Utils::isValidEmail(email)) return email;
        std::cout << "  -> Email khong hop le! Phai co dang abc@xyz.com\n";
    }
}

std::string ConsoleUI::nhapNgay(const std::string& goiY, bool choPhepTrong) {
    while (true) {
        std::string ngay = nhapChuoi(goiY);
        if (choPhepTrong && ngay.empty()) return Utils::ngayHomNay();
        if (Utils::isValidNgay(ngay)) return ngay;
        std::cout << "  -> Ngay khong hop le! Nhap dung dinh dang dd/mm/yyyy.\n";
    }
}

std::string ConsoleUI::nhapGio(const std::string& goiY) {
    while (true) {
        std::string gio = nhapChuoi(goiY);
        if (Utils::isValidGio(gio)) return gio;
        std::cout << "  -> Gio khong hop le! Nhap dung dinh dang HH:MM (00:00 - 23:59).\n";
    }
}

// ========================= MÀN HÌNH ĐĂNG NHẬP =========================
bool ConsoleUI::manHinhDangNhap() {
    std::cout << "\n###################################################\n"
                 "#      QUAN LY NHAN VIEN QUAN CA PHE - C++        #\n"
                 "#-------------------------------------------------#\n"
                 "#                  DANG NHAP                      #\n"
                 "###################################################\n";
    std::string tenDN = nhapChuoi("Ten dang nhap: ");
    std::string matKhau = nhapMatKhau("Mat khau: ");
    if (quanLyTK.dangNhap(tenDN, matKhau)) {
        std::cout << "\n  Dang nhap thanh cong! Xin chao, " << tenDN << "!\n";
        std::cout << "  Vai tro: " << vaiTroToChuoi(quanLyTK.getVaiTroHienTai()) << "\n";
        return true;
    }
    std::cout << "\n  Sai ten dang nhap hoac mat khau!\n";
    return false;
}

// ============================ DASHBOARD ============================
void ConsoleUI::hienDashboard() {
    auto dt = app.layDashboard();
    std::cout << "\n==================== DASHBOARD QUAN CA PHE ====================\n";
    std::cout << "  [Dang nhap: " << quanLyTK.getTenDangNhapHienTai()
              << " | Vai tro: " << vaiTroToChuoi(quanLyTK.getVaiTroHienTai()) << "]\n";
    std::cout << "+--------------------+--------------------+--------------------+\n";
    std::cout << "|  Tong NV hoat dong |  Dang lam viec     |  Tam nghi          |\n";
    std::cout << "|      " << std::left << std::setw(15) << dt.tongNhanVien
              << "|      " << std::setw(15) << dt.dangLamViec
              << "|      " << std::setw(15) << dt.tamNghi << "|\n";
    std::cout << "+--------------------+--------------------+--------------------+\n";
    std::cout << "|  Da nghi viec      |  Ca lam hom nay    |  NV duoc xep ca    |\n";
    std::cout << "|      " << std::setw(15) << dt.daNghiViec
              << "|      " << std::setw(15) << dt.soCaHomNay
              << "|      " << std::setw(15) << dt.soNhanVienDuocXepCaHomNay << "|\n";
    std::cout << "+--------------------+--------------------+--------------------+\n";

    std::cout << "\n-- Lich hom nay (" << Utils::ngayHomNay() << ") --\n";
    auto dsCa = app.layLich().xemLichNgay(Utils::ngayHomNay());
    if (dsCa.empty()) {
        std::cout << "  (chua co ca nao duoc xep)\n";
    } else {
        for (const auto& ca : dsCa) {
            std::cout << "  [" << ca.getMaCa() << "] " << ca.getTenCa() << "  "
                      << ca.getGioBatDau() << " - " << ca.getGioKetThuc()
                      << "  | NV: ";
            const auto& ds = ca.getDsNhanVien();
            if (ds.empty()) std::cout << "(chua co)";
            for (size_t i = 0; i < ds.size(); ++i) {
                std::cout << ds[i] << (i + 1 < ds.size() ? ", " : "");
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n-- Hoat dong gan day --\n";
    auto nk = app.layNhatKyGanDay();
    size_t start = nk.size() > 5 ? nk.size() - 5 : 0;
    for (size_t i = start; i < nk.size(); ++i) std::cout << "  " << nk[i] << "\n";
    std::cout << "=================================================================\n";
}

// ====================== NHAN VIEN (ADMIN) ======================
void ConsoleUI::menuNhanVien() {
    while (true) {
        std::cout << "\n--- QUAN LY NHAN VIEN ---\n"
                     "1. Danh sach nhan vien\n"
                     "2. Them nhan vien\n"
                     "3. Sua thong tin nhan vien\n"
                     "4. Cho nhan vien nghi viec (xoa mem)\n"
                     "5. Doi trang thai (Dang lam / Tam nghi)\n"
                     "6. Tim kiem & Loc nhan vien nang cao\n"
                     "7. Sap xep danh sach nhan vien\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::cout << std::left << std::setw(8) << "Ma NV" << std::setw(20) << "Ho ten"
                      << std::setw(14) << "Loai" << std::setw(13) << "SDT"
                      << std::setw(12) << "Trang thai"
                      << std::right << std::setw(14) << "Luong co ban" << "\n";
            std::cout << std::string(82, '-') << "\n";
            for (const auto& nv : app.layDanhSachNV()) {
                std::cout << std::left << std::setw(8) << nv->getMaNV()
                          << std::setw(20) << nv->getHoTen()
                          << std::setw(14) << nv->loaiNhanVien()
                          << std::setw(13) << nv->getSdt()
                          << std::setw(12) << trangThaiToChuoi(nv->getTrangThai())
                          << std::right << std::setw(14) << std::fixed << std::setprecision(0) << nv->getLuongCoBan() << "\n";
            }
        } else if (c == 2) {
            std::string ma = app.sinhMaNVMoi();
            std::cout << "Ma nhan vien tu dong: " << ma << "\n";
            std::string ten = nhapChuoi("Ho ten: ");
            std::string sdt = nhapSDT("SDT: ");
            std::string email = nhapEmail("Email: ");
            std::cout << "Loai nhan vien: 1-Pha che  2-Thu ngan  3-Quan ly  4-Phu vu  5-Bao ve\n";
            int loai = nhapSoNguyen("Chon loai: ");
            std::string ngay = Utils::ngayHomNay();
            std::unique_ptr<NhanVien> nv;
            if (loai == 1) {
                double luongGio = nhapSoThuc("Luong theo gio (VND): ");
                nv = std::make_unique<PhaChe>(ma, ten, sdt, email, ngay, luongGio);
            } else if (loai == 2) {
                double luongGio = nhapSoThuc("Luong theo gio (VND): ");
                double phuCap = nhapSoThuc("Phu cap trach nhiem/thang (VND): ");
                nv = std::make_unique<ThuNgan>(ma, ten, sdt, email, ngay, luongGio, phuCap);
            } else if (loai == 3) {
                double luongCung = nhapSoThuc("Luong cung/thang (VND): ");
                double phuCap = nhapSoThuc("Phu cap quan ly (VND): ");
                nv = std::make_unique<QuanLy>(ma, ten, sdt, email, ngay, luongCung, phuCap);
            } else if (loai == 4) {
                double luongGio = nhapSoThuc("Luong theo gio (VND): ");
                nv = std::make_unique<PhuVu>(ma, ten, sdt, email, ngay, luongGio);
            } else if (loai == 5) {
                double luongCung = nhapSoThuc("Luong cung/thang (VND): ");
                nv = std::make_unique<BaoVe>(ma, ten, sdt, email, ngay, luongCung);
            } else {
                std::cout << "Loai khong hop le!\n"; continue;
            }
            if (app.themNhanVien(std::move(nv))) {
                std::cout << "Da them nhan vien " << ma << "\n";
                // Tự động tạo tài khoản cho NV mới (mã NV làm username, mật khẩu 123456)
                if (quanLyTK.themTaiKhoan(ma, "123456", VaiTro::EMPLOYEE, ma)) {
                    std::cout << "  -> Tai khoan dang nhap: " << ma << " / mat khau: 123456\n";
                }
            }
        } else if (c == 3) {
            std::string ma = nhapChuoi("Nhap ma NV can sua: ");
            NhanVien* nv = app.timNhanVien(ma);
            if (!nv) { std::cout << "Khong tim thay!\n"; continue; }
            std::string ten = nhapChuoi("Ho ten moi (Enter de giu nguyen): ");
            if (!ten.empty()) nv->setHoTen(ten);
            std::string sdt = nhapChuoi("SDT moi (Enter de giu nguyen): ");
            if (!sdt.empty()) {
                if (Utils::isValidSDT(sdt)) nv->setSdt(sdt);
                else std::cout << "  -> SDT khong hop le, giu nguyen.\n";
            }
            std::string email = nhapChuoi("Email moi (Enter de giu nguyen): ");
            if (!email.empty()) {
                if (Utils::isValidEmail(email)) nv->setEmail(email);
                else std::cout << "  -> Email khong hop le, giu nguyen.\n";
            }
            std::cout << "Da cap nhat.\n";
        } else if (c == 4) {
            std::string ma = nhapChuoi("Nhap ma NV: ");
            std::cout << (app.xoaNhanVien(ma) ? "Da cho nghi viec.\n" : "Khong tim thay!\n");
        } else if (c == 5) {
            std::string ma = nhapChuoi("Nhap ma NV: ");
            NhanVien* nv = app.timNhanVien(ma);
            if (!nv) { std::cout << "Khong tim thay!\n"; continue; }
            std::cout << "1-Dang lam  2-Tam nghi\n";
            int t = nhapSoNguyen("Chon: ");
            nv->setTrangThai(t == 2 ? TrangThaiNV::TAM_NGHI : TrangThaiNV::DANG_LAM);
            std::cout << "Da cap nhat trang thai.\n";
        } else if (c == 6) {
            menuTimKiemLocNV();
        } else if (c == 7) {
            menuSapXepNV();
        }
    }
}

// ====================== TÌM KIẾM & LỌC NÂNG CAO ======================
void ConsoleUI::menuTimKiemLocNV() {
    std::cout << "\n--- TIM KIEM & LOC NHAN VIEN NANG CAO ---\n"
                 "(De trong / nhap 0 neu muon bo qua tieu chi)\n";
    BoLocNhanVien bl;

    bl.tuKhoa = nhapChuoi("Tu khoa tim kiem (ten, ma, SDT, email): ");

    std::cout << "Loai NV: 0-Tat ca  1-Pha che  2-Thu ngan  3-Quan ly  4-Phu vu  5-Bao ve\n";
    int loai = nhapSoNguyen("Chon loai: ");
    if (loai == 1) bl.loaiNV = "PHA_CHE";
    else if (loai == 2) bl.loaiNV = "THU_NGAN";
    else if (loai == 3) bl.loaiNV = "QUAN_LY";
    else if (loai == 4) bl.loaiNV = "PHU_VU";
    else if (loai == 5) bl.loaiNV = "BAO_VE";

    std::cout << "Trang thai: 0-Tat ca  1-Dang lam  2-Tam nghi  3-Da nghi viec\n";
    int tt = nhapSoNguyen("Chon trang thai: ");
    if (tt == 1) bl.trangThai = 0;
    else if (tt == 2) bl.trangThai = 1;
    else if (tt == 3) bl.trangThai = 2;
    else bl.trangThai = -1;

    std::string minStr = nhapChuoi("Muc luong co ban toi thieu (VND, Enter de bo qua): ");
    if (!minStr.empty()) {
        try { bl.luongMin = std::stod(minStr); } catch (...) { bl.luongMin = -1; }
    }

    std::string maxStr = nhapChuoi("Muc luong co ban toi da (VND, Enter de bo qua): ");
    if (!maxStr.empty()) {
        try { bl.luongMax = std::stod(maxStr); } catch (...) { bl.luongMax = -1; }
    }

    auto ketQua = app.timKiemVaLoc(bl);
    std::cout << "\n>> Tim thay " << ketQua.size() << " nhan vien phu hop:\n";
    if (ketQua.empty()) {
        std::cout << "  (Khong co nhan vien nao thoa man tieu chi)\n";
        return;
    }

    std::cout << std::left << std::setw(8) << "Ma NV" << std::setw(20) << "Ho ten"
              << std::setw(14) << "Loai" << std::setw(13) << "SDT"
              << std::setw(12) << "Trang thai"
              << std::right << std::setw(14) << "Luong co ban" << "\n";
    std::cout << std::string(82, '-') << "\n";
    for (const auto* nv : ketQua) {
        std::cout << std::left << std::setw(8) << nv->getMaNV()
                  << std::setw(20) << nv->getHoTen()
                  << std::setw(14) << nv->loaiNhanVien()
                  << std::setw(13) << nv->getSdt()
                  << std::setw(12) << trangThaiToChuoi(nv->getTrangThai())
                  << std::right << std::setw(14) << std::fixed << std::setprecision(0) << nv->getLuongCoBan() << "\n";
    }
}

// ====================== SẮP XẾP DANH SÁCH ======================
void ConsoleUI::menuSapXepNV() {
    std::cout << "\n--- SAP XEP DANH SACH NHAN VIEN ---\n"
                 "1. Theo Ho ten (A -> Z)\n"
                 "2. Theo Ho ten (Z -> A)\n"
                 "3. Theo Luong co ban tang dan\n"
                 "4. Theo Luong co ban giam dan\n"
                 "5. Theo Ngay vao lam (lau nam nhat truoc)\n"
                 "6. Theo Ngay vao lam (moi nhat truoc)\n"
                 "7. Theo Ma NV (tang dan)\n"
                 "8. Theo Ma NV (giam dan)\n"
                 "0. Quay lai\n";
    int c = nhapSoNguyen("Chon tieu chi sap xep: ");
    if (c == 0) return;

    TieuChiSapXep tc = TieuChiSapXep::MA_NV_TANG;
    std::string tieuDe = "";
    if (c == 1) { tc = TieuChiSapXep::TEN_TANG; tieuDe = "Ho ten (A -> Z)"; }
    else if (c == 2) { tc = TieuChiSapXep::TEN_GIAM; tieuDe = "Ho ten (Z -> A)"; }
    else if (c == 3) { tc = TieuChiSapXep::LUONG_TANG; tieuDe = "Luong tang dan"; }
    else if (c == 4) { tc = TieuChiSapXep::LUONG_GIAM; tieuDe = "Luong giam dan"; }
    else if (c == 5) { tc = TieuChiSapXep::NGAY_VAO_LAM_CU_NHAT; tieuDe = "Ngay vao lam (lau nam nhat)"; }
    else if (c == 6) { tc = TieuChiSapXep::NGAY_VAO_LAM_MOI_NHAT; tieuDe = "Ngay vao lam (moi nhat)"; }
    else if (c == 7) { tc = TieuChiSapXep::MA_NV_TANG; tieuDe = "Ma NV tang dan"; }
    else if (c == 8) { tc = TieuChiSapXep::MA_NV_GIAM; tieuDe = "Ma NV giam dan"; }
    else { std::cout << "Lua chon khong hop le!\n"; return; }

    std::vector<NhanVien*> ds;
    for (const auto& nv : app.layDanhSachNV()) {
        ds.push_back(nv.get());
    }

    app.sapXepNhanVien(ds, tc);

    std::cout << "\n>> Danh sach nhan vien sau khi sap xep [" << tieuDe << "]:\n";
    std::cout << std::left << std::setw(8) << "Ma NV" << std::setw(20) << "Ho ten"
              << std::setw(14) << "Loai" << std::setw(13) << "Ngay vao"
              << std::setw(12) << "Trang thai"
              << std::right << std::setw(14) << "Luong co ban" << "\n";
    std::cout << std::string(82, '-') << "\n";
    for (const auto* nv : ds) {
        std::cout << std::left << std::setw(8) << nv->getMaNV()
                  << std::setw(20) << nv->getHoTen()
                  << std::setw(14) << nv->loaiNhanVien()
                  << std::setw(13) << nv->getNgayVaoLam()
                  << std::setw(12) << trangThaiToChuoi(nv->getTrangThai())
                  << std::right << std::setw(14) << std::fixed << std::setprecision(0) << nv->getLuongCoBan() << "\n";
    }
}

// ====================== NHAN VIEN (EMPLOYEE) ======================
void ConsoleUI::menuNhanVienEmployee() {
    std::string maNV = quanLyTK.getMaNVHienTai();
    while (true) {
        std::cout << "\n--- THONG TIN CA NHAN ---\n"
                     "1. Xem thong tin cua toi\n"
                     "2. Cap nhat SDT / Email\n"
                     "3. Doi mat khau\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            NhanVien* nv = app.timNhanVien(maNV);
            if (!nv) { std::cout << "Khong tim thay thong tin!\n"; continue; }
            std::cout << "\n--- Thong tin nhan vien ---\n";
            std::cout << "  Ma NV       : " << nv->getMaNV() << "\n";
            std::cout << "  Ho ten      : " << nv->getHoTen() << "\n";
            std::cout << "  SDT         : " << nv->getSdt() << "\n";
            std::cout << "  Email       : " << nv->getEmail() << "\n";
            std::cout << "  Ngay vao lam: " << nv->getNgayVaoLam() << "\n";
            std::cout << "  Loai NV     : " << nv->loaiNhanVien() << "\n";
            std::cout << "  Trang thai  : " << trangThaiToChuoi(nv->getTrangThai()) << "\n";
        } else if (c == 2) {
            NhanVien* nv = app.timNhanVien(maNV);
            if (!nv) { std::cout << "Khong tim thay thong tin!\n"; continue; }
            std::string sdt = nhapChuoi("SDT moi (Enter de giu nguyen): ");
            if (!sdt.empty()) {
                if (Utils::isValidSDT(sdt)) { nv->setSdt(sdt); std::cout << "  -> Da cap nhat SDT.\n"; }
                else std::cout << "  -> SDT khong hop le!\n";
            }
            std::string email = nhapChuoi("Email moi (Enter de giu nguyen): ");
            if (!email.empty()) {
                if (Utils::isValidEmail(email)) { nv->setEmail(email); std::cout << "  -> Da cap nhat email.\n"; }
                else std::cout << "  -> Email khong hop le!\n";
            }
        } else if (c == 3) {
            std::string mkCu = nhapMatKhau("Mat khau cu: ");
            TaiKhoan* tk = quanLyTK.timTaiKhoan(quanLyTK.getTenDangNhapHienTai());
            if (!tk || !tk->kiemTraMatKhau(mkCu)) {
                std::cout << "  -> Mat khau cu khong dung!\n"; continue;
            }
            std::string mkMoi = nhapMatKhau("Mat khau moi: ");
            std::string mkXacNhan = nhapMatKhau("Xac nhan mat khau moi: ");
            if (mkMoi != mkXacNhan) {
                std::cout << "  -> Mat khau xac nhan khong khop!\n"; continue;
            }
            if (mkMoi.size() < 4) {
                std::cout << "  -> Mat khau phai co it nhat 4 ky tu!\n"; continue;
            }
            tk->doiMatKhau(mkMoi);
            std::cout << "  -> Da doi mat khau thanh cong!\n";
        }
    }
}

// ====================== LICH LAM VIEC (ADMIN) ======================
void ConsoleUI::menuLichLamViec() {
    while (true) {
        std::cout << "\n--- LICH LAM VIEC (XEP CA) ---\n"
                     "1. Tao ca lam viec moi\n"
                     "2. Xep nhan vien vao ca\n"
                     "3. Xem lich theo ngay\n"
                     "4. Xem lich cua 1 nhan vien\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::string ngay = nhapNgay("Ngay (dd/mm/yyyy, Enter = hom nay): ", true);
            std::string maCa = nhapChuoi("Ma ca (VD: CA01): ");
            std::string ten = nhapChuoi("Ten ca (Sang/Chieu/Toi): ");
            std::string batDau = nhapGio("Gio bat dau (HH:MM): ");
            std::string ketThuc = nhapGio("Gio ket thuc (HH:MM): ");
            app.layLich().themCa(CaLamViec(maCa, ngay, ten, batDau, ketThuc));
            std::cout << "Da tao ca " << maCa << " ngay " << ngay << "\n";
        } else if (c == 2) {
            std::string ngay = nhapNgay("Ngay (dd/mm/yyyy): ");
            std::string maCa = nhapChuoi("Ma ca: ");
            std::string maNV = nhapChuoi("Ma nhan vien: ");
            if (app.xepCa(ngay, maCa, maNV)) std::cout << "Xep ca thanh cong!\n";
            else std::cout << "That bai: NV khong ton tai, ca khong ton tai, hoac bi trung gio voi ca khac.\n";
        } else if (c == 3) {
            std::string ngay = nhapNgay("Ngay can xem (dd/mm/yyyy, Enter = hom nay): ", true);
            auto ds = app.layLich().xemLichNgay(ngay);
            if (ds.empty()) { std::cout << "Khong co ca nao.\n"; continue; }
            for (const auto& ca : ds) {
                std::cout << "[" << ca.getMaCa() << "] " << ca.getTenCa() << " "
                          << ca.getGioBatDau() << "-" << ca.getGioKetThuc() << " | NV: ";
                for (const auto& m : ca.getDsNhanVien()) std::cout << m << " ";
                std::cout << "\n";
            }
        } else if (c == 4) {
            std::string maNV = nhapChuoi("Ma nhan vien: ");
            auto ds = app.layLich().xemLichNhanVien(maNV);
            if (ds.empty()) { std::cout << "Nhan vien chua co ca nao.\n"; continue; }
            for (const auto& ca : ds) {
                std::cout << ca.getNgay() << " - [" << ca.getMaCa() << "] " << ca.getTenCa()
                          << " " << ca.getGioBatDau() << "-" << ca.getGioKetThuc() << "\n";
            }
        }
    }
}

// ====================== LICH (EMPLOYEE — chỉ xem bản thân) ======================
void ConsoleUI::menuLichEmployee() {
    std::string maNV = quanLyTK.getMaNVHienTai();
    std::cout << "\n--- LICH LAM VIEC CUA TOI (" << maNV << ") ---\n";
    auto ds = app.layLich().xemLichNhanVien(maNV);
    if (ds.empty()) {
        std::cout << "  Chua co ca nao duoc xep.\n";
    } else {
        for (const auto& ca : ds) {
            std::cout << "  " << ca.getNgay() << " - [" << ca.getMaCa() << "] " << ca.getTenCa()
                      << " " << ca.getGioBatDau() << "-" << ca.getGioKetThuc() << "\n";
        }
    }
}

// ====================== CHAM CONG (ADMIN) ======================
void ConsoleUI::menuChamCong() {
    while (true) {
        std::cout << "\n--- CHAM CONG / GIO LAM ---\n"
                     "1. Cham cong VAO\n"
                     "2. Cham cong RA\n"
                     "3. Xem lich su cham cong cua 1 nhan vien\n"
                     "4. Xem tong gio lam trong thang\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::string ma = nhapChuoi("Ma nhan vien: ");
            std::cout << (app.chamCongVao(ma) ? "Cham cong VAO thanh cong!\n"
                                               : "That bai (NV khong ton tai hoac da cham vao roi).\n");
        } else if (c == 2) {
            std::string ma = nhapChuoi("Ma nhan vien: ");
            std::cout << (app.chamCongRa(ma) ? "Cham cong RA thanh cong!\n"
                                              : "That bai (nhan vien chua cham vao).\n");
        } else if (c == 3) {
            std::string ma = nhapChuoi("Ma nhan vien: ");
            auto ds = app.layChamCong().lichSuNhanVien(ma);
            if (ds.empty()) { std::cout << "Chua co du lieu.\n"; continue; }
            for (const auto& bg : ds) {
                std::cout << bg.ngay << "  Vao: " << bg.gioVao
                          << "  Ra: " << (bg.gioRa.empty() ? "(dang lam)" : bg.gioRa) << "\n";
            }
        } else if (c == 4) {
            std::string ma = nhapChuoi("Ma nhan vien: ");
            std::string thang = nhapChuoi("Thang (mm/yyyy, Enter = thang hien tai): ");
            if (thang.empty()) thang = Utils::thangNamHienTai();
            double gio = app.layChamCong().tongGioLamThang(ma, thang);
            std::cout << "Tong gio lam thang " << thang << ": " << gio << " gio\n";
        }
    }
}

// ====================== CHAM CONG (EMPLOYEE — chỉ bản thân) ======================
void ConsoleUI::menuChamCongEmployee() {
    std::string maNV = quanLyTK.getMaNVHienTai();
    while (true) {
        std::cout << "\n--- CHAM CONG CUA TOI (" << maNV << ") ---\n"
                     "1. Cham cong VAO\n"
                     "2. Cham cong RA\n"
                     "3. Xem lich su cham cong cua toi\n"
                     "4. Xem tong gio lam trong thang\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::cout << (app.chamCongVao(maNV) ? "Cham cong VAO thanh cong!\n"
                                                 : "That bai (da cham vao roi hoac loi).\n");
        } else if (c == 2) {
            std::cout << (app.chamCongRa(maNV) ? "Cham cong RA thanh cong!\n"
                                                : "That bai (chua cham vao).\n");
        } else if (c == 3) {
            auto ds = app.layChamCong().lichSuNhanVien(maNV);
            if (ds.empty()) { std::cout << "Chua co du lieu.\n"; continue; }
            for (const auto& bg : ds) {
                std::cout << bg.ngay << "  Vao: " << bg.gioVao
                          << "  Ra: " << (bg.gioRa.empty() ? "(dang lam)" : bg.gioRa) << "\n";
            }
        } else if (c == 4) {
            std::string thang = nhapChuoi("Thang (mm/yyyy, Enter = thang hien tai): ");
            if (thang.empty()) thang = Utils::thangNamHienTai();
            double gio = app.layChamCong().tongGioLamThang(maNV, thang);
            std::cout << "Tong gio lam thang " << thang << ": " << gio << " gio\n";
        }
    }
}

// ====================== TINH LUONG (ADMIN) ======================
void ConsoleUI::menuTinhLuong() {
    while (true) {
        std::cout << "\n--- TINH LUONG ---\n"
                     "1. Tinh luong 1 nhan vien theo thang\n"
                     "2. Xem bang luong toan bo nhan vien theo thang\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::string ma = nhapChuoi("Ma nhan vien: ");
            std::string thang = nhapChuoi("Thang (mm/yyyy, Enter = thang hien tai): ");
            if (thang.empty()) thang = Utils::thangNamHienTai();
            double luong = app.tinhLuongThang(ma, thang);
            if (luong < 0) std::cout << "Khong tim thay nhan vien!\n";
            else std::cout << "Luong thang " << thang << " cua " << ma << ": "
                            << std::fixed << std::setprecision(0) << luong << " VND\n";
        } else if (c == 2) {
            std::string thang = nhapChuoi("Thang (mm/yyyy, Enter = thang hien tai): ");
            if (thang.empty()) thang = Utils::thangNamHienTai();
            auto ds = app.bangLuongThang(thang);
            std::cout << std::left << std::setw(10) << "Ma NV" << std::setw(12) << "Gio lam"
                      << "Thuc linh (VND)\n";
            std::cout << std::string(40, '-') << "\n";
            double tong = 0;
            for (const auto& bg : ds) {
                std::cout << std::left << std::setw(10) << bg.maNV
                          << std::setw(12) << std::fixed << std::setprecision(1) << bg.soGioLam
                          << std::fixed << std::setprecision(0) << bg.thucLinh << "\n";
                tong += bg.thucLinh;
            }
            std::cout << std::string(40, '-') << "\nTong quy luong thang: "
                      << std::fixed << std::setprecision(0) << tong << " VND\n";
        }
    }
}

// ====================== LUONG (EMPLOYEE — chỉ bản thân) ======================
void ConsoleUI::menuLuongEmployee() {
    std::string maNV = quanLyTK.getMaNVHienTai();
    std::string thang = nhapChuoi("Thang (mm/yyyy, Enter = thang hien tai): ");
    if (thang.empty()) thang = Utils::thangNamHienTai();
    double luong = app.tinhLuongThang(maNV, thang);
    if (luong < 0) std::cout << "Khong the tinh luong!\n";
    else {
        double gio = app.layChamCong().tongGioLamThang(maNV, thang);
        std::cout << "\n--- LUONG THANG " << thang << " ---\n";
        std::cout << "  Ma NV     : " << maNV << "\n";
        std::cout << "  Gio lam   : " << std::fixed << std::setprecision(1) << gio << " gio\n";
        std::cout << "  Thuc linh : " << std::fixed << std::setprecision(0) << luong << " VND\n";
    }
}

// ====================== QUAN LY TAI KHOAN (ADMIN) ======================
void ConsoleUI::menuQuanLyTaiKhoan() {
    while (true) {
        std::cout << "\n--- QUAN LY TAI KHOAN ---\n"
                     "1. Danh sach tai khoan\n"
                     "2. Them tai khoan\n"
                     "3. Reset mat khau (ve 123456)\n"
                     "4. Xoa tai khoan\n"
                     "5. Doi mat khau cua toi\n"
                     "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ");
        if (c == 0) return;

        if (c == 1) {
            std::cout << std::left << std::setw(18) << "Ten dang nhap"
                      << std::setw(12) << "Vai tro"
                      << "Ma NV\n";
            std::cout << std::string(42, '-') << "\n";
            for (const auto& tk : quanLyTK.layDanhSachTK()) {
                std::cout << std::left << std::setw(18) << tk.getTenDangNhap()
                          << std::setw(12) << vaiTroToChuoi(tk.getVaiTro())
                          << tk.getMaNV() << "\n";
            }
        } else if (c == 2) {
            std::string tenDN = nhapChuoi("Ten dang nhap: ");
            std::string matKhau = nhapMatKhau("Mat khau: ");
            std::cout << "Vai tro: 1-ADMIN  2-EMPLOYEE\n";
            int vt = nhapSoNguyen("Chon: ");
            VaiTro vaiTro = (vt == 1) ? VaiTro::ADMIN : VaiTro::EMPLOYEE;
            std::string maNV = "";
            if (vaiTro == VaiTro::EMPLOYEE) {
                maNV = nhapChuoi("Ma nhan vien lien ket: ");
            }
            if (quanLyTK.themTaiKhoan(tenDN, matKhau, vaiTro, maNV))
                std::cout << "Da tao tai khoan " << tenDN << ".\n";
            else
                std::cout << "That bai! Ten dang nhap da ton tai.\n";
        } else if (c == 3) {
            std::string tenDN = nhapChuoi("Ten dang nhap can reset: ");
            if (quanLyTK.resetMatKhau(tenDN))
                std::cout << "Da reset mat khau cua " << tenDN << " ve 123456.\n";
            else
                std::cout << "Khong tim thay tai khoan!\n";
        } else if (c == 4) {
            std::string tenDN = nhapChuoi("Ten dang nhap can xoa: ");
            if (quanLyTK.xoaTaiKhoan(tenDN))
                std::cout << "Da xoa tai khoan " << tenDN << ".\n";
            else
                std::cout << "That bai! Khong tim thay hoac dang dang nhap bang tai khoan nay.\n";
        } else if (c == 5) {
            std::string mkCu = nhapMatKhau("Mat khau cu: ");
            TaiKhoan* tk = quanLyTK.timTaiKhoan(quanLyTK.getTenDangNhapHienTai());
            if (!tk || !tk->kiemTraMatKhau(mkCu)) {
                std::cout << "Mat khau cu khong dung!\n"; continue;
            }
            std::string mkMoi = nhapMatKhau("Mat khau moi: ");
            std::string mkXN = nhapMatKhau("Xac nhan mat khau moi: ");
            if (mkMoi != mkXN) { std::cout << "Mat khau xac nhan khong khop!\n"; continue; }
            if (mkMoi.size() < 4) { std::cout << "Mat khau phai co it nhat 4 ky tu!\n"; continue; }
            tk->doiMatKhau(mkMoi);
            std::cout << "Da doi mat khau thanh cong!\n";
        }
    }
}

// ========================= MENU CHINH (ADMIN) =========================
void ConsoleUI::hienMenuChinh() {
    std::cout << "\n###################################################\n"
                 "#      QUAN LY NHAN VIEN QUAN CA PHE - C++        #\n"
                 "#  [" << std::left << std::setw(12) << quanLyTK.getTenDangNhapHienTai()
              << " | " << std::setw(10) << vaiTroToChuoi(quanLyTK.getVaiTroHienTai())
              << "]                    #\n"
                 "###################################################\n"
                 "1. Dashboard & Bao cao\n"
                 "2. Quan ly thong tin nhan vien\n"
                 "3. Lich lam viec (xep ca)\n"
                 "4. Cham cong / gio lam\n"
                 "5. Tinh luong\n"
                 "6. Quan ly tai khoan\n"
                 "7. Luu du lieu\n"
                 "8. Dang xuat\n"
                 "0. Luu & Thoat\n";
}

// ========================= MENU EMPLOYEE =========================
void ConsoleUI::hienMenuEmployee() {
    std::string maNV = quanLyTK.getMaNVHienTai();
    NhanVien* nv = app.timNhanVien(maNV);
    std::string tenHienThi = nv ? nv->getHoTen() : maNV;

    std::cout << "\n###################################################\n"
                 "#      QUAN LY NHAN VIEN QUAN CA PHE - C++        #\n"
                 "#  Xin chao, " << std::left << std::setw(37) << tenHienThi << "#\n"
                 "###################################################\n"
                 "1. Thong tin ca nhan & Doi mat khau\n"
                 "2. Xem lich lam viec cua toi\n"
                 "3. Cham cong\n"
                 "4. Xem luong cua toi\n"
                 "5. Dang xuat\n"
                 "0. Thoat\n";
}

// ========================= VÒNG LẶP CHÍNH =========================
void ConsoleUI::chay() {
    // === Màn hình đăng nhập ===
    while (true) {
        if (manHinhDangNhap()) break;
        std::cout << "Nhan Enter de thu lai...";
        std::cin.get();
    }

    // === Menu theo vai trò ===
    if (quanLyTK.laAdmin()) {
        // ---- ADMIN ----
        while (true) {
            hienMenuChinh();
            int c = nhapSoNguyen("Chon chuc nang: ");
            switch (c) {
                case 1: hienDashboard(); break;
                case 2: menuNhanVien(); break;
                case 3: menuLichLamViec(); break;
                case 4: menuChamCong(); break;
                case 5: menuTinhLuong(); break;
                case 6: menuQuanLyTaiKhoan(); break;
                case 7:
                    app.luuTatCa(thuMucDuLieu);
                    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
                    std::cout << "Da luu du lieu vao thu muc '" << thuMucDuLieu << "'.\n";
                    break;
                case 8:
                    app.luuTatCa(thuMucDuLieu);
                    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
                    quanLyTK.dangXuat();
                    std::cout << "Da dang xuat.\n";
                    chay(); // quay lại màn hình đăng nhập
                    return;
                case 0:
                    app.luuTatCa(thuMucDuLieu);
                    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
                    std::cout << "Da luu du lieu. Tam biet!\n";
                    return;
                default:
                    std::cout << "Lua chon khong hop le!\n";
            }
        }
    } else {
        // ---- EMPLOYEE ----
        while (true) {
            hienMenuEmployee();
            int c = nhapSoNguyen("Chon chuc nang: ");
            switch (c) {
                case 1: menuNhanVienEmployee(); break;
                case 2: menuLichEmployee(); break;
                case 3: menuChamCongEmployee(); break;
                case 4: menuLuongEmployee(); break;
                case 5:
                    app.luuTatCa(thuMucDuLieu);
                    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
                    quanLyTK.dangXuat();
                    std::cout << "Da dang xuat.\n";
                    chay(); // quay lại màn hình đăng nhập
                    return;
                case 0:
                    app.luuTatCa(thuMucDuLieu);
                    quanLyTK.luuFile(thuMucDuLieu + "/taikhoan.csv");
                    std::cout << "Tam biet!\n";
                    return;
                default:
                    std::cout << "Lua chon khong hop le!\n";
            }
        }
    }
}

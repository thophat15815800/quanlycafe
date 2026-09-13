#include "TaiKhoan.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <functional>

// ============ Chuyển đổi VaiTro <-> chuỗi ============
std::string vaiTroToChuoi(VaiTro vt) {
    switch (vt) {
        case VaiTro::ADMIN: return "ADMIN";
        case VaiTro::EMPLOYEE: return "EMPLOYEE";
    }
    return "EMPLOYEE";
}

VaiTro chuoiToVaiTro(const std::string& s) {
    if (s == "ADMIN") return VaiTro::ADMIN;
    return VaiTro::EMPLOYEE;
}

// ============ Hash mật khẩu đơn giản ============
// Dùng std::hash (không bảo mật cao, nhưng đủ cho bài tập)
std::string TaiKhoan::hashMatKhau(const std::string& matKhauGoc) {
    std::hash<std::string> hasher;
    size_t h = hasher(matKhauGoc);
    // Chuyển hash thành chuỗi hex
    char buf[32];
    snprintf(buf, sizeof(buf), "%016zx", h);
    return std::string(buf);
}

// ============ TaiKhoan ============
TaiKhoan::TaiKhoan(std::string tenDangNhap, const std::string& matKhauGoc,
                     VaiTro vaiTro, std::string maNV)
    : tenDangNhap(std::move(tenDangNhap)),
      matKhau(hashMatKhau(matKhauGoc)),
      vaiTro(vaiTro),
      maNV(std::move(maNV)) {}

std::string TaiKhoan::getTenDangNhap() const { return tenDangNhap; }
VaiTro TaiKhoan::getVaiTro() const { return vaiTro; }
std::string TaiKhoan::getMaNV() const { return maNV; }

bool TaiKhoan::kiemTraMatKhau(const std::string& matKhauGoc) const {
    return matKhau == hashMatKhau(matKhauGoc);
}

void TaiKhoan::doiMatKhau(const std::string& matKhauMoi) {
    matKhau = hashMatKhau(matKhauMoi);
}

std::string TaiKhoan::toCSV() const {
    return tenDangNhap + "," + matKhau + "," + vaiTroToChuoi(vaiTro) + "," + maNV;
}

TaiKhoan TaiKhoan::fromCSV(const std::string& line) {
    auto p = Utils::splitAndTrim(line, ',');
    TaiKhoan tk;
    if (p.size() < 3) return tk;
    tk.tenDangNhap = p[0];
    tk.matKhau = p[1];         // đã là hash, không hash lại
    tk.vaiTro = chuoiToVaiTro(p[2]);
    tk.maNV = (p.size() >= 4) ? p[3] : "";
    return tk;
}

// ============ QuanLyTaiKhoan ============
bool QuanLyTaiKhoan::dangNhap(const std::string& tenDN, const std::string& matKhau) {
    for (auto& tk : danhSachTK) {
        if (tk.getTenDangNhap() == tenDN && tk.kiemTraMatKhau(matKhau)) {
            tkHienTai = &tk;
            return true;
        }
    }
    return false;
}

void QuanLyTaiKhoan::dangXuat() {
    tkHienTai = nullptr;
}

bool QuanLyTaiKhoan::daDangNhap() const {
    return tkHienTai != nullptr;
}

bool QuanLyTaiKhoan::laAdmin() const {
    return daDangNhap() && tkHienTai->getVaiTro() == VaiTro::ADMIN;
}

std::string QuanLyTaiKhoan::getMaNVHienTai() const {
    return daDangNhap() ? tkHienTai->getMaNV() : "";
}

std::string QuanLyTaiKhoan::getTenDangNhapHienTai() const {
    return daDangNhap() ? tkHienTai->getTenDangNhap() : "";
}

VaiTro QuanLyTaiKhoan::getVaiTroHienTai() const {
    return daDangNhap() ? tkHienTai->getVaiTro() : VaiTro::EMPLOYEE;
}

bool QuanLyTaiKhoan::themTaiKhoan(const std::string& tenDN, const std::string& matKhau,
                                    VaiTro vaiTro, const std::string& maNV) {
    // Kiểm tra trùng tên đăng nhập
    for (const auto& tk : danhSachTK) {
        if (tk.getTenDangNhap() == tenDN) return false;
    }
    danhSachTK.emplace_back(tenDN, matKhau, vaiTro, maNV);
    return true;
}

bool QuanLyTaiKhoan::xoaTaiKhoan(const std::string& tenDN) {
    for (auto it = danhSachTK.begin(); it != danhSachTK.end(); ++it) {
        if (it->getTenDangNhap() == tenDN) {
            // Không cho xóa tài khoản đang đăng nhập
            if (tkHienTai == &(*it)) return false;
            danhSachTK.erase(it);
            return true;
        }
    }
    return false;
}

bool QuanLyTaiKhoan::doiMatKhau(const std::string& tenDN, const std::string& matKhauMoi) {
    TaiKhoan* tk = timTaiKhoan(tenDN);
    if (!tk) return false;
    tk->doiMatKhau(matKhauMoi);
    return true;
}

bool QuanLyTaiKhoan::resetMatKhau(const std::string& tenDN) {
    return doiMatKhau(tenDN, "123456");
}

const std::vector<TaiKhoan>& QuanLyTaiKhoan::layDanhSachTK() const {
    return danhSachTK;
}

TaiKhoan* QuanLyTaiKhoan::timTaiKhoan(const std::string& tenDN) {
    for (auto& tk : danhSachTK) {
        if (tk.getTenDangNhap() == tenDN) return &tk;
    }
    return nullptr;
}

bool QuanLyTaiKhoan::luuFile(const std::string& duongDan) const {
    std::ofstream f(duongDan);
    if (!f.is_open()) {
        return false;
    }
    for (const auto& tk : danhSachTK) {
        f << tk.toCSV() << "\n";
    }
    return true;
}

void QuanLyTaiKhoan::taiFile(const std::string& duongDan) {
    danhSachTK.clear();
    tkHienTai = nullptr;
    std::ifstream f(duongDan);
    if (!f.is_open()) {
        taoDuLieuMacDinh();
        return;
    }
    std::string line;
    while (std::getline(f, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;
        auto tk = TaiKhoan::fromCSV(line);
        if (!tk.getTenDangNhap().empty()) {
            danhSachTK.push_back(tk);
        }
    }
    // Nếu file rỗng, tạo dữ liệu mặc định
    if (danhSachTK.empty()) {
        taoDuLieuMacDinh();
    }
}

void QuanLyTaiKhoan::dongBoTaiKhoanNhanVien(const std::vector<std::string>& dsMaNV) {
    for (const auto& maNV : dsMaNV) {
        if (maNV.empty()) continue;
        if (!timTaiKhoan(maNV)) {
            themTaiKhoan(maNV, "123456", VaiTro::EMPLOYEE, maNV);
        }
    }
}

void QuanLyTaiKhoan::taoDuLieuMacDinh() {
    // Tạo tài khoản admin mặc định
    themTaiKhoan("admin", "admin123", VaiTro::ADMIN, "");
}

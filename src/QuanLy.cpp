#include "QuanLy.h"

QuanLy::QuanLy(std::string maNV, std::string hoTen, std::string sdt,
               std::string email, std::string ngayVaoLam, double luongCung,
               double phuCapQuanLy, std::string avatar)
    : NhanVien(std::move(maNV), std::move(hoTen), std::move(sdt), std::move(email),
               std::move(ngayVaoLam), luongCung, std::move(avatar)),
      phuCapQuanLy(phuCapQuanLy) {}

double QuanLy::getLuongCung() const { return getLuongCoBan(); }
double QuanLy::getPhuCapQuanLy() const { return phuCapQuanLy; }
void QuanLy::setPhuCapQuanLy(double pc) { if (pc >= 0) phuCapQuanLy = pc; }

double QuanLy::tinhLuong(double soGioLam) const {
    // Lương cứng + phụ cấp; nếu số giờ làm vượt chuẩn (>200h) thưởng thêm nhẹ
    double luong = getLuongCoBan() + phuCapQuanLy;
    if (soGioLam > 200) luong += (soGioLam - 200) * 20000;
    return luong;
}

std::string QuanLy::loaiNhanVien() const { return "QUAN_LY"; }

void QuanLy::hienThiThongTin() const {
    NhanVien::hienThiThongTin();
}

std::string QuanLy::toCSV() const {
    return NhanVien::toCSV() + "," + std::to_string(phuCapQuanLy) + "," + avatar;
}

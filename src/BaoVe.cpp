#include "BaoVe.h"
#include <iostream>

BaoVe::BaoVe(std::string maNV, std::string hoTen, std::string sdt,
             std::string email, std::string ngayVaoLam, double luongCung,
             std::string avatar)
    : NhanVien(std::move(maNV), std::move(hoTen), std::move(sdt), std::move(email),
               std::move(ngayVaoLam), luongCung, std::move(avatar)) {}

double BaoVe::getLuongCung() const { return getLuongCoBan(); }
void BaoVe::setLuongCung(double luong) { setLuongCoBan(luong); }

double BaoVe::tinhLuong(double soGioLam) const {
    (void)soGioLam; // Bảo vệ chỉ có lương cứng, không tính theo giờ
    return getLuongCoBan();
}

std::string BaoVe::loaiNhanVien() const { return "BAO_VE"; }

void BaoVe::hienThiThongTin() const {
    NhanVien::hienThiThongTin();
}

std::string BaoVe::toCSV() const {
    return NhanVien::toCSV() + "," + avatar;
}

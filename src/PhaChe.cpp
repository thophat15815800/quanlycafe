#include "PhaChe.h"
#include <iostream>
#include <iomanip>

PhaChe::PhaChe(std::string maNV, std::string hoTen, std::string sdt,
               std::string email, std::string ngayVaoLam, double luongTheoGio)
    : NhanVien(std::move(maNV), std::move(hoTen), std::move(sdt), std::move(email),
               std::move(ngayVaoLam), luongTheoGio) {}

double PhaChe::getLuongTheoGio() const { return getLuongCoBan(); }
void PhaChe::setLuongTheoGio(double luong) { setLuongCoBan(luong); }

double PhaChe::tinhLuong(double soGioLam) const {
    return soGioLam * getLuongCoBan();
}

std::string PhaChe::loaiNhanVien() const { return "PHA_CHE"; }

void PhaChe::hienThiThongTin() const {
    NhanVien::hienThiThongTin();
}

std::string PhaChe::toCSV() const {
    return NhanVien::toCSV();
}

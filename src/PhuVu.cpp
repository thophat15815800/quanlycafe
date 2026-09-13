#include "PhuVu.h"
#include <iostream>

PhuVu::PhuVu(std::string maNV, std::string hoTen, std::string sdt,
             std::string email, std::string ngayVaoLam, double luongTheoGio)
    : NhanVien(std::move(maNV), std::move(hoTen), std::move(sdt), std::move(email),
               std::move(ngayVaoLam), luongTheoGio) {}

double PhuVu::getLuongTheoGio() const { return getLuongCoBan(); }
void PhuVu::setLuongTheoGio(double luong) { setLuongCoBan(luong); }

double PhuVu::tinhLuong(double soGioLam) const {
    return soGioLam * getLuongCoBan();
}

std::string PhuVu::loaiNhanVien() const { return "PHU_VU"; }

void PhuVu::hienThiThongTin() const {
    NhanVien::hienThiThongTin();
}

std::string PhuVu::toCSV() const {
    return NhanVien::toCSV();
}

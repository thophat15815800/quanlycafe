#include "ThuNgan.h"
#include <iostream>

ThuNgan::ThuNgan(std::string maNV, std::string hoTen, std::string sdt,
                  std::string email, std::string ngayVaoLam, double luongTheoGio,
                  double phuCapTrachNhiem)
    : NhanVien(std::move(maNV), std::move(hoTen), std::move(sdt), std::move(email),
               std::move(ngayVaoLam), luongTheoGio),
      phuCapTrachNhiem(phuCapTrachNhiem) {}

double ThuNgan::getLuongTheoGio() const { return getLuongCoBan(); }
double ThuNgan::getPhuCap() const { return phuCapTrachNhiem; }
void ThuNgan::setPhuCap(double pc) { if (pc >= 0) phuCapTrachNhiem = pc; }

double ThuNgan::tinhLuong(double soGioLam) const {
    return soGioLam * getLuongCoBan() + phuCapTrachNhiem;
}

std::string ThuNgan::loaiNhanVien() const { return "THU_NGAN"; }

void ThuNgan::hienThiThongTin() const {
    NhanVien::hienThiThongTin();
}

std::string ThuNgan::toCSV() const {
    return NhanVien::toCSV() + "," + std::to_string(phuCapTrachNhiem);
}

#include "NhanVien.h"
#include <iostream>
#include <iomanip>

std::string trangThaiToChuoi(TrangThaiNV tt) {
    switch (tt) {
        case TrangThaiNV::DANG_LAM: return "DANG_LAM";
        case TrangThaiNV::TAM_NGHI: return "TAM_NGHI";
        case TrangThaiNV::DA_NGHI_VIEC: return "DA_NGHI_VIEC";
    }
    return "DANG_LAM";
}

TrangThaiNV chuoiToTrangThai(const std::string& s) {
    if (s == "TAM_NGHI") return TrangThaiNV::TAM_NGHI;
    if (s == "DA_NGHI_VIEC") return TrangThaiNV::DA_NGHI_VIEC;
    return TrangThaiNV::DANG_LAM;
}

NhanVien::NhanVien(std::string maNV, std::string hoTen, std::string sdt,
                   std::string email, std::string ngayVaoLam, double luongCoBan,
                   std::string avatar)
    : maNV(std::move(maNV)), hoTen(std::move(hoTen)), sdt(std::move(sdt)),
      email(std::move(email)), ngayVaoLam(std::move(ngayVaoLam)),
      luongCoBan(luongCoBan), trangThai(TrangThaiNV::DANG_LAM),
      avatar(std::move(avatar)) {}

std::string NhanVien::getMaNV() const { return maNV; }
std::string NhanVien::getHoTen() const { return hoTen; }
void NhanVien::setHoTen(const std::string& ten) { if (!ten.empty()) hoTen = ten; }
std::string NhanVien::getSdt() const { return sdt; }
void NhanVien::setSdt(const std::string& s) { if (!s.empty()) sdt = s; }
std::string NhanVien::getEmail() const { return email; }
void NhanVien::setEmail(const std::string& e) { email = e; }
std::string NhanVien::getNgayVaoLam() const { return ngayVaoLam; }
double NhanVien::getLuongCoBan() const { return luongCoBan; }
void NhanVien::setLuongCoBan(double luong) { if (luong >= 0) luongCoBan = luong; }
TrangThaiNV NhanVien::getTrangThai() const { return trangThai; }
void NhanVien::setTrangThai(TrangThaiNV tt) { trangThai = tt; }
std::string NhanVien::getAvatar() const { return avatar; }
void NhanVien::setAvatar(const std::string& avt) { avatar = avt; }

void NhanVien::hienThiThongTin() const {
    std::cout << std::left
              << std::setw(8) << maNV
              << std::setw(20) << hoTen
              << std::setw(14) << loaiNhanVien()
              << std::setw(13) << sdt
              << std::setw(12) << trangThaiToChuoi(trangThai)
              << std::endl;
}

std::string NhanVien::toCSV() const {
    return maNV + "," + hoTen + "," + sdt + "," + email + "," + ngayVaoLam + "," +
           std::to_string(luongCoBan) + "," + trangThaiToChuoi(trangThai) + "," + loaiNhanVien();
}

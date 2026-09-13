#include "FileManager.h"
#include "PhaChe.h"
#include "ThuNgan.h"
#include "PhuVu.h"
#include "BaoVe.h"
#include "QuanLy.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

bool FileManager::luuNhanVien(const std::vector<std::unique_ptr<NhanVien>>& ds,
                               const std::string& duongDan) {
    std::ofstream f(duongDan);
    if (!f.is_open()) {
        return false;
    }
    for (const auto& nv : ds) {
        f << nv->toCSV() << "\n"; // gọi đa hình toCSV() của từng lớp con
    }
    return true;
}

// Cột: maNV,hoTen,sdt,email,ngayVaoLam,luongCoBan,trangThai,loai[,phuCap]
std::unique_ptr<NhanVien> FileManager::taoTuDong(const std::string& line) {
    auto p = Utils::splitAndTrim(line, ',');
    if (p.size() < 8) return nullptr;

    std::string maNV = p[0], hoTen = p[1], sdt = p[2], email = p[3], ngay = p[4];
    double luongCoBan = 0;
    try { luongCoBan = std::stod(p[5]); } catch (...) { luongCoBan = 0; }
    TrangThaiNV tt = chuoiToTrangThai(p[6]);
    std::string loai = p[7];

    std::unique_ptr<NhanVien> nv;
    if (loai == "PHA_CHE") {
        nv = std::make_unique<PhaChe>(maNV, hoTen, sdt, email, ngay, luongCoBan);
    } else if (loai == "THU_NGAN") {
        double phuCap = (p.size() >= 9) ? std::stod(p[8]) : 0;
        nv = std::make_unique<ThuNgan>(maNV, hoTen, sdt, email, ngay, luongCoBan, phuCap);
    } else if (loai == "QUAN_LY") {
        double phuCap = (p.size() >= 9) ? std::stod(p[8]) : 0;
        nv = std::make_unique<QuanLy>(maNV, hoTen, sdt, email, ngay, luongCoBan, phuCap);
    } else if (loai == "PHU_VU") {
        nv = std::make_unique<PhuVu>(maNV, hoTen, sdt, email, ngay, luongCoBan);
    } else if (loai == "BAO_VE") {
        nv = std::make_unique<BaoVe>(maNV, hoTen, sdt, email, ngay, luongCoBan);
    } else {
        return nullptr;
    }
    nv->setTrangThai(tt);
    return nv;
}

std::vector<std::unique_ptr<NhanVien>> FileManager::taiNhanVien(const std::string& duongDan) {
    std::vector<std::unique_ptr<NhanVien>> ket;
    std::ifstream f(duongDan);
    if (!f.is_open()) return ket; // file chưa tồn tại -> trả về danh sách rỗng
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto nv = taoTuDong(line);
        if (nv) ket.push_back(std::move(nv));
    }
    return ket;
}

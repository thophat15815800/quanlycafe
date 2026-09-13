#include "BangLuong.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>

double BangLuong::tinhVaLuu(const NhanVien* nv, double soGioLam, const std::string& thangNam) {
    double luong = nv->tinhLuong(soGioLam); 
    for (auto& bg : lichSu) {
        if (bg.maNV == nv->getMaNV() && bg.thangNam == thangNam) {
            bg.soGioLam = soGioLam;
            bg.thucLinh = luong;
            return luong;
        }
    }
    lichSu.push_back({nv->getMaNV(), thangNam, soGioLam, luong});
    return luong;
}

std::vector<BanGhiLuong> BangLuong::layTheoThang(const std::string& thangNam) const {
    std::vector<BanGhiLuong> ket;
    for (const auto& bg : lichSu) if (bg.thangNam == thangNam) ket.push_back(bg);
    return ket;
}

std::vector<BanGhiLuong> BangLuong::layTheoNhanVien(const std::string& maNV) const {
    std::vector<BanGhiLuong> ket;
    for (const auto& bg : lichSu) if (bg.maNV == maNV) ket.push_back(bg);
    return ket;
}

void BangLuong::luuFile(const std::string& duongDan) const {
    std::ofstream f(duongDan);
    for (const auto& bg : lichSu) {
        f << bg.maNV << "," << bg.thangNam << "," << bg.soGioLam << "," << bg.thucLinh << "\n";
    }
}

void BangLuong::taiFile(const std::string& duongDan) {
    lichSu.clear();
    std::ifstream f(duongDan);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto p = Utils::splitAndTrim(line, ',');
        if (p.size() < 4) continue;
        BanGhiLuong bg;
        bg.maNV = p[0]; bg.thangNam = p[1];
        try { bg.soGioLam = std::stod(p[2]); bg.thucLinh = std::stod(p[3]); }
        catch (...) { continue; }
        lichSu.push_back(bg);
    }
}

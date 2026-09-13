#include "ChamCong.h"
#include "Utils.h"
#include <fstream>

bool ChamCong::checkIn(const std::string& maNV, const std::string& ngay, const std::string& gio,
                       const std::string& maCa, const std::string& trangThai) {
    if (dangTrongCa(maNV)) return false; // đã check-in rồi, chưa check-out
    BanGhiChamCong bg{maNV, ngay, gio, "", maCa, trangThai};
    lichSu.push_back(bg);
    dangMoCa[maNV] = (int)lichSu.size() - 1;
    return true;
}

bool ChamCong::checkOut(const std::string& maNV, const std::string& ngay, const std::string& gio,
                        const std::string& trangThaiMoi) {
    (void)ngay; // ngay không dùng vì tìm bản ghi mở qua map dangMoCa
    auto it = dangMoCa.find(maNV);
    if (it == dangMoCa.end()) return false; // chưa check-in
    lichSu[it->second].gioRa = gio;
    if (!trangThaiMoi.empty()) {
        lichSu[it->second].trangThai = trangThaiMoi;
    }
    dangMoCa.erase(it);
    return true;
}

bool ChamCong::dangTrongCa(const std::string& maNV) const {
    return dangMoCa.find(maNV) != dangMoCa.end();
}

const BanGhiChamCong* ChamCong::layBanGhiDangMo(const std::string& maNV) const {
    auto it = dangMoCa.find(maNV);
    if (it == dangMoCa.end()) return nullptr;
    if (it->second >= 0 && it->second < (int)lichSu.size()) {
        return &lichSu[it->second];
    }
    return nullptr;
}

double ChamCong::tongGioLamThang(const std::string& maNV, const std::string& thangNam) const {
    double tongPhut = 0;
    for (const auto& bg : lichSu) {
        if (bg.maNV != maNV || bg.gioRa.empty()) continue;
        if (Utils::layThangNamTuNgay(bg.ngay) != thangNam) continue;
        int vao = Utils::gioToPhut(bg.gioVao);
        int ra = Utils::gioToPhut(bg.gioRa);
        if (vao >= 0 && ra >= 0 && ra > vao) tongPhut += (ra - vao);
    }
    return tongPhut / 60.0;
}

std::vector<BanGhiChamCong> ChamCong::lichSuNhanVien(const std::string& maNV) const {
    std::vector<BanGhiChamCong> ket;
    for (const auto& bg : lichSu) if (bg.maNV == maNV) ket.push_back(bg);
    return ket;
}

std::vector<BanGhiChamCong> ChamCong::lichSuNgay(const std::string& ngay) const {
    std::vector<BanGhiChamCong> ket;
    for (const auto& bg : lichSu) if (bg.ngay == ngay) ket.push_back(bg);
    return ket;
}

int ChamCong::soNhanVienDangLam() const {
    return (int)dangMoCa.size();
}

void ChamCong::luuFile(const std::string& duongDan) const {
    std::ofstream f(duongDan);
    for (const auto& bg : lichSu) {
        f << bg.maNV << "," << bg.ngay << "," << bg.gioVao << "," << bg.gioRa
          << "," << bg.maCa << "," << bg.trangThai << "\n";
    }
}

void ChamCong::taiFile(const std::string& duongDan) {
    lichSu.clear();
    dangMoCa.clear();
    std::ifstream f(duongDan);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto p = Utils::splitAndTrim(line, ',');
        if (p.size() < 3) continue;
        BanGhiChamCong bg;
        bg.maNV = p[0]; bg.ngay = p[1]; bg.gioVao = p[2];
        bg.gioRa = (p.size() >= 4) ? p[3] : "";
        bg.maCa = (p.size() >= 5) ? p[4] : "";
        bg.trangThai = (p.size() >= 6) ? p[5] : (bg.gioRa.empty() ? "(Dang lam)" : "Dung gio");
        lichSu.push_back(bg);
        if (bg.gioRa.empty()) dangMoCa[bg.maNV] = (int)lichSu.size() - 1;
    }
}

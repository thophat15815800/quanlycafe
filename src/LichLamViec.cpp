#include "LichLamViec.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>

void LichLamViec::themCa(const CaLamViec& ca) {
    lichTheoNgay[ca.getNgay()].push_back(ca);
}

bool LichLamViec::xoaCa(const std::string& ngay, const std::string& maCa) {
    auto it = lichTheoNgay.find(ngay);
    if (it == lichTheoNgay.end()) return false;
    auto& vec = it->second;
    auto before = vec.size();
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                              [&](const CaLamViec& c) { return c.getMaCa() == maCa; }),
              vec.end());
    return vec.size() != before;
}

CaLamViec* LichLamViec::timCa(const std::string& ngay, const std::string& maCa) {
    auto it = lichTheoNgay.find(ngay);
    if (it == lichTheoNgay.end()) return nullptr;
    for (auto& c : it->second) {
        if (c.getMaCa() == maCa) return &c;
    }
    return nullptr;
}

bool LichLamViec::bTrungCa(const std::string& ngay, const std::string& maNV,
                             const std::string& gioBatDau, const std::string& gioKetThuc) const {
    auto it = lichTheoNgay.find(ngay);
    if (it == lichTheoNgay.end()) return false;
    int batDauMoi = Utils::gioToPhut(gioBatDau);
    int ketThucMoi = Utils::gioToPhut(gioKetThuc);
    for (const auto& ca : it->second) {
        if (!ca.coNhanVien(maNV)) continue;
        int b = Utils::gioToPhut(ca.getGioBatDau());
        int k = Utils::gioToPhut(ca.getGioKetThuc());
        // Hai khung giờ giao nhau nếu không (kết thúc sớm <= bắt đầu muộn)
        if (!(ketThucMoi <= b || batDauMoi >= k)) return true;
    }
    return false;
}

bool LichLamViec::ganNhanVien(const std::string& ngay, const std::string& maCa, const std::string& maNV) {
    CaLamViec* ca = timCa(ngay, maCa);
    if (!ca) return false;
    if (bTrungCa(ngay, maNV, ca->getGioBatDau(), ca->getGioKetThuc())) return false;
    return ca->themNhanVien(maNV);
}

bool LichLamViec::boNhanVien(const std::string& ngay, const std::string& maCa, const std::string& maNV) {
    CaLamViec* ca = timCa(ngay, maCa);
    if (!ca) return false;
    return ca->xoaNhanVien(maNV);
}

void LichLamViec::khoiTao3CaCoDinhNeuChuaCo(const std::string& ngay) {
    auto& dsCa = lichTheoNgay[ngay];
    bool coSang = false, coChieu = false, coToi = false;
    for (const auto& ca : dsCa) {
        if (ca.getTenCa() == "Ca Sang" || ca.getTenCa() == "Sang") coSang = true;
        if (ca.getTenCa() == "Ca Chieu" || ca.getTenCa() == "Chieu") coChieu = true;
        if (ca.getTenCa() == "Ca Toi" || ca.getTenCa() == "Toi") coToi = true;
    }

    if (!coSang) {
        dsCa.insert(dsCa.begin(), CaLamViec("CA_SANG", ngay, "Ca Sang", "07:00", "12:00"));
    }
    if (!coChieu) {
        size_t idx = coSang ? 1 : 0;
        dsCa.insert(dsCa.begin() + idx, CaLamViec("CA_CHIEU", ngay, "Ca Chieu", "12:00", "17:00"));
    }
    if (!coToi) {
        size_t idx = (coSang ? 1 : 0) + (coChieu ? 1 : 0);
        dsCa.insert(dsCa.begin() + idx, CaLamViec("CA_TOI", ngay, "Ca Toi", "17:00", "22:00"));
    }
}

std::vector<CaLamViec> LichLamViec::xemLichNgay(const std::string& ngay) const {
    auto it = lichTheoNgay.find(ngay);
    if (it == lichTheoNgay.end()) return {};
    return it->second;
}

std::vector<CaLamViec> LichLamViec::xemLichNhanVien(const std::string& maNV) const {
    std::vector<CaLamViec> ket;
    for (const auto& [ngay, dsCa] : lichTheoNgay) {
        for (const auto& ca : dsCa) {
            if (ca.coNhanVien(maNV)) ket.push_back(ca);
        }
    }
    return ket;
}

const std::map<std::string, std::vector<CaLamViec>>& LichLamViec::getToanBoLich() const {
    return lichTheoNgay;
}

void LichLamViec::luuFile(const std::string& duongDan) const {
    std::ofstream f(duongDan);
    for (const auto& [ngay, dsCa] : lichTheoNgay) {
        for (const auto& ca : dsCa) f << ca.toCSV() << "\n";
    }
}

void LichLamViec::taiFile(const std::string& duongDan) {
    lichTheoNgay.clear();
    std::ifstream f(duongDan);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        themCa(CaLamViec::fromCSV(line));
    }
}

#include "CaLamViec.h"
#include "Utils.h"
#include <algorithm>

CaLamViec::CaLamViec(std::string maCa, std::string ngay, std::string tenCa,
                       std::string gioBatDau, std::string gioKetThuc)
    : maCa(std::move(maCa)), ngay(std::move(ngay)), tenCa(std::move(tenCa)),
      gioBatDau(std::move(gioBatDau)), gioKetThuc(std::move(gioKetThuc)) {}

std::string CaLamViec::getMaCa() const { return maCa; }
std::string CaLamViec::getNgay() const { return ngay; }
std::string CaLamViec::getTenCa() const { return tenCa; }
std::string CaLamViec::getGioBatDau() const { return gioBatDau; }
std::string CaLamViec::getGioKetThuc() const { return gioKetThuc; }
const std::vector<std::string>& CaLamViec::getDsNhanVien() const { return dsMaNV; }

bool CaLamViec::themNhanVien(const std::string& maNV) {
    if (coNhanVien(maNV)) return false;
    dsMaNV.push_back(maNV);
    return true;
}

bool CaLamViec::xoaNhanVien(const std::string& maNV) {
    auto it = std::find(dsMaNV.begin(), dsMaNV.end(), maNV);
    if (it == dsMaNV.end()) return false;
    dsMaNV.erase(it);
    return true;
}

bool CaLamViec::coNhanVien(const std::string& maNV) const {
    return std::find(dsMaNV.begin(), dsMaNV.end(), maNV) != dsMaNV.end();
}

std::string CaLamViec::toCSV() const {
    std::string nvStr;
    for (size_t i = 0; i < dsMaNV.size(); ++i) {
        nvStr += dsMaNV[i];
        if (i + 1 < dsMaNV.size()) nvStr += "|";
    }
    return maCa + ";" + ngay + ";" + tenCa + ";" + gioBatDau + ";" + gioKetThuc + ";" + nvStr;
}

CaLamViec CaLamViec::fromCSV(const std::string& line) {
    auto p = Utils::splitAndTrim(line, ';');
    CaLamViec ca;
    if (p.size() < 5) return ca;
    ca.maCa = p[0]; ca.ngay = p[1]; ca.tenCa = p[2];
    ca.gioBatDau = p[3]; ca.gioKetThuc = p[4];
    if (p.size() >= 6 && !p[5].empty()) {
        ca.dsMaNV = Utils::split(p[5], '|');
    }
    return ca;
}

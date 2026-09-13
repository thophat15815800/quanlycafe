#include "LichLamViec.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <unordered_set>

void LichLamViec::themCa(const CaLamViec& ca) {
    auto& dsCa = lichTheoNgay[ca.getNgay()];
    for (auto& c : dsCa) {
        if (c.getMaCa() == ca.getMaCa()) {
            c = ca; // Ghi đè nếu trùng mã ca trong ngày (tính lần cuối cùng)
            return;
        }
    }
    dsCa.push_back(ca);
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
        if (!(ketThucMoi <= b || batDauMoi >= k)) return true;
    }
    return false;
}

std::string LichLamViec::timCaCuaNhanVienTrongNgay(const std::string& ngay, const std::string& maNV) const {
    auto it = lichTheoNgay.find(ngay);
    if (it == lichTheoNgay.end()) return "";
    for (const auto& ca : it->second) {
        if (ca.coNhanVien(maNV)) return ca.getMaCa();
    }
    return "";
}

bool LichLamViec::ganNhanVien(const std::string& ngay, const std::string& maCa, const std::string& maNV) {
    CaLamViec* ca = timCa(ngay, maCa);
    if (!ca) return false;

    // Nếu đã ở trong ca này rồi -> không thêm trùng
    if (ca->coNhanVien(maNV)) return false;

    // 1 nhân viên có thể làm tất cả các ca trong ngày (Sáng, Chiều, Tối không trùng giờ)
    // Chỉ từ chối nếu ca mới bị chồng chéo/trùng khung giờ thực tế với một ca khác đã được xếp
    if (bTrungCa(ngay, maNV, ca->getGioBatDau(), ca->getGioKetThuc())) {
        return false;
    }

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
        if (ca.getMaCa() == "CA_SANG" || ca.getTenCa() == "Ca Sang" || ca.getTenCa() == "Sang") coSang = true;
        if (ca.getMaCa() == "CA_CHIEU" || ca.getTenCa() == "Ca Chieu" || ca.getTenCa() == "Chieu") coChieu = true;
        if (ca.getMaCa() == "CA_TOI" || ca.getTenCa() == "Ca Toi" || ca.getTenCa() == "Toi") coToi = true;
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

void LichLamViec::chuanHoaLich() {
    for (auto& [ngay, dsCa] : lichTheoNgay) {
        // 1. Khử trùng lặp mã ca trong cùng một ngày (giữ lại lần cuối cùng)
        std::vector<CaLamViec> caDuyNhat;
        for (auto it = dsCa.rbegin(); it != dsCa.rend(); ++it) {
            bool daCo = false;
            for (const auto& c : caDuyNhat) {
                if (c.getMaCa() == it->getMaCa()) {
                    daCo = true;
                    break;
                }
            }
            if (!daCo) {
                caDuyNhat.push_back(*it);
            }
        }
        std::reverse(caDuyNhat.begin(), caDuyNhat.end());
        dsCa = std::move(caDuyNhat);

        // 2. Khử trùng lặp mã nhân viên trong cùng một ca làm việc (mỗi NV chỉ xuất hiện 1 lần/ca)
        // 1 nhân viên có thể làm nhiều ca khác nhau trong ngày (Sáng, Chiều, Tối)
        for (auto& ca : dsCa) {
            std::vector<std::string> dsNVHopLe;
            std::unordered_set<std::string> daCo;
            for (const auto& maNV : ca.getDsNhanVien()) {
                if (!maNV.empty() && daCo.find(maNV) == daCo.end()) {
                    dsNVHopLe.push_back(maNV);
                    daCo.insert(maNV);
                }
            }
            ca.setDsNhanVien(dsNVHopLe);
        }
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
    // Sắp xếp các ca theo thứ tự thời gian chuẩn xác: Năm -> Tháng -> Ngày -> Giờ bắt đầu
    std::sort(ket.begin(), ket.end(), [](const CaLamViec& a, const CaLamViec& b) {
        auto pA = Utils::split(a.getNgay(), '/');
        auto pB = Utils::split(b.getNgay(), '/');
        if (pA.size() == 3 && pB.size() == 3) {
            try {
                int yA = std::stoi(pA[2]), yB = std::stoi(pB[2]);
                if (yA != yB) return yA < yB;
                int mA = std::stoi(pA[1]), mB = std::stoi(pB[1]);
                if (mA != mB) return mA < mB;
                int dA = std::stoi(pA[0]), dB = std::stoi(pB[0]);
                if (dA != dB) return dA < dB;
            } catch (...) {}
        }
        if (a.getNgay() != b.getNgay()) return a.getNgay() < b.getNgay();
        return Utils::gioToPhut(a.getGioBatDau()) < Utils::gioToPhut(b.getGioBatDau());
    });
    return ket;
}

const std::map<std::string, std::vector<CaLamViec>>& LichLamViec::getToanBoLich() const {
    return lichTheoNgay;
}

void LichLamViec::luuFile(const std::string& duongDan) {
    chuanHoaLich(); // Chuẩn hóa trước khi lưu: khử trùng lặp ca và nhân viên
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
    chuanHoaLich();
}

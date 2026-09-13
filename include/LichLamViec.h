#pragma once
#include <map>
#include <vector>
#include <string>
#include "CaLamViec.h"

// Quản lý lịch làm việc toàn quán: map<ngay, danh sách ca trong ngày đó>
// Dùng std::map để lịch luôn được sắp xếp và tra cứu theo ngày hiệu quả.
class LichLamViec {
private:
    std::map<std::string, std::vector<CaLamViec>> lichTheoNgay;

public:
    void themCa(const CaLamViec& ca);
    bool xoaCa(const std::string& ngay, const std::string& maCa);
    CaLamViec* timCa(const std::string& ngay, const std::string& maCa);

    // Xếp 1 nhân viên vào ca; kiểm tra trùng giờ trong cùng ngày trước khi xếp
    bool ganNhanVien(const std::string& ngay, const std::string& maCa, const std::string& maNV);
    bool boNhanVien(const std::string& ngay, const std::string& maCa, const std::string& maNV);

    // Kiểm tra nhân viên đã có ca trùng khung giờ trong ngày chưa (tránh xếp chồng ca)
    bool bTrungCa(const std::string& ngay, const std::string& maNV,
                   const std::string& gioBatDau, const std::string& gioKetThuc) const;

    // Tự động khởi tạo 3 ca cố định (Sáng 07:00-12:00, Chiều 12:00-17:00, Tối 17:00-22:00) cho một ngày nếu chưa có
    void khoiTao3CaCoDinhNeuChuaCo(const std::string& ngay);

    std::vector<CaLamViec> xemLichNgay(const std::string& ngay) const;
    std::vector<CaLamViec> xemLichNhanVien(const std::string& maNV) const; // toàn bộ ca của 1 NV
    const std::map<std::string, std::vector<CaLamViec>>& getToanBoLich() const;

    void luuFile(const std::string& duongDan) const;
    void taiFile(const std::string& duongDan);
};

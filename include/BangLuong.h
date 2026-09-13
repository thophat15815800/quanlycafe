#pragma once
#include <string>
#include <vector>
#include "NhanVien.h"

struct BanGhiLuong {
    std::string maNV;
    std::string thangNam;   // mm/yyyy
    double soGioLam;
    double thucLinh;
};

// Lưu lịch sử lương đã tính từng tháng cho từng nhân viên
class BangLuong {
private:
    std::vector<BanGhiLuong> lichSu;

public:
    // Tính lương (gọi hàm ảo tinhLuong() -> ĐA HÌNH theo từng loại NV) rồi lưu lại
    double tinhVaLuu(const NhanVien* nv, double soGioLam, const std::string& thangNam);

    std::vector<BanGhiLuong> layTheoThang(const std::string& thangNam) const;
    std::vector<BanGhiLuong> layTheoNhanVien(const std::string& maNV) const;

    void luuFile(const std::string& duongDan) const;
    void taiFile(const std::string& duongDan);
};

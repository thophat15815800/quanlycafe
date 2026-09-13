#pragma once
#include "NhanVien.h"

// Nhân viên thu ngân: lương theo giờ + phụ cấp trách nhiệm cố định/tháng.
// luongCoBan của lớp cha được dùng làm lương/giờ.
class ThuNgan : public NhanVien {
private:
    double phuCapTrachNhiem;

public:
    ThuNgan(std::string maNV, std::string hoTen, std::string sdt,
            std::string email, std::string ngayVaoLam, double luongTheoGio,
            double phuCapTrachNhiem);

    double getLuongTheoGio() const;
    double getPhuCap() const;
    void setPhuCap(double pc);

    double tinhLuong(double soGioLam) const override;
    std::string loaiNhanVien() const override;
    void hienThiThongTin() const override;
    std::string toCSV() const override;
};

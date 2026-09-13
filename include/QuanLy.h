#pragma once
#include "NhanVien.h"

// Quản lý: hưởng lương cứng theo tháng + phụ cấp quản lý, không tính theo giờ.
// luongCoBan của lớp cha được dùng làm lương cứng.
class QuanLy : public NhanVien {
private:
    double phuCapQuanLy;

public:
    QuanLy(std::string maNV, std::string hoTen, std::string sdt,
           std::string email, std::string ngayVaoLam, double luongCung,
           double phuCapQuanLy);

    double getLuongCung() const;
    double getPhuCapQuanLy() const;
    void setPhuCapQuanLy(double pc);

    double tinhLuong(double soGioLam) const override; // soGioLam không ảnh hưởng nhiều
    std::string loaiNhanVien() const override;
    void hienThiThongTin() const override;
    std::string toCSV() const override;
};

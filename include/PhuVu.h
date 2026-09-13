#pragma once
#include "NhanVien.h"

// Nhân viên phục vụ: lương theo giờ (giống Pha chế)
// luongCoBan của lớp cha được dùng làm lương/giờ.
class PhuVu : public NhanVien {
public:
    PhuVu(std::string maNV, std::string hoTen, std::string sdt,
          std::string email, std::string ngayVaoLam, double luongTheoGio);

    double getLuongTheoGio() const;
    void setLuongTheoGio(double luong);

    double tinhLuong(double soGioLam) const override;
    std::string loaiNhanVien() const override;
    void hienThiThongTin() const override;
    std::string toCSV() const override;
};

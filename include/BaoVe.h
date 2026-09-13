#pragma once
#include "NhanVien.h"

// Nhân viên bảo vệ: lương cứng/tháng (không phụ cấp)
// luongCoBan của lớp cha được dùng làm lương cứng.
class BaoVe : public NhanVien {
public:
    BaoVe(std::string maNV, std::string hoTen, std::string sdt,
          std::string email, std::string ngayVaoLam, double luongCung);

    double getLuongCung() const;
    void setLuongCung(double luong);

    double tinhLuong(double soGioLam) const override;  // không phụ thuộc soGioLam
    std::string loaiNhanVien() const override;
    void hienThiThongTin() const override;
    std::string toCSV() const override;
};

#pragma once
#include "NhanVien.h"

// ================ KẾ THỪA (INHERITANCE) ================
// Nhân viên pha chế: hưởng lương theo giờ, không có thêm phụ cấp.
// luongCoBan của lớp cha được dùng làm lương/giờ (không duplicate biến riêng).
class PhaChe : public NhanVien {
public:
    PhaChe(std::string maNV, std::string hoTen, std::string sdt,
           std::string email, std::string ngayVaoLam, double luongTheoGio,
           std::string avatar = "");

    double getLuongTheoGio() const;
    void setLuongTheoGio(double luong);

    // ====== ĐA HÌNH: hiện thực công thức tính lương riêng ======
    double tinhLuong(double soGioLam) const override;
    std::string loaiNhanVien() const override;
    void hienThiThongTin() const override;
    std::string toCSV() const override;
};

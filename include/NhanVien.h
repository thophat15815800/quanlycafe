#pragma once
#include <string>

// Trạng thái làm việc của nhân viên
enum class TrangThaiNV { DANG_LAM, TAM_NGHI, DA_NGHI_VIEC };

std::string trangThaiToChuoi(TrangThaiNV tt);
TrangThaiNV chuoiToTrangThai(const std::string& s);

// ===================== LỚP TRỪU TƯỢNG (ABSTRACTION) =====================
// NhanVien là lớp cơ sở trừu tượng: không thể tạo đối tượng trực tiếp,
// bắt buộc các lớp con phải hiện thực tinhLuong() và loaiNhanVien().
class NhanVien {
protected:
    // ============ ĐÓNG GÓI (ENCAPSULATION): dữ liệu private/protected =====
    std::string maNV;
    std::string hoTen;
    std::string sdt;
    std::string email;
    std::string ngayVaoLam;   // dd/mm/yyyy
    double luongCoBan;        // ý nghĩa tùy loại NV (lương/giờ hoặc lương cứng)
    TrangThaiNV trangThai;

public:
    NhanVien(std::string maNV, std::string hoTen, std::string sdt,
             std::string email, std::string ngayVaoLam, double luongCoBan);
    virtual ~NhanVien() = default;

    // ---- Getter/Setter có kiểm tra hợp lệ (đóng gói dữ liệu) ----
    std::string getMaNV() const;
    std::string getHoTen() const;
    void setHoTen(const std::string& ten);
    std::string getSdt() const;
    void setSdt(const std::string& sdt);
    std::string getEmail() const;
    void setEmail(const std::string& email);
    std::string getNgayVaoLam() const;
    double getLuongCoBan() const;
    void setLuongCoBan(double luong);
    TrangThaiNV getTrangThai() const;
    void setTrangThai(TrangThaiNV tt);

    // ============ ĐA HÌNH (POLYMORPHISM) - hàm ảo thuần =============
    virtual double tinhLuong(double soGioLam) const = 0;   // công thức khác nhau theo lớp con
    virtual std::string loaiNhanVien() const = 0;           // "PHA_CHE" / "THU_NGAN" / "QUAN_LY"
    virtual void hienThiThongTin() const;                   // có thể override để in thêm field riêng
    virtual std::string toCSV() const;                      // ghi ra file, override để thêm cột riêng
};

#pragma once
#include <string>
#include <vector>

// ============ PHÂN QUYỀN ============
enum class VaiTro { ADMIN, EMPLOYEE };

std::string vaiTroToChuoi(VaiTro vt);
VaiTro chuoiToVaiTro(const std::string& s);

// Một tài khoản đăng nhập
class TaiKhoan {
private:
    std::string tenDangNhap;
    std::string matKhau;      // lưu dạng hash đơn giản
    VaiTro vaiTro;
    std::string maNV;         // liên kết với nhân viên (rỗng nếu admin thuần)

public:
    TaiKhoan() = default;
    TaiKhoan(std::string tenDangNhap, const std::string& matKhauGoc,
             VaiTro vaiTro, std::string maNV = "");

    std::string getTenDangNhap() const;
    VaiTro getVaiTro() const;
    std::string getMaNV() const;
    bool kiemTraMatKhau(const std::string& matKhauGoc) const;
    void doiMatKhau(const std::string& matKhauMoi);

    std::string toCSV() const;
    static TaiKhoan fromCSV(const std::string& line);

    // Hash mật khẩu đơn giản (không dùng thư viện ngoài)
    static std::string hashMatKhau(const std::string& matKhauGoc);
};

// Quản lý tài khoản toàn hệ thống
class QuanLyTaiKhoan {
private:
    std::vector<TaiKhoan> danhSachTK;
    TaiKhoan* tkHienTai = nullptr;  // con trỏ tới tài khoản đang đăng nhập

public:
    // Đăng nhập/đăng xuất
    bool dangNhap(const std::string& tenDN, const std::string& matKhau);
    void dangXuat();

    // Kiểm tra quyền
    bool daDangNhap() const;
    bool laAdmin() const;
    std::string getMaNVHienTai() const;
    std::string getTenDangNhapHienTai() const;
    VaiTro getVaiTroHienTai() const;

    // Quản lý tài khoản (chỉ Admin)
    bool themTaiKhoan(const std::string& tenDN, const std::string& matKhau,
                      VaiTro vaiTro, const std::string& maNV = "");
    bool xoaTaiKhoan(const std::string& tenDN);
    bool doiMatKhau(const std::string& tenDN, const std::string& matKhauMoi);
    bool resetMatKhau(const std::string& tenDN); // reset về 123456
    const std::vector<TaiKhoan>& layDanhSachTK() const;
    TaiKhoan* timTaiKhoan(const std::string& tenDN);

    // Lưu/tải file
    bool luuFile(const std::string& duongDan) const;
    void taiFile(const std::string& duongDan);

    // Tự động đồng bộ tài khoản cho các nhân viên đã có sẵn (mật khẩu mặc định 123456)
    void dongBoTaiKhoanNhanVien(const std::vector<std::string>& dsMaNV);

    // Tạo tài khoản mặc định nếu file rỗng
    void taoDuLieuMacDinh();
};

#pragma once
#include "QuanLyQuanCafe.h"
#include "TaiKhoan.h"

// Lớp giao diện (View) - console. Chỉ gọi vào QuanLyQuanCafe (Controller/Model),
// không chứa logic nghiệp vụ -> sau này có thể thay bằng GUI (Qt / Dear ImGui)
// mà không cần sửa các lớp nghiệp vụ phía trên.
class ConsoleUI {
private:
    QuanLyQuanCafe app;
    QuanLyTaiKhoan quanLyTK;
    std::string thuMucDuLieu = "data";

    // ---- Màn hình đăng nhập ----
    bool manHinhDangNhap();

    // ---- Menu Admin (toàn quyền) ----
    void hienMenuChinh();
    void menuNhanVien();
    void menuTimKiemLocNV();
    void menuSapXepNV();
    void menuLichLamViec();
    void menuChamCong();
    void menuTinhLuong();
    void hienDashboard();
    void menuQuanLyTaiKhoan();

    // ---- Menu Employee (quyền hạn chế) ----
    void hienMenuEmployee();
    void menuNhanVienEmployee();
    void menuLichEmployee();
    void menuChamCongEmployee();
    void menuLuongEmployee();

    // Các hàm nhập liệu tiện ích
    int nhapSoNguyen(const std::string& goiY);
    double nhapSoThuc(const std::string& goiY);
    std::string nhapChuoi(const std::string& goiY);
    std::string nhapMatKhau(const std::string& goiY); // ẩn ký tự khi nhập

    // Validate input
    std::string nhapSDT(const std::string& goiY);
    std::string nhapEmail(const std::string& goiY);
    std::string nhapNgay(const std::string& goiY, bool choPhepTrong = false);
    std::string nhapGio(const std::string& goiY);

public:
    ConsoleUI();
    void chay(); // vòng lặp chính của chương trình
};

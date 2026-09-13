#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include "NhanVien.h"
#include "LichLamViec.h"
#include "ChamCong.h"
#include "BangLuong.h"

// Thông tin tổng hợp cho Dashboard (giống các "card" trên giao diện mẫu)
struct ThongTinDashboard {
    int tongNhanVien = 0;
    int dangLamViec = 0;      // đang trong ca (đã check-in, chưa check-out)
    int tamNghi = 0;          // trạng thái TAM_NGHI
    int daNghiViec = 0;
    int soCaHomNay = 0;
    int soNhanVienDuocXepCaHomNay = 0;
};

// Cấu trúc tiêu chí tìm kiếm và lọc nhân viên
struct BoLocNhanVien {
    std::string tuKhoa = "";       // tìm trong mã, tên, SDT, email
    std::string loaiNV = "";        // "PHA_CHE", "THU_NGAN", "QUAN_LY", "PHU_VU", "BAO_VE", "" = tất cả
    int trangThai = -1;             // -1 = tất cả, 0 = DANG_LAM, 1 = TAM_NGHI, 2 = DA_NGHI_VIEC
    double luongMin = -1;           // -1 = không giới hạn dưới
    double luongMax = -1;           // -1 = không giới hạn trên
};

// Tiêu chí sắp xếp danh sách nhân viên
enum class TieuChiSapXep {
    MA_NV_TANG,
    MA_NV_GIAM,
    TEN_TANG,
    TEN_GIAM,
    LUONG_TANG,
    LUONG_GIAM,
    NGAY_VAO_LAM_CU_NHAT,
    NGAY_VAO_LAM_MOI_NHAT
};

// ===================== LỚP ĐIỀU KHIỂN TRUNG TÂM =====================
// Kết hợp (composition) toàn bộ các module nghiệp vụ: nhân viên, lịch làm việc,
// chấm công, bảng lương. Đây là nơi các thao tác nghiệp vụ chính được thực hiện,
// tách biệt khỏi lớp giao diện (ConsoleUI) để dễ dàng thay giao diện sau này.
class QuanLyQuanCafe {
private:
    std::vector<std::unique_ptr<NhanVien>> danhSachNV;
    LichLamViec lich;
    ChamCong chamCong;
    BangLuong bangLuong;

    // unordered_map: tra cứu vị trí nhân viên theo mã trong O(1) thay vì duyệt vector
    std::unordered_map<std::string, size_t> chiSoNV;

    // queue: nhật ký hoạt động gần đây (demo thêm 1 cấu trúc dữ liệu FIFO)
    std::queue<std::string> nhatKyHoatDong;
    static const size_t GIOI_HAN_NHAT_KY = 50;

    void ghiNhatKy(const std::string& noiDung);
    void xayDungChiSo(); // build lại unordered_map sau khi tải file / thêm / xoá

public:
    // ---------- Quản lý thông tin nhân viên ----------
    bool themNhanVien(std::unique_ptr<NhanVien> nv);
    bool xoaNhanVien(const std::string& maNV);          // xoá mềm -> DA_NGHI_VIEC
    NhanVien* timNhanVien(const std::string& maNV);
    const std::vector<std::unique_ptr<NhanVien>>& layDanhSachNV() const;
    std::vector<NhanVien*> locTheoTrangThai(TrangThaiNV tt) const;
    std::string sinhMaNVMoi() const;

    // Tìm kiếm và lọc nâng cao đa tiêu chí
    std::vector<NhanVien*> timKiemVaLoc(const BoLocNhanVien& boLoc) const;

    // Sắp xếp danh sách nhân viên theo tiêu chí (dùng std::sort + lambda)
    void sapXepNhanVien(std::vector<NhanVien*>& danhSach, TieuChiSapXep tieuChi) const;

    // ---------- Lịch làm việc / xếp ca ----------
    LichLamViec& layLich();
    bool xepCa(const std::string& ngay, const std::string& maCa, const std::string& maNV);

    // ---------- Chấm công ----------
    bool chamCongVao(const std::string& maNV);
    bool chamCongRa(const std::string& maNV);
    ChamCong& layChamCong();

    // ---------- Tính lương ----------
    double tinhLuongThang(const std::string& maNV, const std::string& thangNam);
    std::vector<BanGhiLuong> bangLuongThang(const std::string& thangNam);
    BangLuong& layBangLuong();

    // ---------- Dashboard & báo cáo ----------
    ThongTinDashboard layDashboard() const;
    std::vector<std::string> layNhatKyGanDay() const;

    // ---------- Lưu / tải toàn bộ dữ liệu ----------
    void luuTatCa(const std::string& thuMuc);
    void taiTatCa(const std::string& thuMuc);
};

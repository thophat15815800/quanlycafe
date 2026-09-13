#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// Một bản ghi chấm công (1 lượt vào-ra trong ngày)
struct BanGhiChamCong {
    std::string maNV;
    std::string ngay;     // dd/mm/yyyy
    std::string gioVao;   // HH:MM
    std::string gioRa;    // HH:MM, rỗng nếu chưa check-out
};

// Quản lý chấm công toàn quán.
// - vector: lưu toàn bộ lịch sử (duyệt tuần tự, ghi file dễ dàng)
// - unordered_map: tra cứu O(1) xem 1 nhân viên có đang trong ca hay không
class ChamCong {
private:
    std::vector<BanGhiChamCong> lichSu;
    std::unordered_map<std::string, int> dangMoCa; // maNV -> vị trí bản ghi chưa check-out

public:
    bool checkIn(const std::string& maNV, const std::string& ngay, const std::string& gio);
    bool checkOut(const std::string& maNV, const std::string& ngay, const std::string& gio);
    bool dangTrongCa(const std::string& maNV) const;

    // Tổng số giờ đã làm của 1 nhân viên trong 1 tháng (mm/yyyy)
    double tongGioLamThang(const std::string& maNV, const std::string& thangNam) const;
    std::vector<BanGhiChamCong> lichSuNhanVien(const std::string& maNV) const;
    std::vector<BanGhiChamCong> lichSuNgay(const std::string& ngay) const;
    int soNhanVienDangLam() const;

    void luuFile(const std::string& duongDan) const;
    void taiFile(const std::string& duongDan);
};

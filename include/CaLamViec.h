#pragma once
#include <string>
#include <vector>

// Một ca làm việc trong ngày (VD: Ca sáng 07:00-11:00)
class CaLamViec {
private:
    std::string maCa;
    std::string ngay;        // dd/mm/yyyy
    std::string tenCa;       // "Sang" / "Chieu" / "Toi"
    std::string gioBatDau;   // HH:MM
    std::string gioKetThuc;  // HH:MM
    std::vector<std::string> dsMaNV; // danh sách nhân viên được xếp vào ca

public:
    CaLamViec() = default;
    CaLamViec(std::string maCa, std::string ngay, std::string tenCa,
               std::string gioBatDau, std::string gioKetThuc);

    std::string getMaCa() const;
    std::string getNgay() const;
    std::string getTenCa() const;
    std::string getGioBatDau() const;
    std::string getGioKetThuc() const;
    const std::vector<std::string>& getDsNhanVien() const;

    bool themNhanVien(const std::string& maNV);   // false nếu đã có
    bool xoaNhanVien(const std::string& maNV);
    bool coNhanVien(const std::string& maNV) const;
    void setDsNhanVien(const std::vector<std::string>& ds);

    std::string toCSV() const;             // maCa;ngay;tenCa;batdau;ketthuc;nv1|nv2|nv3
    static CaLamViec fromCSV(const std::string& line);
};

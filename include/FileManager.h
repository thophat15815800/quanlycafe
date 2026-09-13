#pragma once
#include <string>
#include <vector>
#include <memory>
#include "NhanVien.h"

// Lớp tiện ích tĩnh: đọc/ghi danh sách nhân viên (đa hình) ra file .csv
// Dùng "factory method" để tạo đúng lớp con dựa vào cột "loai" trong file.
class FileManager {
public:
    static bool luuNhanVien(const std::vector<std::unique_ptr<NhanVien>>& ds,
                             const std::string& duongDan);
    static std::vector<std::unique_ptr<NhanVien>> taiNhanVien(const std::string& duongDan);

private:
    static std::unique_ptr<NhanVien> taoTuDong(const std::string& line);
};

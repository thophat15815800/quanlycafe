#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cctype>

// ==== Các hàm tiện ích dùng chung trong toàn bộ chương trình ====
namespace Utils {

// Loại bỏ khoảng trắng, \r, \n, \t ở đầu và cuối chuỗi
// Quan trọng: file CSV trên Windows có thể chứa \r\n, std::getline
// chỉ bỏ \n nên \r dính vào cuối token gây sai dữ liệu.
inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Tách chuỗi theo ký tự phân cách (dùng khi đọc/ghi CSV)
inline std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    // Giữ lại trường rỗng ở cuối (ví dụ giờ ra chưa chấm công)
    if (!s.empty() && s.back() == delim) tokens.push_back("");
    return tokens;
}

// Split rồi tự động trim mỗi token — dùng khi đọc file CSV
// để tránh lỗi \r dính cuối chuỗi trên Windows.
inline std::vector<std::string> splitAndTrim(const std::string& s, char delim) {
    auto tokens = split(s, delim);
    for (auto& t : tokens) t = trim(t);
    return tokens;
}

// Nối các trường lại thành 1 dòng CSV
inline std::string join(const std::vector<std::string>& fields, char delim) {
    std::string out;
    for (size_t i = 0; i < fields.size(); ++i) {
        out += fields[i];
        if (i + 1 < fields.size()) out += delim;
    }
    return out;
}

// Chuyển "HH:MM" thành tổng số phút trong ngày, trả -1 nếu rỗng/không hợp lệ
inline int gioToPhut(const std::string& hhmm) {
    if (hhmm.empty()) return -1;
    auto parts = split(hhmm, ':');
    if (parts.size() != 2) return -1;
    try {
        int h = std::stoi(parts[0]);
        int m = std::stoi(parts[1]);
        return h * 60 + m;
    } catch (...) {
        return -1;
    }
}

// Lấy ngày hiện tại dạng dd/mm/yyyy
inline std::string ngayHomNay() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    return std::string(buf);
}

// Lấy tháng/năm hiện tại dạng mm/yyyy (dùng để tính lương theo tháng)
inline std::string thangNamHienTai() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[32];
    snprintf(buf, sizeof(buf), "%02d/%04d", lt->tm_mon + 1, lt->tm_year + 1900);
    return std::string(buf);
}

// Lấy giờ hiện tại dạng HH:MM
inline std::string gioHienTai() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", lt->tm_hour, lt->tm_min);
    return std::string(buf);
}

// Trích tháng/năm (mm/yyyy) từ ngày dd/mm/yyyy
inline std::string layThangNamTuNgay(const std::string& ngay) {
    auto p = split(ngay, '/');
    if (p.size() != 3) return "";
    return p[1] + "/" + p[2];
}

// ============ VALIDATE INPUT ============

// Kiểm tra email hợp lệ (có @ và . sau @)
inline bool isValidEmail(const std::string& email) {
    if (email.empty()) return false;
    auto atPos = email.find('@');
    if (atPos == std::string::npos || atPos == 0) return false;
    auto dotPos = email.find('.', atPos);
    if (dotPos == std::string::npos || dotPos == email.size() - 1) return false;
    return true;
}

// Kiểm tra SĐT hợp lệ (10-11 chữ số, bắt đầu bằng 0)
inline bool isValidSDT(const std::string& sdt) {
    if (sdt.size() < 10 || sdt.size() > 11) return false;
    if (sdt[0] != '0') return false;
    for (char c : sdt) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

// Kiểm tra ngày hợp lệ (dd/mm/yyyy)
inline bool isValidNgay(const std::string& ngay) {
    if (ngay.size() != 10) return false;
    if (ngay[2] != '/' || ngay[5] != '/') return false;
    try {
        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));
        if (y < 1900 || y > 2100) return false;
        if (m < 1 || m > 12) return false;
        int maxDay[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        // Năm nhuận
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) maxDay[1] = 29;
        if (d < 1 || d > maxDay[m - 1]) return false;
        return true;
    } catch (...) { return false; }
}

// Kiểm tra giờ hợp lệ (HH:MM, 00:00 - 23:59)
inline bool isValidGio(const std::string& gio) {
    if (gio.size() != 5) return false;
    if (gio[2] != ':') return false;
    try {
        int h = std::stoi(gio.substr(0, 2));
        int m = std::stoi(gio.substr(3, 2));
        return h >= 0 && h <= 23 && m >= 0 && m <= 59;
    } catch (...) { return false; }
}

// Chuyển chuỗi thành chữ thường (dùng để tìm kiếm không phân biệt hoa/thường)
inline std::string toLower(const std::string& s) {
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return res;
}

// Kiểm tra haystack có chứa needle không (không phân biệt hoa/thường)
inline bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    std::string h = toLower(haystack);
    std::string n = toLower(needle);
    return h.find(n) != std::string::npos;
}

// So sánh 2 ngày dd/mm/yyyy. Trả về < 0 nếu d1 < d2, 0 nếu d1 == d2, > 0 nếu d1 > d2
inline int soSanhNgay(const std::string& d1, const std::string& d2) {
    auto p1 = split(d1, '/');
    auto p2 = split(d2, '/');
    if (p1.size() != 3 || p2.size() != 3) return d1.compare(d2);
    try {
        int y1 = std::stoi(p1[2]), y2 = std::stoi(p2[2]);
        if (y1 != y2) return y1 - y2;
        int m1 = std::stoi(p1[1]), m2 = std::stoi(p2[1]);
        if (m1 != m2) return m1 - m2;
        int day1 = std::stoi(p1[0]), day2 = std::stoi(p2[0]);
        return day1 - day2;
    } catch (...) {
        return d1.compare(d2);
    }
}

} // namespace Utils

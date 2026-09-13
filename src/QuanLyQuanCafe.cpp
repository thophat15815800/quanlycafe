#include "QuanLyQuanCafe.h"
#include "FileManager.h"
#include "Utils.h"
#include <algorithm>
#include <set>
#include <sstream>

void QuanLyQuanCafe::ghiNhatKy(const std::string& noiDung) {
    std::string dong = "[" + Utils::ngayHomNay() + " " + Utils::gioHienTai() + "] " + noiDung;
    nhatKyHoatDong.push(dong);
    if (nhatKyHoatDong.size() > GIOI_HAN_NHAT_KY) nhatKyHoatDong.pop(); // FIFO, giữ tối đa N dòng
}

void QuanLyQuanCafe::xayDungChiSo() {
    chiSoNV.clear();
    for (size_t i = 0; i < danhSachNV.size(); ++i) {
        chiSoNV[danhSachNV[i]->getMaNV()] = i;
    }
}

bool QuanLyQuanCafe::themNhanVien(std::unique_ptr<NhanVien> nv) {
    if (!nv) return false;
    if (timNhanVien(nv->getMaNV()) != nullptr) return false; // trùng mã
    ghiNhatKy("Them nhan vien moi: " + nv->getMaNV() + " - " + nv->getHoTen());
    chiSoNV[nv->getMaNV()] = danhSachNV.size();
    danhSachNV.push_back(std::move(nv));
    return true;
}

bool QuanLyQuanCafe::xoaNhanVien(const std::string& maNV) {
    NhanVien* nv = timNhanVien(maNV);
    if (!nv) return false;
    nv->setTrangThai(TrangThaiNV::DA_NGHI_VIEC); // xoá mềm, giữ lịch sử lương/chấm công
    ghiNhatKy("Cho nhan vien nghi viec: " + maNV);
    return true;
}

NhanVien* QuanLyQuanCafe::timNhanVien(const std::string& maNV) {
    auto it = chiSoNV.find(maNV);
    if (it == chiSoNV.end() || it->second >= danhSachNV.size()) return nullptr;
    if (danhSachNV[it->second]->getMaNV() != maNV) return nullptr; // an toàn nếu chỉ số lệch
    return danhSachNV[it->second].get();
}

const std::vector<std::unique_ptr<NhanVien>>& QuanLyQuanCafe::layDanhSachNV() const {
    return danhSachNV;
}

std::vector<NhanVien*> QuanLyQuanCafe::locTheoTrangThai(TrangThaiNV tt) const {
    std::vector<NhanVien*> ket;
    for (const auto& nv : danhSachNV) {
        if (nv->getTrangThai() == tt) ket.push_back(nv.get());
    }
    return ket;
}

std::string QuanLyQuanCafe::sinhMaNVMoi() const {
    int maxSo = 0;
    for (const auto& nv : danhSachNV) {
        std::string ma = nv->getMaNV();
        if (ma.size() > 2 && ma[0] == 'N' && ma[1] == 'V') {
            try { maxSo = std::max(maxSo, std::stoi(ma.substr(2))); } catch (...) {}
        }
    }
    std::ostringstream oss;
    oss << "NV" << (maxSo + 1);
    return oss.str();
}

std::vector<NhanVien*> QuanLyQuanCafe::timKiemVaLoc(const BoLocNhanVien& boLoc) const {
    std::vector<NhanVien*> ketQua;
    for (const auto& nv : danhSachNV) {
        // 1. Kiểm tra từ khóa (tìm trong Mã NV, Họ tên, SĐT, Email)
        if (!boLoc.tuKhoa.empty()) {
            bool khopMa = Utils::containsIgnoreCase(nv->getMaNV(), boLoc.tuKhoa);
            bool khopTen = Utils::containsIgnoreCase(nv->getHoTen(), boLoc.tuKhoa);
            bool khopSDT = Utils::containsIgnoreCase(nv->getSdt(), boLoc.tuKhoa);
            bool khopEmail = Utils::containsIgnoreCase(nv->getEmail(), boLoc.tuKhoa);
            if (!khopMa && !khopTen && !khopSDT && !khopEmail) continue;
        }

        // 2. Kiểm tra loại nhân viên
        if (!boLoc.loaiNV.empty() && nv->loaiNhanVien() != boLoc.loaiNV) {
            continue;
        }

        // 3. Kiểm tra trạng thái làm việc (-1 = tất cả)
        if (boLoc.trangThai != -1) {
            if (boLoc.trangThai == 0 && nv->getTrangThai() != TrangThaiNV::DANG_LAM) continue;
            if (boLoc.trangThai == 1 && nv->getTrangThai() != TrangThaiNV::TAM_NGHI) continue;
            if (boLoc.trangThai == 2 && nv->getTrangThai() != TrangThaiNV::DA_NGHI_VIEC) continue;
        }

        // 4. Kiểm tra khoảng lương
        if (boLoc.luongMin >= 0 && nv->getLuongCoBan() < boLoc.luongMin) continue;
        if (boLoc.luongMax >= 0 && nv->getLuongCoBan() > boLoc.luongMax) continue;

        ketQua.push_back(nv.get());
    }
    return ketQua;
}

void QuanLyQuanCafe::sapXepNhanVien(std::vector<NhanVien*>& danhSach, TieuChiSapXep tieuChi) const {
    switch (tieuChi) {
        case TieuChiSapXep::MA_NV_TANG:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return a->getMaNV() < b->getMaNV();
            });
            break;
        case TieuChiSapXep::MA_NV_GIAM:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return a->getMaNV() > b->getMaNV();
            });
            break;
        case TieuChiSapXep::TEN_TANG:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return Utils::toLower(a->getHoTen()) < Utils::toLower(b->getHoTen());
            });
            break;
        case TieuChiSapXep::TEN_GIAM:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return Utils::toLower(a->getHoTen()) > Utils::toLower(b->getHoTen());
            });
            break;
        case TieuChiSapXep::LUONG_TANG:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return a->getLuongCoBan() < b->getLuongCoBan();
            });
            break;
        case TieuChiSapXep::LUONG_GIAM:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return a->getLuongCoBan() > b->getLuongCoBan();
            });
            break;
        case TieuChiSapXep::NGAY_VAO_LAM_CU_NHAT:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return Utils::soSanhNgay(a->getNgayVaoLam(), b->getNgayVaoLam()) < 0;
            });
            break;
        case TieuChiSapXep::NGAY_VAO_LAM_MOI_NHAT:
            std::sort(danhSach.begin(), danhSach.end(), [](const NhanVien* a, const NhanVien* b) {
                return Utils::soSanhNgay(a->getNgayVaoLam(), b->getNgayVaoLam()) > 0;
            });
            break;
    }
}

LichLamViec& QuanLyQuanCafe::layLich() { return lich; }

bool QuanLyQuanCafe::xepCa(const std::string& ngay, const std::string& maCa, const std::string& maNV) {
    if (!timNhanVien(maNV)) return false;
    bool ok = lich.ganNhanVien(ngay, maCa, maNV);
    if (ok) ghiNhatKy("Xep ca " + maCa + " ngay " + ngay + " cho NV " + maNV);
    return ok;
}

bool QuanLyQuanCafe::chamCongVao(const std::string& maNV) {
    if (!timNhanVien(maNV)) return false;
    bool ok = chamCong.checkIn(maNV, Utils::ngayHomNay(), Utils::gioHienTai());
    if (ok) ghiNhatKy("Cham cong VAO: " + maNV);
    return ok;
}

bool QuanLyQuanCafe::chamCongRa(const std::string& maNV) {
    bool ok = chamCong.checkOut(maNV, Utils::ngayHomNay(), Utils::gioHienTai());
    if (ok) ghiNhatKy("Cham cong RA: " + maNV);
    return ok;
}

ChamCong& QuanLyQuanCafe::layChamCong() { return chamCong; }

double QuanLyQuanCafe::tinhLuongThang(const std::string& maNV, const std::string& thangNam) {
    NhanVien* nv = timNhanVien(maNV);
    if (!nv) return -1;
    double soGio = chamCong.tongGioLamThang(maNV, thangNam);
    double luong = bangLuong.tinhVaLuu(nv, soGio, thangNam); // <-- đa hình
    ghiNhatKy("Tinh luong thang " + thangNam + " cho " + maNV + ": " + std::to_string(luong));
    return luong;
}

std::vector<BanGhiLuong> QuanLyQuanCafe::bangLuongThang(const std::string& thangNam) {
    // Đảm bảo tất cả nhân viên đang hoạt động đều được tính trước khi trả về bảng
    for (const auto& nv : danhSachNV) {
        if (nv->getTrangThai() == TrangThaiNV::DA_NGHI_VIEC) continue;
        tinhLuongThang(nv->getMaNV(), thangNam);
    }
    return bangLuong.layTheoThang(thangNam);
}

BangLuong& QuanLyQuanCafe::layBangLuong() { return bangLuong; }

ThongTinDashboard QuanLyQuanCafe::layDashboard() const {
    ThongTinDashboard dt;
    for (const auto& nv : danhSachNV) {
        if (nv->getTrangThai() == TrangThaiNV::TAM_NGHI) dt.tamNghi++;
        else if (nv->getTrangThai() == TrangThaiNV::DA_NGHI_VIEC) dt.daNghiViec++;
    }
    // tongNhanVien chỉ đếm NV đang hoạt động (không tính đã nghỉ việc)
    dt.tongNhanVien = (int)danhSachNV.size() - dt.daNghiViec;
    dt.dangLamViec = chamCong.soNhanVienDangLam();

    std::string homNay = Utils::ngayHomNay();
    auto dsCaHomNay = lich.xemLichNgay(homNay);
    dt.soCaHomNay = (int)dsCaHomNay.size();
    std::set<std::string> nvDuocXep;
    for (const auto& ca : dsCaHomNay) {
        for (const auto& maNV : ca.getDsNhanVien()) nvDuocXep.insert(maNV);
    }
    dt.soNhanVienDuocXepCaHomNay = (int)nvDuocXep.size();
    return dt;
}

std::vector<std::string> QuanLyQuanCafe::layNhatKyGanDay() const {
    std::vector<std::string> ket;
    std::queue<std::string> tmp = nhatKyHoatDong; // copy để duyệt không phá hàng gốc
    while (!tmp.empty()) { ket.push_back(tmp.front()); tmp.pop(); }
    return ket;
}

void QuanLyQuanCafe::luuTatCa(const std::string& thuMuc) {
    FileManager::luuNhanVien(danhSachNV, thuMuc + "/nhanvien.csv");
    lich.luuFile(thuMuc + "/lichlamviec.csv");
    chamCong.luuFile(thuMuc + "/chamcong.csv");
    bangLuong.luuFile(thuMuc + "/luong.csv");
}

void QuanLyQuanCafe::taiTatCa(const std::string& thuMuc) {
    danhSachNV = FileManager::taiNhanVien(thuMuc + "/nhanvien.csv");
    xayDungChiSo();
    lich.taiFile(thuMuc + "/lichlamviec.csv");
    chamCong.taiFile(thuMuc + "/chamcong.csv");
    bangLuong.taiFile(thuMuc + "/luong.csv");
    ghiNhatKy("Da tai du lieu tu " + thuMuc);
}

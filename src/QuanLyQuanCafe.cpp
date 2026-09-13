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

KetQuaChamCong QuanLyQuanCafe::kiemTraVaChamCongVao(const std::string& maNV, const std::string& gioTuyChon) {
    NhanVien* nv = timNhanVien(maNV);
    if (!nv) {
        return {false, "Nhan vien khong ton tai!", "", "", 4};
    }
    if (nv->getTrangThai() == TrangThaiNV::DA_NGHI_VIEC) {
        return {false, "Nhan vien nay da nghi viec, khong the cham cong!", "", "", 4};
    }
    if (chamCong.dangTrongCa(maNV)) {
        return {false, "Ban dang trong ca lam viec, chua check-out ca truoc!", "", "", 4};
    }

    std::string ngay = Utils::ngayHomNay();
    std::string gio = gioTuyChon.empty() ? Utils::gioHienTai() : gioTuyChon;
    int phutHienTai = Utils::gioToPhut(gio);

    // Lấy các ca mà nhân viên được phân công trong ngày hôm nay
    auto dsCaNgay = lich.xemLichNgay(ngay);
    std::vector<CaLamViec> caCuaNV;
    for (const auto& c : dsCaNgay) {
        if (c.coNhanVien(maNV)) {
            caCuaNV.push_back(c);
        }
    }

    if (caCuaNV.empty()) {
        return {false, "Hom nay nhan vien " + maNV + " khong co lich lam viec nao duoc phan cong!", "", "", 3};
    }

    // Sắp xếp các ca của NV theo giờ bắt đầu tăng dần
    std::sort(caCuaNV.begin(), caCuaNV.end(), [](const CaLamViec& a, const CaLamViec& b) {
        return Utils::gioToPhut(a.getGioBatDau()) < Utils::gioToPhut(b.getGioBatDau());
    });

    // Lấy danh sách các ca mà NV đã hoàn thành trong ngày hôm nay
    std::vector<std::string> caDaXong;
    for (const auto& bg : chamCong.lichSuNgay(ngay)) {
        if (bg.maNV == maNV && !bg.gioRa.empty() && !bg.maCa.empty()) {
            caDaXong.push_back(bg.maCa);
        }
    }

    // Lọc các ca chưa hoàn thành
    std::vector<CaLamViec> caChuaLam;
    for (const auto& c : caCuaNV) {
        bool daXong = false;
        for (const auto& ma : caDaXong) {
            if (ma == c.getMaCa()) { daXong = true; break; }
        }
        if (!daXong) {
            caChuaLam.push_back(c);
        }
    }

    if (caChuaLam.empty()) {
        return {false, "Ban da hoan thanh tat ca cac ca lam viec duoc phan cong trong ngay hom nay!", "", "", 4};
    }

    // Tìm ca phù hợp để check-in:
    // Khung giờ hợp lệ: phutBatDau - 30 <= phutHienTai <= phutKetThuc
    const CaLamViec* caHopLe = nullptr;
    for (const auto& c : caChuaLam) {
        int phutBD = Utils::gioToPhut(c.getGioBatDau());
        int phutKT = Utils::gioToPhut(c.getGioKetThuc());
        int phutChoPhep = phutBD - 30;
        if (phutHienTai >= phutChoPhep && phutHienTai <= phutKT) {
            caHopLe = &c;
            break;
        }
    }

    if (caHopLe) {
        int phutBD = Utils::gioToPhut(caHopLe->getGioBatDau());
        std::string trangThai = (phutHienTai <= phutBD) ? "Dung gio" : "Di muon";
        chamCong.checkIn(maNV, ngay, gio, caHopLe->getMaCa(), trangThai);
        ghiNhatKy("Cham cong VAO: " + maNV + " - " + caHopLe->getMaCa() + " (" + trangThai + ")");

        std::string msg = "Cham cong VAO ca " + caHopLe->getTenCa() + " (" + caHopLe->getGioBatDau() + " - " + caHopLe->getGioKetThuc() + ") thanh cong!\nPhan loai: " + (trangThai == "Dung gio" ? "Dung gio" : "Di muon");
        return {true, msg, caHopLe->getMaCa(), trangThai, 0};
    }

    // Nếu không có ca nào trong khung giờ hợp lệ:
    // Tìm ca tiếp theo chưa đến giờ (phutHienTai < phutBD - 30)
    for (const auto& c : caChuaLam) {
        int phutBD = Utils::gioToPhut(c.getGioBatDau());
        int phutChoPhep = phutBD - 30;
        if (phutHienTai < phutChoPhep) {
            char b[16];
            snprintf(b, sizeof(b), "%02d:%02d", phutChoPhep / 60, phutChoPhep % 60);
            std::string gioChoPhep(b);
            std::string msg = "Chua den thoi gian cham cong!\nQuy dinh: Chi duoc cham cong truoc gio vao ca toi da 30 phut.\nBan co the check-in tu " + gioChoPhep + " cho ca " + c.getTenCa() + " [" + c.getGioBatDau() + " - " + c.getGioKetThuc() + "].";
            return {false, msg, c.getMaCa(), "", 1};
        }
    }

    // Nếu tất cả các ca đều đã qua giờ kết thúc
    return {false, "Cac ca lam viec trong ngay cua ban deu da qua gio ket thuc!", "", "", 4};
}

KetQuaChamCong QuanLyQuanCafe::kiemTraVaChamCongRa(const std::string& maNV, const std::string& gioTuyChon, bool chapNhanVeSom) {
    if (!chamCong.dangTrongCa(maNV)) {
        return {false, "Nhan vien chua check-in ca lam viec!", "", "", 4};
    }

    const BanGhiChamCong* bg = chamCong.layBanGhiDangMo(maNV);
    if (!bg) {
        return {false, "Khong tim thay ban ghi dang lam viec!", "", "", 4};
    }

    std::string ngay = bg->ngay;
    std::string gio = gioTuyChon.empty() ? Utils::gioHienTai() : gioTuyChon;
    int phutHienTai = Utils::gioToPhut(gio);

    CaLamViec* ca = lich.timCa(bg->ngay, bg->maCa);
    if (ca) {
        int phutKT = Utils::gioToPhut(ca->getGioKetThuc());
        if (phutHienTai < phutKT) {
            // Chưa đến giờ kết thúc ca quy định
            if (!chapNhanVeSom) {
                std::string msg = "Chua den gio ket thuc ca quy dinh (" + ca->getGioKetThuc() + ")!\nQuy dinh check-out phai tu " + ca->getGioKetThuc() + " tro di.";
                return {false, msg, ca->getGioKetThuc(), bg->trangThai, 2};
            } else {
                // Xác nhận về sớm
                std::string trangThaiMoi = (bg->trangThai == "Di muon") ? "Di muon & Ve som" : "Ve som";
                chamCong.checkOut(maNV, ngay, gio, trangThaiMoi);
                ghiNhatKy("Cham cong RA: " + maNV + " - " + bg->maCa + " (" + trangThaiMoi + ")");
                return {true, "Da check-out ca " + ca->getTenCa() + " thanh cong!\nPhan loai: " + trangThaiMoi, bg->maCa, trangThaiMoi, 0};
            }
        } else {
            // Đúng giờ kết thúc ca trở đi
            std::string tt = bg->trangThai;
            chamCong.checkOut(maNV, ngay, gio, tt);
            ghiNhatKy("Cham cong RA: " + maNV + " - " + bg->maCa + " (" + tt + ")");
            return {true, "Da check-out ca " + ca->getTenCa() + " thanh cong!\nPhan loai: " + (tt == "Dung gio" ? "Dung gio" : tt), bg->maCa, tt, 0};
        }
    }

    // Không tìm thấy thông tin ca (ngoại lệ ca tự do)
    chamCong.checkOut(maNV, ngay, gio, bg->trangThai);
    return {true, "Da check-out thanh cong!", "", bg->trangThai, 0};
}

bool QuanLyQuanCafe::chamCongVao(const std::string& maNV) {
    auto kq = kiemTraVaChamCongVao(maNV);
    return kq.thanhCong;
}

bool QuanLyQuanCafe::chamCongRa(const std::string& maNV) {
    auto kq = kiemTraVaChamCongRa(maNV, "", false);
    return kq.thanhCong;
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

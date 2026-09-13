#include <QApplication>
#include "QuanLyQuanCafe.h"
#include "TaiKhoan.h"
#include "LoginDialog.h"
#include "MainWindow.h"
#include <direct.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::string dataDir = "data";
    _mkdir(dataDir.c_str());

    QuanLyQuanCafe cafeApp;
    cafeApp.taiTatCa(dataDir);

    QuanLyTaiKhoan tkManager;
    tkManager.taiFile(dataDir + "/taikhoan.csv");

    // Tự động đồng bộ tài khoản cho các nhân viên sẵn có
    std::vector<std::string> dsMaNV;
    for (const auto& nv : cafeApp.layDanhSachNV()) {
        dsMaNV.push_back(nv->getMaNV());
    }
    tkManager.dongBoTaiKhoanNhanVien(dsMaNV);
    tkManager.luuFile(dataDir + "/taikhoan.csv");

    // Vòng lặp đăng nhập
    while (true) {
        LoginDialog loginDlg(tkManager);
        if (loginDlg.exec() != QDialog::Accepted) {
            // Người dùng bấm thoát hoặc đóng cửa sổ đăng nhập
            break;
        }

        MainWindow mainWin(cafeApp, tkManager, dataDir);
        mainWin.show();

        // Chạy vòng lặp sự kiện cho cửa sổ chính
        app.exec();

        // Nếu người dùng không đăng xuất mà tắt cửa sổ, thoát hẳn
        if (tkManager.daDangNhap()) {
            break;
        }
        // Nếu người dùng bấm "Đăng xuất", tkManager đã đăng xuất và vòng lặp tiếp tục mở lại LoginDialog
    }

    // Lưu dữ liệu lần cuối trước khi thoát
    cafeApp.luuTatCa(dataDir);
    tkManager.luuFile(dataDir + "/taikhoan.csv");

    return 0;
}

#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "NhanVien.h"

class EditEmployeeDialog : public QDialog {
    Q_OBJECT

private:
    NhanVien* nhanVien;
    std::string thuMucDuLieu;

    QLabel* lblAvatar;
    QPushButton* btnChonAnh;
    QPushButton* btnXoaAnh;
    QString duongDanAnhMoi;
    bool daXoaAnh;

    QLineEdit* editMaNV;
    QLineEdit* editHoTen;
    QLineEdit* editSDT;
    QLineEdit* editEmail;
    QLabel* lblLoaiNV;
    QLabel* lblNgayVaoLam;
    QComboBox* comboTrangThai;
    QLineEdit* editLuongCoBan;
    QLineEdit* editPhuCap;

    QPushButton* btnSave;
    QPushButton* btnCancel;

public:
    explicit EditEmployeeDialog(NhanVien* nv, const std::string& dataDir, QWidget* parent = nullptr);

private slots:
    void handleChonAnh();
    void handleXoaAnh();
    void handleSave();

private:
    void capNhatHienThiAvatar();
};

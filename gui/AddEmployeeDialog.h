#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <memory>
#include "NhanVien.h"

class AddEmployeeDialog : public QDialog {
    Q_OBJECT

private:
    QLineEdit* editMaNV;
    QLineEdit* editHoTen;
    QLineEdit* editSDT;
    QLineEdit* editEmail;
    QComboBox* comboLoaiNV;
    QLineEdit* editLuongCoBan;
    QLineEdit* editPhuCap;
    QPushButton* btnSave;
    QPushButton* btnCancel;

    std::unique_ptr<NhanVien> nhanVienMoi;

public:
    explicit AddEmployeeDialog(const std::string& maNVMoi, QWidget* parent = nullptr);

    std::unique_ptr<NhanVien> getNhanVienMoi();

private slots:
    void handleLoaiNVChanged(int index);
    void handleSave();
};

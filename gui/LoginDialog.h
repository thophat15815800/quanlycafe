#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "TaiKhoan.h"

class LoginDialog : public QDialog {
    Q_OBJECT

private:
    QuanLyTaiKhoan& qlTK;
    QLineEdit* editUser;
    QLineEdit* editPass;
    QPushButton* btnLogin;
    QPushButton* btnCancel;

public:
    explicit LoginDialog(QuanLyTaiKhoan& tkManager, QWidget* parent = nullptr);

private slots:
    void handleLogin();
};

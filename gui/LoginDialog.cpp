#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QIcon>

LoginDialog::LoginDialog(QuanLyTaiKhoan& tkManager, QWidget* parent)
    : QDialog(parent), qlTK(tkManager) {
    setWindowTitle("Quan Ly Quan Cafe - Dang Nhap");
    setFixedSize(380, 260);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Tiêu đề
    QLabel* lblTitle = new QLabel("QUAN LY QUAN CAFE", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #6F4E37;");
    mainLayout->addWidget(lblTitle);

    QLabel* lblSub = new QLabel("Vui long dang nhap de tiep tuc", this);
    lblSub->setAlignment(Qt::AlignCenter);
    lblSub->setStyleSheet("color: #777; font-size: 12px;");
    mainLayout->addWidget(lblSub);

    // Form nhập liệu
    editUser = new QLineEdit(this);
    editUser->setPlaceholderText("Ten dang nhap");
    editUser->setFixedHeight(36);
    editUser->setStyleSheet("padding: 5px 10px; border: 1px solid #ccc; border-radius: 4px; font-size: 13px;");
    mainLayout->addWidget(editUser);

    editPass = new QLineEdit(this);
    editPass->setPlaceholderText("Mat khau");
    editPass->setEchoMode(QLineEdit::Password);
    editPass->setFixedHeight(36);
    editPass->setStyleSheet("padding: 5px 10px; border: 1px solid #ccc; border-radius: 4px; font-size: 13px;");
    mainLayout->addWidget(editPass);

    // Hàng nút bấm
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLogin = new QPushButton("Dang Nhap", this);
    btnLogin->setFixedHeight(38);
    btnLogin->setStyleSheet("background-color: #6F4E37; color: white; font-weight: bold; border-radius: 4px; font-size: 13px;");

    btnCancel = new QPushButton("Thoat", this);
    btnCancel->setFixedHeight(38);
    btnCancel->setStyleSheet("background-color: #e0e0e0; color: #333; border-radius: 4px; font-size: 13px;");

    btnLayout->addWidget(btnLogin);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnLogin, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(editPass, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
}

void LoginDialog::handleLogin() {
    QString user = editUser->text().trimmed();
    QString pass = editPass->text().trimmed();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Canh bao", "Vui long nhap day du Ten dang nhap va Mat khau!");
        return;
    }

    if (qlTK.dangNhap(user.toStdString(), pass.toStdString())) {
        accept(); // Thành công đóng dialog với kết quả Accepted
    } else {
        QMessageBox::critical(this, "Loi dang nhap", "Sai Ten dang nhap hoac Mat khau!");
    }
}

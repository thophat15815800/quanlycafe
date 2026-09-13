#include "EditEmployeeDialog.h"
#include "AvatarHelper.h"
#include "ThuNgan.h"
#include "QuanLy.h"
#include "Utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>

EditEmployeeDialog::EditEmployeeDialog(NhanVien* nv, const std::string& dataDir, QWidget* parent)
    : QDialog(parent), nhanVien(nv), thuMucDuLieu(dataDir), daXoaAnh(false) {
    setWindowTitle("Chinh Sua Thong Tin Nhan Vien");
    setFixedSize(480, 560);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    QLabel* lblHeader = new QLabel("CHINH SUA THONG TIN NHAN VIEN", this);
    lblHeader->setAlignment(Qt::AlignCenter);
    lblHeader->setStyleSheet("font-size: 15px; font-weight: bold; color: #6F4E37; margin-bottom: 2px;");
    mainLayout->addWidget(lblHeader);

    // --- Khu vực Ảnh đại diện (Avatar) ---
    QHBoxLayout* avatarLayout = new QHBoxLayout();
    avatarLayout->setSpacing(16);

    lblAvatar = new QLabel(this);
    lblAvatar->setFixedSize(100, 100);
    lblAvatar->setAlignment(Qt::AlignCenter);
    avatarLayout->addWidget(lblAvatar);

    QVBoxLayout* avatarBtnLayout = new QVBoxLayout();
    avatarBtnLayout->setAlignment(Qt::AlignVCenter);
    avatarBtnLayout->setSpacing(8);

    btnChonAnh = new QPushButton("Chon anh dai dien...", this);
    btnChonAnh->setFixedHeight(32);
    btnChonAnh->setStyleSheet("background-color: #8D6E63; color: white; border-radius: 4px; padding: 0 10px; font-weight: 500;");

    btnXoaAnh = new QPushButton("Dat ve mac dinh", this);
    btnXoaAnh->setFixedHeight(32);
    btnXoaAnh->setStyleSheet("color: #757575; border-radius: 4px; padding: 0 10px;");

    avatarBtnLayout->addWidget(btnChonAnh);
    avatarBtnLayout->addWidget(btnXoaAnh);
    avatarLayout->addLayout(avatarBtnLayout);
    avatarLayout->addStretch();

    mainLayout->addLayout(avatarLayout);

    // --- Form nhập liệu ---
    QFormLayout* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);
    form->setSpacing(10);

    editMaNV = new QLineEdit(QString::fromStdString(nv->getMaNV()), this);
    editMaNV->setReadOnly(true);
    editMaNV->setStyleSheet("background-color: #f0f0f0; padding: 5px;");
    form->addRow("Ma nhan vien:", editMaNV);

    editHoTen = new QLineEdit(QString::fromStdString(nv->getHoTen()), this);
    editHoTen->setStyleSheet("padding: 5px;");
    form->addRow("Ho va ten (*):", editHoTen);

    lblLoaiNV = new QLabel(QString::fromStdString(nv->loaiNhanVien()), this);
    lblLoaiNV->setStyleSheet("font-weight: bold; color: #6F4E37; padding: 5px;");
    form->addRow("Vi tri cong viec:", lblLoaiNV);

    lblNgayVaoLam = new QLabel(QString::fromStdString(nv->getNgayVaoLam()), this);
    lblNgayVaoLam->setStyleSheet("padding: 5px; color: #555;");
    form->addRow("Ngay vao lam:", lblNgayVaoLam);

    editSDT = new QLineEdit(QString::fromStdString(nv->getSdt()), this);
    editSDT->setStyleSheet("padding: 5px;");
    form->addRow("So dien thoai (*):", editSDT);

    editEmail = new QLineEdit(QString::fromStdString(nv->getEmail()), this);
    editEmail->setStyleSheet("padding: 5px;");
    form->addRow("Email (*):", editEmail);

    comboTrangThai = new QComboBox(this);
    comboTrangThai->addItem("Dang lam viec", static_cast<int>(TrangThaiNV::DANG_LAM));
    comboTrangThai->addItem("Tam nghi", static_cast<int>(TrangThaiNV::TAM_NGHI));
    comboTrangThai->addItem("Da nghi viec", static_cast<int>(TrangThaiNV::DA_NGHI_VIEC));
    comboTrangThai->setCurrentIndex(static_cast<int>(nv->getTrangThai()));
    comboTrangThai->setStyleSheet("padding: 5px;");
    form->addRow("Trang thai:", comboTrangThai);

    editLuongCoBan = new QLineEdit(QString::number(nv->getLuongCoBan(), 'f', 0), this);
    editLuongCoBan->setStyleSheet("padding: 5px;");
    form->addRow("Luong co ban (*):", editLuongCoBan);

    editPhuCap = new QLineEdit(this);
    editPhuCap->setStyleSheet("padding: 5px;");
    double phuCapHienTai = 0;
    bool coPhuCap = false;

    if (auto tn = dynamic_cast<ThuNgan*>(nv)) {
        phuCapHienTai = tn->getPhuCap();
        coPhuCap = true;
    } else if (auto ql = dynamic_cast<QuanLy*>(nv)) {
        phuCapHienTai = ql->getPhuCapQuanLy();
        coPhuCap = true;
    }

    if (coPhuCap) {
        editPhuCap->setText(QString::number(phuCapHienTai, 'f', 0));
        form->addRow("Phu cap trach nhiem:", editPhuCap);
    } else {
        editPhuCap->setEnabled(false);
    }

    mainLayout->addLayout(form);

    // --- Nút hành động ---
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Luu thay doi", this);
    btnSave->setFixedHeight(36);
    btnSave->setStyleSheet("background-color: #6F4E37; color: white; font-weight: bold; border-radius: 4px;");

    btnCancel = new QPushButton("Huy bo", this);
    btnCancel->setFixedHeight(36);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnChonAnh, &QPushButton::clicked, this, &EditEmployeeDialog::handleChonAnh);
    connect(btnXoaAnh, &QPushButton::clicked, this, &EditEmployeeDialog::handleXoaAnh);
    connect(editHoTen, &QLineEdit::textChanged, this, [this]() { capNhatHienThiAvatar(); });
    connect(btnSave, &QPushButton::clicked, this, &EditEmployeeDialog::handleSave);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    capNhatHienThiAvatar();
}

void EditEmployeeDialog::capNhatHienThiAvatar() {
    std::string ten = editHoTen->text().trimmed().toStdString();
    if (ten.empty()) ten = nhanVien->getHoTen();

    if (!duongDanAnhMoi.isEmpty()) {
        QPixmap pix(duongDanAnhMoi);
        if (!pix.isNull()) {
            lblAvatar->setPixmap(AvatarHelper::getCircularPixmap(pix, 100));
            return;
        }
    }

    if (daXoaAnh) {
        lblAvatar->setPixmap(AvatarHelper::getDefaultAvatar(ten, 100));
        return;
    }

    lblAvatar->setPixmap(AvatarHelper::getAvatarPixmap(nhanVien->getAvatar(), ten, 100));
}

void EditEmployeeDialog::handleChonAnh() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Chon anh dai dien cho nhan vien",
        "",
        "Hinh anh (*.png *.jpg *.jpeg *.bmp)"
    );

    if (!filePath.isEmpty()) {
        duongDanAnhMoi = filePath;
        daXoaAnh = false;
        capNhatHienThiAvatar();
    }
}

void EditEmployeeDialog::handleXoaAnh() {
    duongDanAnhMoi.clear();
    daXoaAnh = true;
    capNhatHienThiAvatar();
}

void EditEmployeeDialog::handleSave() {
    std::string ten = editHoTen->text().trimmed().toStdString();
    std::string sdt = editSDT->text().trimmed().toStdString();
    std::string email = editEmail->text().trimmed().toStdString();

    if (ten.empty()) {
        QMessageBox::warning(this, "Thieu thong tin", "Vui long nhap Ho va ten!");
        editHoTen->setFocus();
        return;
    }

    if (!Utils::isValidSDT(sdt)) {
        QMessageBox::warning(this, "Loi dinh dang", "So dien thoai khong hop le (10-11 chu so, bat dau bang so 0)!");
        editSDT->setFocus();
        return;
    }

    if (!Utils::isValidEmail(email)) {
        QMessageBox::warning(this, "Loi dinh dang", "Email khong hop le (can co @ va ten mien hop le)!");
        editEmail->setFocus();
        return;
    }

    bool okLuong = false;
    double luong = editLuongCoBan->text().trimmed().toDouble(&okLuong);
    if (!okLuong || luong <= 0) {
        QMessageBox::warning(this, "Loi dinh dang", "Muc luong phai la so duong (> 0)!");
        editLuongCoBan->setFocus();
        return;
    }

    double phuCap = 0;
    if (editPhuCap->isEnabled()) {
        bool okPC = false;
        phuCap = editPhuCap->text().trimmed().toDouble(&okPC);
        if (!okPC || phuCap < 0) {
            QMessageBox::warning(this, "Loi dinh dang", "Muc phu cap khong hop le (>= 0)!");
            editPhuCap->setFocus();
            return;
        }
    }

    // Xử lý lưu ảnh đại diện
    if (!duongDanAnhMoi.isEmpty()) {
        QString relativePath = AvatarHelper::saveAvatarImage(duongDanAnhMoi, nhanVien->getMaNV(), thuMucDuLieu);
        if (!relativePath.isEmpty()) {
            nhanVien->setAvatar(relativePath.toStdString());
        }
    } else if (daXoaAnh) {
        nhanVien->setAvatar("");
    }

    // Cập nhật các thông tin cơ bản
    nhanVien->setHoTen(ten);
    nhanVien->setSdt(sdt);
    nhanVien->setEmail(email);
    nhanVien->setTrangThai(static_cast<TrangThaiNV>(comboTrangThai->currentIndex()));
    nhanVien->setLuongCoBan(luong);

    // Cập nhật phụ cấp nếu có
    if (auto tn = dynamic_cast<ThuNgan*>(nhanVien)) {
        tn->setPhuCap(phuCap);
    } else if (auto ql = dynamic_cast<QuanLy*>(nhanVien)) {
        ql->setPhuCapQuanLy(phuCap);
    }

    accept();
}

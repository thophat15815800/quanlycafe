#include "AddEmployeeDialog.h"
#include "AvatarHelper.h"
#include "PhaChe.h"
#include "ThuNgan.h"
#include "QuanLy.h"
#include "PhuVu.h"
#include "BaoVe.h"
#include "Utils.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>

AddEmployeeDialog::AddEmployeeDialog(const std::string& maNVMoi, const std::string& dataDir, QWidget* parent)
    : QDialog(parent), thuMucDuLieu(dataDir) {
    setWindowTitle("Them Nhan Vien Moi");
    setFixedSize(450, 540);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 16);
    mainLayout->setSpacing(10);

    QLabel* lblHeader = new QLabel("THONG TIN NHAN VIEN MOI", this);
    lblHeader->setAlignment(Qt::AlignCenter);
    lblHeader->setStyleSheet("font-size: 15px; font-weight: bold; color: #6F4E37; margin-bottom: 2px;");
    mainLayout->addWidget(lblHeader);

    // Khu vực chọn Avatar
    QHBoxLayout* avatarLayout = new QHBoxLayout();
    avatarLayout->setSpacing(14);

    lblAvatar = new QLabel(this);
    lblAvatar->setFixedSize(72, 72);
    lblAvatar->setAlignment(Qt::AlignCenter);
    avatarLayout->addWidget(lblAvatar);

    QVBoxLayout* avatarBtnLayout = new QVBoxLayout();
    avatarBtnLayout->setAlignment(Qt::AlignVCenter);
    avatarBtnLayout->setSpacing(6);

    btnChonAnh = new QPushButton("Chon anh dai dien...", this);
    btnChonAnh->setFixedHeight(30);
    btnChonAnh->setStyleSheet("background-color: #8D6E63; color: white; border-radius: 4px; padding: 0 10px; font-size: 12px;");

    btnXoaAnh = new QPushButton("Dat ve mac dinh", this);
    btnXoaAnh->setFixedHeight(28);
    btnXoaAnh->setStyleSheet("color: #757575; border-radius: 4px; padding: 0 10px; font-size: 12px;");

    avatarBtnLayout->addWidget(btnChonAnh);
    avatarBtnLayout->addWidget(btnXoaAnh);
    avatarLayout->addLayout(avatarBtnLayout);
    avatarLayout->addStretch();
    mainLayout->addLayout(avatarLayout);

    QFormLayout* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);
    form->setSpacing(8);

    editMaNV = new QLineEdit(QString::fromStdString(maNVMoi), this);
    editMaNV->setReadOnly(true);
    editMaNV->setStyleSheet("background-color: #f0f0f0; padding: 4px;");
    form->addRow("Ma nhan vien:", editMaNV);

    editHoTen = new QLineEdit(this);
    editHoTen->setPlaceholderText("VD: Nguyen Van An");
    form->addRow("Ho va ten (*):", editHoTen);

    editSDT = new QLineEdit(this);
    editSDT->setPlaceholderText("VD: 0912345678");
    form->addRow("So dien thoai (*):", editSDT);

    editEmail = new QLineEdit(this);
    editEmail->setPlaceholderText("VD: an@gmail.com");
    form->addRow("Email (*):", editEmail);

    comboLoaiNV = new QComboBox(this);
    comboLoaiNV->addItem("Pha Che (Luong theo gio)", "PHA_CHE");
    comboLoaiNV->addItem("Thu Ngan (Luong gio + Phu cap)", "THU_NGAN");
    comboLoaiNV->addItem("Quan Ly (Luong cung + Phu cap)", "QUAN_LY");
    comboLoaiNV->addItem("Phu Vu (Luong theo gio)", "PHU_VU");
    comboLoaiNV->addItem("Bao Ve (Luong cung/thang)", "BAO_VE");
    form->addRow("Vi tri cong viec:", comboLoaiNV);

    editLuongCoBan = new QLineEdit(this);
    editLuongCoBan->setPlaceholderText("VD: 25000 (gio) hoac 8000000 (cung)");
    form->addRow("Luong co ban (*):", editLuongCoBan);

    editPhuCap = new QLineEdit(this);
    editPhuCap->setPlaceholderText("VD: 500000 (neu co)");
    editPhuCap->setEnabled(false); // Mặc định Pha chế không có phụ cấp
    form->addRow("Phu cap:", editPhuCap);

    mainLayout->addLayout(form);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnSave = new QPushButton("Luu nhan vien", this);
    btnSave->setFixedHeight(36);
    btnSave->setStyleSheet("background-color: #6F4E37; color: white; font-weight: bold; border-radius: 4px;");

    btnCancel = new QPushButton("Huy bo", this);
    btnCancel->setFixedHeight(36);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);
    mainLayout->addLayout(btnLayout);

    connect(btnChonAnh, &QPushButton::clicked, this, &AddEmployeeDialog::handleChonAnh);
    connect(btnXoaAnh, &QPushButton::clicked, this, &AddEmployeeDialog::handleXoaAnh);
    connect(editHoTen, &QLineEdit::textChanged, this, &AddEmployeeDialog::capNhatAvatarPreview);
    connect(comboLoaiNV, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AddEmployeeDialog::handleLoaiNVChanged);
    connect(btnSave, &QPushButton::clicked, this, &AddEmployeeDialog::handleSave);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    capNhatAvatarPreview();
}

void AddEmployeeDialog::capNhatAvatarPreview() {
    std::string ten = editHoTen->text().trimmed().toStdString();
    if (ten.empty()) ten = "Nhan Vien";

    if (!duongDanAnh.isEmpty()) {
        QPixmap pix(duongDanAnh);
        if (!pix.isNull()) {
            lblAvatar->setPixmap(AvatarHelper::getCircularPixmap(pix, 72));
            return;
        }
    }
    lblAvatar->setPixmap(AvatarHelper::getDefaultAvatar(ten, 72));
}

void AddEmployeeDialog::handleChonAnh() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Chon anh dai dien cho nhan vien",
        "",
        "Hinh anh (*.png *.jpg *.jpeg *.bmp)"
    );

    if (!filePath.isEmpty()) {
        duongDanAnh = filePath;
        capNhatAvatarPreview();
    }
}

void AddEmployeeDialog::handleXoaAnh() {
    duongDanAnh.clear();
    capNhatAvatarPreview();
}

void AddEmployeeDialog::handleLoaiNVChanged(int index) {
    // Thu ngân và Quản lý mới có phụ cấp
    if (index == 1 || index == 2) {
        editPhuCap->setEnabled(true);
    } else {
        editPhuCap->setEnabled(false);
        editPhuCap->clear();
    }
}

void AddEmployeeDialog::handleSave() {
    std::string ma = editMaNV->text().trimmed().toStdString();
    std::string ten = editHoTen->text().trimmed().toStdString();
    std::string sdt = editSDT->text().trimmed().toStdString();
    std::string email = editEmail->text().trimmed().toStdString();
    std::string ngay = Utils::ngayHomNay();

    if (ten.empty()) {
        QMessageBox::warning(this, "Thieu thong tin", "Vui long nhap Ho ten!");
        return;
    }

    if (!Utils::isValidSDT(sdt)) {
        QMessageBox::warning(this, "Loi dinh dang", "So dien thoai khong hop le (10-11 chu so, bat dau bang so 0)!");
        return;
    }

    if (!Utils::isValidEmail(email)) {
        QMessageBox::warning(this, "Loi dinh dang", "Email khong hop le (phai co @ va domain .com...)!");
        return;
    }

    bool okLuong = false;
    double luong = editLuongCoBan->text().trimmed().toDouble(&okLuong);
    if (!okLuong || luong <= 0) {
        QMessageBox::warning(this, "Loi dinh dang", "Vui long nhap muc luong hop le (> 0)!");
        return;
    }

    double phuCap = 0;
    if (editPhuCap->isEnabled() && !editPhuCap->text().trimmed().isEmpty()) {
        bool okPC = false;
        phuCap = editPhuCap->text().trimmed().toDouble(&okPC);
        if (!okPC || phuCap < 0) {
            QMessageBox::warning(this, "Loi dinh dang", "Muc phu cap khong hop le!");
            return;
        }
    }

    // Xử lý copy file ảnh đại diện nếu có
    std::string avatarPath = "";
    if (!duongDanAnh.isEmpty()) {
        QString relPath = AvatarHelper::saveAvatarImage(duongDanAnh, ma, thuMucDuLieu);
        avatarPath = relPath.toStdString();
    }

    int loaiIdx = comboLoaiNV->currentIndex();
    if (loaiIdx == 0) { // Pha chế
        nhanVienMoi = std::make_unique<PhaChe>(ma, ten, sdt, email, ngay, luong, avatarPath);
    } else if (loaiIdx == 1) { // Thu ngân
        nhanVienMoi = std::make_unique<ThuNgan>(ma, ten, sdt, email, ngay, luong, phuCap, avatarPath);
    } else if (loaiIdx == 2) { // Quản lý
        nhanVienMoi = std::make_unique<QuanLy>(ma, ten, sdt, email, ngay, luong, phuCap, avatarPath);
    } else if (loaiIdx == 3) { // Phục vụ
        nhanVienMoi = std::make_unique<PhuVu>(ma, ten, sdt, email, ngay, luong, avatarPath);
    } else if (loaiIdx == 4) { // Bảo vệ
        nhanVienMoi = std::make_unique<BaoVe>(ma, ten, sdt, email, ngay, luong, avatarPath);
    }

    accept();
}

std::unique_ptr<NhanVien> AddEmployeeDialog::getNhanVienMoi() {
    return std::move(nhanVienMoi);
}

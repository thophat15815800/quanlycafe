#pragma once
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <string>
#include <vector>

class AvatarHelper {
public:
    // Cắt ảnh thành hình tròn với viền trang nhã
    static QPixmap getCircularPixmap(const QPixmap& src, int size) {
        if (src.isNull()) return QPixmap();

        QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        // Cắt vùng trung tâm
        int x = (scaled.width() - size) / 2;
        int y = (scaled.height() - size) / 2;
        QPixmap cropped = scaled.copy(x, y, size, size);

        QPixmap dest(size, size);
        dest.fill(Qt::transparent);

        QPainter painter(&dest);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        QPainterPath path;
        path.addEllipse(1, 1, size - 2, size - 2);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, cropped);

        // Vẽ đường viền tròn nhẹ nhàng
        painter.setClipping(false);
        QPen pen(QColor("#BCAAA4"), 1.5);
        painter.setPen(pen);
        painter.drawEllipse(1, 1, size - 2, size - 2);

        return dest;
    }

    // Tạo avatar mặc định với chữ cái viết tắt của tên nhân viên trên nền màu phong cách cafe
    static QPixmap getDefaultAvatar(const std::string& hoTen, int size) {
        QPixmap dest(size, size);
        dest.fill(Qt::transparent);

        // Bảng màu trang nhã
        static const std::vector<QColor> colors = {
            QColor("#6F4E37"), // Cafe Mocha
            QColor("#8D6E63"), // Light Brown
            QColor("#4E342E"), // Espresso
            QColor("#5D4037"), // Dark Coffee
            QColor("#795548"), // Brown
            QColor("#2E7D32"), // Forest Green
            QColor("#00838F"), // Cyan Dark
            QColor("#1565C0"), // Blue
            QColor("#C62828"), // Red
            QColor("#E65100")  // Deep Orange
        };

        size_t hash = 0;
        for (char c : hoTen) hash = (hash * 31 + static_cast<unsigned char>(c));
        QColor bgColor = colors[hash % colors.size()];

        // Trích xuất 1-2 chữ cái đầu (VD: "Nguyen Van An" -> "NA")
        QString nameStr = QString::fromStdString(hoTen).trimmed();
        QString initials = "NV";
        if (!nameStr.isEmpty()) {
            QStringList parts = nameStr.split(' ', Qt::SkipEmptyParts);
            if (parts.size() == 1) {
                initials = parts[0].left(1).toUpper();
            } else if (parts.size() >= 2) {
                initials = parts.first().left(1).toUpper() + parts.last().left(1).toUpper();
            }
        }

        QPainter painter(&dest);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // Vẽ vòng tròn nền
        painter.setBrush(QBrush(bgColor));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(1, 1, size - 2, size - 2);

        // Vẽ chữ viết tắt
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPixelSize(size * 0.42);
        painter.setFont(font);
        painter.drawText(dest.rect(), Qt::AlignCenter, initials);

        // Đường viền ngoài
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(255, 255, 255, 120), 1.5));
        painter.drawEllipse(1, 1, size - 2, size - 2);

        return dest;
    }

    // Tự động tải ảnh từ file hoặc fallback về avatar mặc định
    static QPixmap getAvatarPixmap(const std::string& avatarPath, const std::string& hoTen, int size) {
        if (!avatarPath.empty()) {
            QString qPath = QString::fromStdString(avatarPath);
            QFileInfo fi(qPath);

            // Kiểm tra đường dẫn trực tiếp
            if (!fi.exists() || !fi.isFile()) {
                // Thử đường dẫn tương đối tới thư mục ứng dụng
                fi = QFileInfo(QCoreApplication::applicationDirPath() + "/" + qPath);
            }
            if (!fi.exists() || !fi.isFile()) {
                // Thử kiểm tra thư mục làm việc hiện tại
                fi = QFileInfo(QDir::currentPath() + "/" + qPath);
            }

            if (fi.exists() && fi.isFile()) {
                QPixmap pix(fi.absoluteFilePath());
                if (!pix.isNull()) {
                    return getCircularPixmap(pix, size);
                }
            }
        }
        return getDefaultAvatar(hoTen, size);
    }

    // Sao chép ảnh từ nguồn vào thư mục data/avatars/<maNV>.<ext>
    static QString saveAvatarImage(const QString& sourcePath, const std::string& maNV, const std::string& dataDir) {
        if (sourcePath.isEmpty()) return "";

        QFileInfo srcInfo(sourcePath);
        if (!srcInfo.exists() || !srcInfo.isFile()) return "";

        // Tạo thư mục data/avatars nếu chưa có
        QString targetDirStr = QString::fromStdString(dataDir) + "/avatars";
        QDir().mkpath(targetDirStr);

        QString ext = srcInfo.suffix().toLower();
        if (ext.isEmpty()) ext = "png";

        QString targetFileName = QString::fromStdString(maNV) + "." + ext;
        QString targetFilePath = targetDirStr + "/" + targetFileName;

        // Xóa file cũ nếu đã tồn tại
        if (QFile::exists(targetFilePath)) {
            QFile::remove(targetFilePath);
        }

        if (QFile::copy(sourcePath, targetFilePath)) {
            // Trả về đường dẫn lưu trữ tương đối
            return QString::fromStdString(dataDir) + "/avatars/" + targetFileName;
        }

        return sourcePath;
    }
};

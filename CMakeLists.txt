cmake_minimum_required(VERSION 3.16)
project(quanlycafe_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(include gui)

set(BACKEND_SOURCES
    src/BangLuong.cpp
    src/BaoVe.cpp
    src/CaLamViec.cpp
    src/ChamCong.cpp
    src/FileManager.cpp
    src/LichLamViec.cpp
    src/NhanVien.cpp
    src/PhaChe.cpp
    src/PhuVu.cpp
    src/QuanLy.cpp
    src/QuanLyQuanCafe.cpp
    src/TaiKhoan.cpp
    src/ThuNgan.cpp
)

set(GUI_SOURCES
    gui/LoginDialog.h
    gui/LoginDialog.cpp
    gui/AddEmployeeDialog.h
    gui/AddEmployeeDialog.cpp
    gui/MainWindow.h
    gui/MainWindow.cpp
    gui/main_gui.cpp
)

add_executable(quanlycafe_gui ${BACKEND_SOURCES} ${GUI_SOURCES})

target_link_libraries(quanlycafe_gui PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

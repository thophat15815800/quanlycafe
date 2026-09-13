@echo off
echo ===================================================
echo   DANG BIEN DICH UNG DUNG QUAN LY CAFE (QT6 GUI)
echo ===================================================
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [LOI] Khong tim thay CMake hoac trinh bien dich trong he thong!
    echo Neu ban dung MSYS2, hay kiem tra thu muc C:\msys64\ucrt64\bin.
    echo Xem huong dan cai dat trong README.md.
    echo.
    pause
    exit /b 1
)

echo [1/3] Dang cau hinh CMake...
cmake -B build_gui -G "Ninja" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo.
    echo [LOI] Cau hinh CMake that bai! Co the may ban chua cai Qt6 Base.
    echo Hay mo MSYS2 UCRT64 va chay lenh:
    echo   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-qt6-base
    echo.
    pause
    exit /b 1
)

echo.
echo [2/3] Dang bien dich ma nguon C++...
cmake --build build_gui
if %errorlevel% neq 0 (
    echo.
    echo [LOI] Bien dich that bai!
    pause
    exit /b 1
)

echo.
echo [3/3] Sao chep file thuc thi...
copy /y "build_gui\quanlycafe_gui.exe" "quanlycafe_gui.exe" >nul

echo.
echo ===================================================
echo   BIEN DICH HOAN TAT THANH CONG!
echo   Da tao file: quanlycafe_gui.exe
echo   Ban co the chay ung dung bang file Chay_App_GUI.bat
echo ===================================================
pause

@echo off
set "PATH=C:\msys64\ucrt64\bin;%PATH%"

if not exist "%~dp0quanlycafe_gui.exe" (
    echo ===================================================
    echo [THONG BAO] Chua tim thay file quanlycafe_gui.exe!
    echo Nguyen nhan: Ban moi tai ma nguon tu GitHub ve va chua bien dich.
    echo He thong se tu dong chuyen sang qua trinh bien dich...
    echo ===================================================
    echo.
    call "%~dp0Build_GUI.bat"
    if not exist "%~dp0quanlycafe_gui.exe" (
        echo [LOI] Khong the khoi chay vi qua trinh bien dich chua thanh cong.
        pause
        exit /b 1
    )
)

start "" "%~dp0quanlycafe_gui.exe"

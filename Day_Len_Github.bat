@echo off
title Day Code Len GitHub
cd /d "%~dp0"
echo ======================================================================
echo           TIEN HANH DAY MA NGUON LEN GITHUB
echo   Kho chua: https://github.com/thophat15815800/quanlycafe
echo ======================================================================
echo.
echo Neu trinh duyet bat len, vui long bam "Authorize" de dang nhap GitHub.
echo.
"C:\Program Files\Git\cmd\git.exe" push -u origin main
if %ERRORLEVEL% equ 0 (
    echo.
    echo ======================================================================
    echo   [THANH CONG] Da day toan bo ma nguon len GitHub thanh cong!
    echo ======================================================================
) else (
    echo.
    echo ======================================================================
    echo   [CHU Y] Neu chua dang nhap duoc hoac bao loi quyen, ban co the
    echo   dung Personal Access Token (PAT) hoac kiem tra lai tai khoan GitHub.
    echo ======================================================================
)
echo.
pause

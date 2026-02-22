@echo off
setlocal

cd /d "%~dp0"

echo ========================================
echo  Building 2DNGE (Debug)
echo ========================================

cmake --build build --config Debug
if %ERRORLEVEL% neq 0 (
    echo.
    echo BUILD FAILED
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo  Running 2DNGE
echo ========================================
echo.

build\app\Debug\2dnge_app.exe

echo.
pause
exit /b 0

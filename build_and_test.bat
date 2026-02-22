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
    exit /b 1
)

echo.
echo ========================================
echo  Running All Tests
echo ========================================
echo.

cd build
ctest --output-on-failure -C Debug
set TEST_RESULT=%ERRORLEVEL%
cd ..

echo.
if %TEST_RESULT% equ 0 (
    echo ========================================
    echo  ALL TESTS PASSED
    echo ========================================
) else (
    echo ========================================
    echo  SOME TESTS FAILED
    echo ========================================
)

echo.
pause
exit /b %TEST_RESULT%

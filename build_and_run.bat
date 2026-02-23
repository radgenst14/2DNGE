@echo off
setlocal

cd /d "%~dp0"

:: ── Parse arguments ──────────────────────────────────
:: Usage: build_and_run.bat [test|run|all]
::   test  - build and run tests only
::   run   - build and run the app only
::   all   - build and run tests, then run the app (default)

set MODE=%1
if "%MODE%"=="" set MODE=all

:: ── Configure (only when needed) ─────────────────────
if not exist build\CMakeCache.txt (
    echo ========================================
    echo  Configuring CMake
    echo ========================================
    cmake -B build -S .
    if %ERRORLEVEL% neq 0 (
        echo.
        echo CONFIGURE FAILED
        pause
        exit /b 1
    )
    echo.
)

:: ── Build ────────────────────────────────────────────
if "%MODE%"=="test" goto :build_tests
if "%MODE%"=="run"  goto :build_app
goto :build_all

:build_all
echo ========================================
echo  Building ALL (Debug)
echo ========================================
cmake --build build --config Debug
if %ERRORLEVEL% neq 0 goto :build_failed
echo.
goto :run_phase

:build_tests
echo ========================================
echo  Building Tests (Debug)
echo ========================================
cmake --build build --target 2dnge_tests --config Debug
if %ERRORLEVEL% neq 0 goto :build_failed
echo.
goto :run_phase

:build_app
echo ========================================
echo  Building App (Debug)
echo ========================================
cmake --build build --target 2dnge_app --config Debug
if %ERRORLEVEL% neq 0 goto :build_failed
echo.
goto :run_phase

:: ── Run ──────────────────────────────────────────────
:run_phase
if "%MODE%"=="run" goto :run_app

:: Run tests for "test" and "all" modes
echo ========================================
echo  Running Tests
echo ========================================
echo.
ctest --test-dir build -C Debug --output-on-failure
if %ERRORLEVEL% neq 0 (
    echo.
    echo ========================================
    echo  TESTS FAILED
    echo ========================================
    pause
    exit /b 1
)
echo.
echo ========================================
echo  All tests passed!
echo ========================================
echo.

if "%MODE%"=="test" goto :done

:run_app
echo ========================================
echo  Running 2DNGE
echo ========================================
echo.
build\app\Debug\2dnge_app.exe
echo.
goto :done

:done
pause
exit /b 0

:build_failed
echo.
echo ========================================
echo  BUILD FAILED
echo ========================================
pause
exit /b 1

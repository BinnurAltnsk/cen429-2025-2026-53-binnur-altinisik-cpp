@echo off
@setlocal enableextensions
@cd /d "%~dp0"

rem Test Coverage Raporu Oluşturma Script'i (Windows)
rem Bu script, OpenCppCoverage kullanarak test coverage raporu oluşturur

echo ========================================
echo Test Coverage Raporu Oluşturuluyor...
echo ========================================

rem Test executable'ın var olduğunu kontrol et
if not exist "build_win\build\Debug\travelexpense_tests.exe" (
    echo HATA: Test executable bulunamadı!
    echo Önce 8-build-test-windows.bat script'ini çalıştırın.
    pause
    exit /b 1
)

rem Coverage raporu dizinini oluştur
if not exist "docs\testcoverage" (
    mkdir "docs\testcoverage"
)

rem OpenCppCoverage ile coverage raporu oluştur
echo OpenCppCoverage ile coverage verisi toplanıyor...
OpenCppCoverage.exe ^
    --export_type=cobertura:docs\testcoverage\coverage.xml ^
    --sources src\travelexpense\src ^
    --sources src\travelexpense\header ^
    --sources src\travelexpenseapp\src ^
    --sources src\travelexpenseapp\header ^
    --excluded_sources src\travelexpense\third_party\* ^
    --excluded_sources src\tests\googletest\* ^
    --excluded_sources src\tests\*.cpp ^
    -- build_win\build\Debug\travelexpense_tests.exe

if %ERRORLEVEL% NEQ 0 (
    echo UYARI: OpenCppCoverage bulunamadı veya hata oluştu.
    echo OpenCppCoverage kurulu değilse, manuel olarak coverage raporu oluşturulamaz.
    echo Alternatif: Visual Studio Code Coverage aracını kullanabilirsiniz.
    pause
    exit /b 1
)

rem ReportGenerator ile HTML raporu oluştur (eğer kuruluysa)
where reportgenerator >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo ReportGenerator ile HTML raporu oluşturuluyor...
    reportgenerator ^
        "-title:TravelExpense Test Coverage Report (Windows)" ^
        "-reports:docs\testcoverage\coverage.xml" ^
        "-targetdir:docs\testcoverage\html" ^
        "-sourcedirs:src\travelexpense\src;src\travelexpense\header;src\travelexpenseapp\src;src\travelexpenseapp\header" ^
        "-reporttypes:Html;Badges" ^
        "-filefilters:-*third_party*;-*googletest*;-*tests\*.cpp"
    
    echo Coverage raporu oluşturuldu: docs\testcoverage\html\index.html
) else (
    echo UYARI: ReportGenerator bulunamadı.
    echo Coverage XML dosyası: docs\testcoverage\coverage.xml
    echo HTML raporu için ReportGenerator kurmanız gerekiyor.
)

rem Coverage özeti oluştur (basit analiz)
echo.
echo ========================================
echo Coverage Özeti
echo ========================================
echo Coverage XML dosyası: docs\testcoverage\coverage.xml
if exist "docs\testcoverage\html\index.html" (
    echo HTML Raporu: docs\testcoverage\html\index.html
)
echo.
echo Coverage raporu oluşturuldu!
echo.

pause


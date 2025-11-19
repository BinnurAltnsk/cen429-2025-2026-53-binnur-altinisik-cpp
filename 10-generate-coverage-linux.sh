#!/bin/bash

# Test Coverage Raporu Oluşturma Script'i (Linux)
# Bu script, lcov kullanarak test coverage raporu oluşturur

set -e

echo "========================================"
echo "Test Coverage Raporu Oluşturuluyor..."
echo "========================================"

# Test executable'ın var olduğunu kontrol et
if [ ! -f "build_linux/build/travelexpense_tests" ]; then
    echo "HATA: Test executable bulunamadı!"
    echo "Önce 7-build-app-linux.sh script'ini çalıştırın."
    exit 1
fi

# Coverage raporu dizinini oluştur
mkdir -p docs/testcoverage

# Testleri çalıştır (coverage verisi toplamak için)
echo "Testler çalıştırılıyor (coverage verisi toplanıyor)..."
cd build_linux
./build/travelexpense_tests
cd ..

# lcov ile coverage verisi topla
echo "lcov ile coverage verisi toplanıyor..."

# İlk capture (initial state)
lcov --rc lcov_branch_coverage=1 \
     --capture --initial \
     --directory build_linux \
     --output-file docs/testcoverage/coverage_base.info

# Test sonrası capture
lcov --rc lcov_branch_coverage=1 \
     --capture \
     --directory build_linux \
     --output-file docs/testcoverage/coverage_test.info

# İki coverage dosyasını birleştir
lcov --rc lcov_branch_coverage=1 \
     --add-tracefile docs/testcoverage/coverage_base.info \
     --add-tracefile docs/testcoverage/coverage_test.info \
     --output-file docs/testcoverage/coverage.info

# Sistem ve third-party dosyalarını hariç tut
lcov --rc lcov_branch_coverage=1 \
     --remove docs/testcoverage/coverage.info \
     '/usr/*' \
     '*/third_party/*' \
     '*/googletest/*' \
     '*/tests/*.cpp' \
     --output-file docs/testcoverage/coverage_filtered.info

# Coverage özeti göster
echo ""
echo "========================================"
echo "Coverage Özeti"
echo "========================================"
lcov --rc lcov_branch_coverage=1 \
     --list docs/testcoverage/coverage_filtered.info

# ReportGenerator ile HTML raporu oluştur (eğer kuruluysa)
if command -v reportgenerator &> /dev/null; then
    echo ""
    echo "ReportGenerator ile HTML raporu oluşturuluyor..."
    reportgenerator \
        "-title:TravelExpense Test Coverage Report (Linux)" \
        "-reports:docs/testcoverage/coverage_filtered.info" \
        "-targetdir:docs/testcoverage/html" \
        "-sourcedirs:src/travelexpense/src;src/travelexpense/header;src/travelexpenseapp/src;src/travelexpenseapp/header" \
        "-reporttypes:Html;Badges" \
        "-filefilters:-*third_party*;-*googletest*;-*tests/*.cpp"
    
    echo "Coverage raporu oluşturuldu: docs/testcoverage/html/index.html"
else
    echo ""
    echo "UYARI: ReportGenerator bulunamadı."
    echo "HTML raporu için genhtml kullanılıyor..."
    
    # genhtml ile HTML raporu oluştur
    genhtml --rc lcov_branch_coverage=1 \
            --branch-coverage \
            --output-directory docs/testcoverage/html \
            docs/testcoverage/coverage_filtered.info
    
    echo "Coverage raporu oluşturuldu: docs/testcoverage/html/index.html"
fi

echo ""
echo "========================================"
echo "Coverage Raporu Oluşturuldu!"
echo "========================================"
echo "Coverage info dosyası: docs/testcoverage/coverage_filtered.info"
echo "HTML Raporu: docs/testcoverage/html/index.html"
echo ""


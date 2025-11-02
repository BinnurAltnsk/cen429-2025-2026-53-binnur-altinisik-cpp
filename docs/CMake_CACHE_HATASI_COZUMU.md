# CMake Cache Hatası Çözümü

## Hata
```
Error: could not load cache
C:\Users\ksyar\Desktop\cen429-2025-2026-53-binnur-altinisik-cpp\out\build\x64-debug\...
```

## Sorun
CMake cache dosyası bozulmuş veya eksik olabilir. Bu durum genellikle build dizini tamamen silindiğinde veya CMake cache'i manuel olarak düzenlendiğinde oluşur.

## Çözüm Adımları

### Yöntem 1: Visual Studio ile Yeniden Configure
1. Visual Studio'da projeyi açın
2. **CMake** menüsünden **"Delete Cache and Reconfigure"** seçeneğini seçin
   - Veya Solution Explorer'da projeye sağ tıklayın
   - **"Delete Cache"** seçeneğini seçin
3. Ardından **"Configure"** seçeneğini seçin

### Yöntem 2: Manuel Olarak Cache Temizleme
1. Visual Studio'yu kapatın
2. `out\build\x64-debug` dizinindeki tüm dosyaları silin
3. Visual Studio'yu tekrar açın
4. Projeyi açtığınızda CMake otomatik olarak yeniden configure edecektir

### Yöntem 3: Terminal Üzerinden
```powershell
# Build dizinini tamamen sil
Remove-Item -Path "out\build\x64-debug" -Recurse -Force

# Visual Studio'yu tekrar açın ve projeyi açın
# CMake otomatik olarak yeniden configure edecektir
```

### Yöntem 4: CMake GUI ile
1. CMake GUI'yi açın
2. Source code: Projenin kök dizini
3. Build directory: `out\build\x64-debug`
4. **"Configure"** butonuna tıklayın
5. **"Generate"** butonuna tıklayın

## Önleyici Tedbirler
- CMake cache dosyalarını manuel olarak düzenlemeyin
- Build dizinini sadece gerektiğinde temizleyin
- Visual Studio'da CMake cache'i temizlemek için doğru menü seçeneklerini kullanın

## Not
Cache hatası genellikle kritik bir sorun değildir. CMake'i yeniden configure etmek genellikle sorunu çözer.


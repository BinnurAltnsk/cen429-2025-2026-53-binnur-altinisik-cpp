# SQLite3 Kurulum Kılavuzu

## Windows / Visual Studio için SQLite3 Kurulumu

Bu proje SQLite3 kullanmaktadır. Windows'ta SQLite3'ü kurmak için iki yöntem vardır:

### Yöntem 1: SQLite3 Amalgamation (Önerilen - En Kolay)

1. **SQLite3 Amalgamation Dosyalarını İndirin:**
   - https://www.sqlite.org/download.html adresine gidin
   - "Amalgamation" bölümünden şu dosyaları indirin:
     - `sqlite3.c` (tek C dosyası - tüm kaynak kod)
     - `sqlite3.h` (başlık dosyası)
   - Veya direkt link:
     - https://www.sqlite.org/2024/sqlite-amalgamation-3440300.zip

2. **Dosyaları Projeye Ekleyin:**
   - İndirdiğiniz zip dosyasını açın
   - `sqlite3.c` ve `sqlite3.h` dosyalarını şu klasöre kopyalayın:
     ```
     src/travelexpense/third_party/sqlite3/
     ```
   - Eğer klasör yoksa oluşturun

3. **CMakeLists.txt Güncellemesi:**
   - CMakeLists.txt dosyası otomatik olarak `src/travelexpense/third_party/sqlite3/` klasöründeki dosyaları bulacak şekilde yapılandırılacak

### Yöntem 2: vcpkg Kullanarak (Gelişmiş)

1. **vcpkg Kurulumu:**
   ```powershell
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   ```

2. **SQLite3 Kurulumu:**
   ```powershell
   .\vcpkg install sqlite3:x64-windows
   ```

3. **CMakeLists.txt'ye vcpkg entegrasyonu ekleyin:**
   ```cmake
   # vcpkg için
   set(CMAKE_TOOLCHAIN_FILE "C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake")
   ```

### Yöntem 3: Sistem SQLite3 Kullanımı (Manuel)

1. **SQLite3'ü Windows'a kurun:**
   - https://www.sqlite.org/download.html adresinden "Precompiled Binaries for Windows" indirin
   - İstediğiniz bir klasöre (örn: `C:\sqlite3\`) çıkarın

2. **Environment Variable Ekleyin:**
   - `SQLITE3_ROOT` environment variable'ını ekleyin
   - Değer: `C:\sqlite3\` (kurulum yolu)

3. **CMakeLists.txt Otomatik Bulacak:**
   - CMake `SQLITE3_ROOT` değişkenini otomatik olarak bulacak

## Kontrol

Kurulum sonrası projeyi build ettiğinizde, CMake çıktısında şunu görmelisiniz:

```
[src] SQLite3 found: 3.44.0
[src] SQLite3 libraries: sqlite3.lib (veya benzer)
[src] SQLite3 include dirs: ...
```

## Sorun Giderme

### "SQLite3 not found" Hatası

1. Dosyaların doğru yerde olduğundan emin olun
2. CMake cache'ini temizleyin:
   ```powershell
   Remove-Item -Recurse -Force build
   ```
3. Yeniden build edin

### Visual Studio IntelliSense Hatası

- Visual Studio'yu yeniden başlatın
- IntelliSense cache'ini temizleyin:
  - Tools → Options → Text Editor → C/C++ → Advanced
  - "Reset to Default" butonuna basın


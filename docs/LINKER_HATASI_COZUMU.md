# Process32FirstA/Process32NextA Linker Hatası Çözümü

## Sorun

Windows'ta `Process32FirstA` ve `Process32NextA` fonksiyonları için linker hatası:

```
Error LNK2019 unresolved external symbol Process32FirstA
Error LNK2019 unresolved external symbol Process32NextA
Error LNK1120 2 unresolved externals
```

## Çözüm

Bu fonksiyonlar `kernel32.dll` içinde bulunur ve `kernel32.lib` ile link edilmelidir.

### Yapılan Düzeltmeler

1. **CMakeLists.txt'de Windows kütüphaneleri eklendi:**
   ```cmake
   if(WIN32)
       target_link_libraries(${LIBNAME} PRIVATE
           kernel32
           psapi
           advapi32
       )
   endif()
   ```

2. **rasp.cpp'de #pragma comment direktifleri eklendi:**
   ```cpp
   #pragma comment(lib, "kernel32.lib")
   #pragma comment(lib, "psapi.lib")
   #pragma comment(lib, "advapi32.lib")
   ```

3. **Windows SDK sürümü tanımlandı:**
   ```cpp
   #define _WIN32_WINNT 0x0601  // Windows 7 or later
   ```

## Hata Devam Ediyorsa

### Adım 1: CMake Cache'i Temizle

Visual Studio'da:
1. Solution Explorer'da projeye sağ tıklayın
2. "Generate Cache" → "Delete Cache and Reconfigure" seçin
3. Veya manuel olarak `out/build/x64-debug` dizinini silin

Komut satırından:
```powershell
# Proje dizininde
Remove-Item -Recurse -Force out\build\x64-debug
```

### Adım 2: Solution'ı Temizle

Visual Studio'da:
1. Build → Clean Solution
2. Build → Rebuild Solution

### Adım 3: CMake'i Yeniden Configure Et

Visual Studio'da:
1. CMake → Configure
2. CMake → Build

### Adım 4: Manuel Kontrol

Eğer hata devam ediyorsa, linker komut satırını kontrol edin:

1. Visual Studio'da Build Output'u açın
2. Linker komut satırında `kernel32.lib` olup olmadığını kontrol edin
3. Eğer yoksa, CMakeLists.txt'yi tekrar kontrol edin

## Alternatif Çözüm: Explicit Library Path

Eğer hala sorun devam ediyorsa, Windows SDK path'ini manuel olarak belirtin:

```cmake
if(WIN32 AND MSVC)
    # Windows SDK path'ini bul
    get_filename_component(WIN_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE)
    if(EXISTS "${WIN_SDK_PATH}")
        set(WIN_SDK_LIB_DIR "${WIN_SDK_PATH}Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/${CMAKE_SYSTEM_PROCESSOR}")
        if(EXISTS "${WIN_SDK_LIB_DIR}/kernel32.lib")
            target_link_directories(${LIBNAME} PRIVATE ${WIN_SDK_LIB_DIR})
        endif()
    endif()
endif()
```

## Notlar

- `Process32FirstA` ve `Process32NextA` Windows 2000+ sürümlerinde mevcuttur
- `_WIN32_WINNT=0x0601` (Windows 7) tanımı gerekli
- `UNICODE` ve `_UNICODE` macro'ları undefined olmalı (ANSI versiyonlar kullanılıyor)
- `tlhelp32.h` header'ı include edilmeli

## İlgili Dosyalar

- `src/travelexpense/CMakeLists.txt` - CMake yapılandırması
- `src/travelexpense/src/rasp.cpp` - RASP implementasyonu
- `src/travelexpense/header/rasp.h` - RASP header

---

**Hazırlayan:** Binnur Altınışık  
**Tarih:** 2025


# SoftHSM Kurulum Kılavuzu

## Genel Bakış

Bu proje SoftHSM (Software HSM) kullanarak PKCS#11 standardı üzerinden kriptografik işlemler yapar. SoftHSM, donanım HSM'lerinin yazılım simülatörüdür ve test/geliştirme amaçlı kullanılır.

## SoftHSM Nedir?

SoftHSM, PKCS#11 standardını implement eden bir software HSM'dir. Güvenli anahtar yönetimi, şifreleme, imzalama ve diğer kriptografik işlemleri sağlar.

## Kurulum

### Windows

#### Yöntem 1: vcpkg ile Kurulum (Önerilen)

```powershell
# vcpkg kurulu değilse önce kurun
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# SoftHSM2'yi yükle
.\vcpkg install softhsm2:x64-windows
```

#### Yöntem 2: Manuel Kurulum

1. **SoftHSM2 İndirin:**
   - https://github.com/opendnssec/SoftHSMv2/releases adresinden en son sürümü indirin
   - Windows için installer'ı seçin

2. **Kurulum:**
   - İndirdiğiniz installer'ı çalıştırın
   - Varsayılan kurulum yolu: `C:\Program Files\SoftHSM2\`

3. **DLL Yolunu Ayarlayın:**
   - `softhsm2.dll` dosyası şu konumda olmalı:
     - `C:\Program Files\SoftHSM2\lib\softhsm2.dll`
     - Veya sistem PATH'e eklenmeli

### Linux / Ubuntu

#### Yöntem 1: Package Manager ile (Önerilen)

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install softhsm2

# Fedora/CentOS
sudo yum install softhsm2
# veya
sudo dnf install softhsm2
```

#### Yöntem 2: Kaynak Koddan Derleme

```bash
# Gerekli bağımlılıkları yükle
sudo apt-get install build-essential libssl-dev libsqlite3-dev pkg-config

# SoftHSM2'yi klonla
git clone https://github.com/opendnssec/SoftHSMv2.git
cd SoftHSMv2

# Derle ve kur
./configure
make
sudo make install
```

### macOS

```bash
# Homebrew ile
brew install softhsm
```

## Yapılandırma

### 1. SoftHSM Konfigürasyon Dosyası

SoftHSM konfigürasyon dosyası oluşturun:

**Linux/macOS:**
```bash
mkdir -p $HOME/.config/softhsm2
```

**Windows:**
```
C:\Users\<Kullanıcı>\AppData\Local\SoftHSM2\
```

Konfigürasyon dosyası: `softhsm2.conf`

#### Linux/macOS Örneği:

```bash
# $HOME/.config/softhsm2/softhsm2.conf
directories.tokendir = /home/kullanici/softhsm2-tokens
objectstore.backend = file

# Log ayarları
log.level = INFO
```

#### Windows Örneği:

```
# C:\Users\<Kullanıcı>\AppData\Local\SoftHSM2\softhsm2.conf
directories.tokendir = C:\Users\<Kullanıcı>\softhsm2-tokens
objectstore.backend = file

# Log ayarları
log.level = INFO
```

### 2. Token Dizini Oluştur

```bash
# Linux/macOS
mkdir -p $HOME/softhsm2-tokens
chmod 755 $HOME/softhsm2-tokens

# Windows
mkdir C:\Users\<Kullanıcı>\softhsm2-tokens
```

### 3. Environment Variable (Opsiyonel)

SoftHSM konfigürasyon dosyasının yerini belirtmek için:

**Linux/macOS:**
```bash
export SOFTHSM2_CONF=$HOME/.config/softhsm2/softhsm2.conf
```

**Windows:**
```powershell
$env:SOFTHSM2_CONF = "C:\Users\<Kullanıcı>\AppData\Local\SoftHSM2\softhsm2.conf"
```

### 4. Token Oluşturma

#### Komut Satırı ile:

```bash
# Token oluştur
softhsm2-util --init-token --slot 0 --label "TravelExpense" --pin 1234 --so-pin 0000

# Token listesi
softhsm2-util --show-slots
```

**Windows:**
```powershell
# SoftHSM2 kurulum dizininden
cd "C:\Program Files\SoftHSM2\bin"
.\softhsm2-util.exe --init-token --slot 0 --label "TravelExpense" --pin 1234 --so-pin 0000
```

## Proje Entegrasyonu

### Otomatik Kütüphane Bulma

Proje, SoftHSM kütüphanesini otomatik olarak bulmaya çalışır:

**Windows:**
- `softhsm2.dll` (current directory)
- `C:\Program Files\SoftHSM2\lib\softhsm2.dll`
- `C:\Program Files (x86)\SoftHSM2\lib\softhsm2.dll`

**Linux:**
- `libsofthsm2.so` (system library path)
- `/usr/lib/softhsm/libsofthsm2.so`
- `/usr/local/lib/softhsm/libsofthsm2.so`
- `/opt/softhsm2/lib/libsofthsm2.so`

### Manuel Kütüphane Yolu Belirtme

Eğer SoftHSM farklı bir konumdaysa, kod içinde manuel olarak belirtilebilir:

```cpp
TravelExpense::SoftHSM::initialize("/custom/path/to/softhsm2.dll");
```

## Kullanım Örneği

```cpp
#include "softhsm.h"

using namespace TravelExpense;

// SoftHSM'yi başlat
ErrorCode result = SoftHSM::initialize();
if (result != ErrorCode::SUCCESS) {
    // Hata yönetimi
}

// Token aç
result = SoftHSM::openToken("TravelExpense", "1234");
if (result != ErrorCode::SUCCESS) {
    // Hata yönetimi
}

// Anahtar oluştur
uint8_t keyId[8];
size_t keyIdLen = sizeof(keyId);
result = SoftHSM::generateKey(
    SoftHSM::KeyType::AES_256,
    SoftHSM::KeyUsage::ENCRYPT_DECRYPT,
    "MyKey",
    keyId,
    keyIdLen
);

// Veriyi şifrele
uint8_t plaintext[] = "Hello, World!";
uint8_t ciphertext[256];
size_t ciphertextLen = sizeof(ciphertext);
result = SoftHSM::encrypt(
    keyId, keyIdLen,
    plaintext, sizeof(plaintext) - 1,
    ciphertext, ciphertextLen,
    nullptr  // IV otomatik oluşturulur
);

// SoftHSM'yi kapat
SoftHSM::shutdown();
```

## Sorun Giderme

### "SoftHSM library not found" Hatası

1. SoftHSM'in kurulu olduğundan emin olun
2. Kütüphane yolunun doğru olduğunu kontrol edin
3. PATH environment variable'ına ekleyin (Windows)
4. `ldconfig` çalıştırın (Linux):

```bash
sudo ldconfig
```

### "Token not found" Hatası

1. Token'ın oluşturulduğundan emin olun:
```bash
softhsm2-util --show-slots
```

2. Token label'in doğru olduğundan emin olun

3. PIN'in doğru olduğundan emin olun

### "PIN incorrect" Hatası

- Varsayılan PIN: `1234`
- SO PIN: `0000`
- Token oluştururken farklı PIN kullandıysanız, `openToken()` fonksiyonunda belirtin

### Konfigürasyon Dosyası Bulunamıyor

1. Konfigürasyon dosyasının doğru yerde olduğundan emin olun
2. SOFTHSM2_CONF environment variable'ını ayarlayın
3. Dosya izinlerini kontrol edin

## Test

SoftHSM'nin doğru çalıştığını test etmek için:

```cpp
#include "softhsm.h"

int main() {
    // Initialize
    auto result = TravelExpense::SoftHSM::initialize();
    assert(result == TravelExpense::ErrorCode::SUCCESS);
    
    // Open token
    result = TravelExpense::SoftHSM::openToken("TravelExpense", "1234");
    assert(result == TravelExpense::ErrorCode::SUCCESS);
    
    // Generate random
    uint8_t random[32];
    result = TravelExpense::SoftHSM::generateRandom(random, 32);
    assert(result == TravelExpense::ErrorCode::SUCCESS);
    
    // Cleanup
    TravelExpense::SoftHSM::shutdown();
    return 0;
}
```

## Güvenlik Notları

⚠️ **ÖNEMLİ:** SoftHSM, **test ve geliştirme amaçlıdır**. Production ortamlarında gerçek HSM kullanılmalıdır.

- PIN'leri güvenli bir şekilde saklayın
- Token dosyalarını koruyun
- Production'da asla varsayılan PIN'leri kullanmayın

## Kaynaklar

- [SoftHSM2 GitHub](https://github.com/opendnssec/SoftHSMv2)
- [PKCS#11 Standard](https://docs.oasis-open.org/pkcs11/pkcs11-base/v2.40/pkcs11-base-v2.40.html)
- [SoftHSM2 Documentation](https://github.com/opendnssec/SoftHSMv2/wiki)

## Desteklenen Platformlar

- ✅ Windows (x64)
- ✅ Linux (Ubuntu, Debian, Fedora, CentOS)
- ✅ macOS

## Notlar

- SoftHSM2 konfigürasyon dosyası proje dizininde de bulunabilir
- Kütüphane otomatik olarak bulunamazsa, manuel yol belirtilmelidir
- Token'lar proje dizini içinde de saklanabilir (test için)

---

**Hazırlayan:** Binnur Altınışık  
**Tarih:** 2025  
**Versiyon:** 1.0


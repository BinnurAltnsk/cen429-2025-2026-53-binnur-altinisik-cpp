/**
 * @file rasp.cpp
 * @brief Seyahat Gideri Takibi - RASP (Runtime Application Self-Protection) Fonksiyonları
 *
 * Bu dosya, RASP tekniklerinin implementasyonunu içerir:
 * - Checksum doğrulama (Kod bloğu ve dosya integrity check)
 * - Anti-debug mekanizmaları (Debugger tespiti ve önleme)
 * - Tamper tespiti (Dosya değişiklik algılama ve müdahale tespiti)
 *
 * @author Binnur Altınışık
 * @date 2025
 */
#include <cstdint>
#include <cstring>
using std::uint8_t;
// CRITICAL: Windows SDK version MUST be defined before ANY includes or headers
#ifdef _WIN32
    // Force Windows SDK version for PROCESSENTRY32A support
    // Must be defined before ANY Windows headers or other includes
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Windows 7 or later (required for PROCESSENTRY32A)

// Ensure ANSI versions are used (not Unicode)
#undef UNICODE
#undef _UNICODE

// Prevent WIN32_LEAN_AND_MEAN from being defined (it excludes needed APIs)
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#endif

// Platform-specific includes - MUST be before other includes
#ifdef _WIN32
    // NOMINMAX ekle (WIN32_LEAN_AND_MEAN zaten undef edildi çünkü PROCESSENTRY32A için gerekli)
#define NOMINMAX
// Include windows.h first - BEFORE any other headers
#include <windows.h>

// Include tlhelp32.h for Process32FirstA and Process32NextA
// These functions are part of kernel32.dll and declared in tlhelp32.h
#include <tlhelp32.h>

// Fallback: If PROCESSENTRY32A is still not defined, define it manually
// This can happen if the Windows SDK version is too old or incorrectly configured
#ifndef PROCESSENTRY32A
typedef struct tagPROCESSENTRY32A {
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ProcessID;
    ULONG_PTR th32DefaultHeapID;
    DWORD th32ModuleID;
    DWORD cntThreads;
    DWORD th32ParentProcessID;
    LONG pcPriClassBase;
    DWORD dwFlags;
    CHAR szExeFile[MAX_PATH];
} PROCESSENTRY32A;
typedef PROCESSENTRY32A* PPROCESSENTRY32A;
typedef const PROCESSENTRY32A* LPPROCESSENTRY32A;
#endif

// Define function pointer types for Process32FirstA and Process32NextA
// We'll load these functions dynamically from kernel32.dll
typedef BOOL(WINAPI* PFN_Process32FirstA)(HANDLE hSnapshot, LPPROCESSENTRY32A lppe);
typedef BOOL(WINAPI* PFN_Process32NextA)(HANDLE hSnapshot, LPPROCESSENTRY32A lppe);

#include <psapi.h>
#include <process.h>

// Note: Process32FirstA and Process32NextA are in kernel32.lib
// Linking is done in CMakeLists.txt via target_link_libraries
// #pragma comment directives ensure linking even if CMake fails
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "advapi32.lib")
#include <intrin.h>
#else
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#endif

// Now include our headers (after Windows headers to ensure definitions are available)
#include "rasp.h"
#include "encryption.h"
#include "safe_string.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <cctype>

// Platform-specific string comparison
#ifdef _WIN32
#define strncasecmp _strnicmp
#define snprintf _snprintf
#endif

/**
 * @namespace TravelExpense
 * @brief Seyahat Gideri Takibi uygulaması ana namespace'i
 */
namespace TravelExpense {
    /**
     * @namespace RASP
     * @brief RASP (Runtime Application Self-Protection) teknikleri modülü implementasyonu
     */
    namespace RASP {

        // ============================================================================
        // GLOBAL STATE - Global Durum Değişkenleri
        // ============================================================================

        /**
         * @var g_raspActive
         * @brief RASP sisteminin aktif olup olmadığını belirten atomic boolean değişken
         *
         * Bu değişken, RASP sisteminin başlatılıp başlatılmadığını ve aktif olup olmadığını
         * belirtir. Thread-safe bir şekilde erişim sağlar (std::atomic).
         * true: RASP aktif, false: RASP pasif
         *
         * @note initializeRASP() ile true yapılır, shutdownRASP() ile false yapılır.
         * @see initializeRASP RASP sistemini başlatma
         * @see shutdownRASP RASP sistemini kapatma
         */
        static std::atomic<bool> g_raspActive(false);

        /**
         * @var g_periodicCheckActive
         * @brief Periyodik kontrol thread'inin aktif olup olmadığını belirten atomic boolean değişken
         *
         * Bu değişken, periyodik kontrol thread'inin çalışıp çalışmadığını belirtir.
         * Thread-safe bir şekilde erişim sağlar (std::atomic).
         * true: Periyodik kontrol aktif, false: Periyodik kontrol pasif
         *
         * @note startPeriodicCheck() ile true yapılır, stopPeriodicCheck() ile false yapılır.
         * @see startPeriodicCheck Periyodik kontrol başlatma
         * @see stopPeriodicCheck Periyodik kontrol durdurma
         */
        static std::atomic<bool> g_periodicCheckActive(false);

        /**
         * @var g_periodicCheckThread
         * @brief Periyodik kontrol thread'i işaretçisi
         *
         * Bu değişken, periyodik kontrol işlemlerini yürüten thread'in işaretçisini tutar.
         * Thread, startPeriodicCheck() ile oluşturulur ve stopPeriodicCheck() ile durdurulur.
         * nullptr: Thread oluşturulmamış veya durdurulmuş
         *
         * @note Thread, startPeriodicCheck() ile oluşturulur ve stopPeriodicCheck() ile durdurulur.
         * @see startPeriodicCheck Periyodik kontrol başlatma (thread oluşturur)
         * @see stopPeriodicCheck Periyodik kontrol durdurma (thread'i durdurur)
         */
        static std::thread* g_periodicCheckThread(nullptr);

        /**
         * @var g_checksumCallback
         * @brief Checksum doğrulama callback fonksiyonu işaretçisi
         *
         * Bu değişken, checksum doğrulama işlemleri için kullanıcı tanımlı callback
         * fonksiyonunun işaretçisini tutar. Eğer nullptr ise, varsayılan checksum
         * doğrulama fonksiyonu kullanılır.
         * nullptr: Varsayılan checksum doğrulama kullanılır
         *
         * @note setChecksumCallback() ile ayarlanır.
         * @see setChecksumCallback Checksum callback fonksiyonu ayarlama
         * @see ChecksumCallback Checksum callback fonksiyonu tipi (rasp.h)
         */
        static ChecksumCallback g_checksumCallback(nullptr);

        /**
         * @var g_checkIntervalMs
         * @brief Periyodik kontrol aralığı (milisaniye cinsinden)
         *
         * Bu değişken, periyodik kontrol işlemlerinin ne sıklıkla çalıştırılacağını
         * belirtir (milisaniye cinsinden). 0 ise, periyodik kontrol yapılmaz.
         * 0: Periyodik kontrol yapılmaz
         * > 0: Periyodik kontrol aralığı (milisaniye)
         *
         * @note setCheckInterval() ile ayarlanır.
         * @see setCheckInterval Periyodik kontrol aralığı ayarlama
         */
        static uint32_t g_checkIntervalMs(0);

        /**
         * @var g_expectedSelfChecksum
         * @brief Beklenen self-checksum değeri (SHA-256 hex string)
         *
         * Bu değişken, uygulamanın beklenen self-checksum değerini tutar.
         * SHA-256 hash değeri, 64 karakter hex string formatında saklanır (+ null terminator).
         * Bu değer, uygulamanın kod bütünlüğünü kontrol etmek için kullanılır.
         *
         * @note setExpectedSelfChecksum() ile ayarlanır.
         * @see setExpectedSelfChecksum Beklenen self-checksum değeri ayarlama
         * @see verifySelfChecksum Self-checksum doğrulama
         */
        static char g_expectedSelfChecksum[65] = { 0 };

        // ============================================================================
        // CHECKSUM DOĞRULAMA - Checksum Verification
        // ============================================================================

        bool calculateCodeChecksum(const void* data, size_t size, char* checksum) {
            if (!data || size == 0 || !checksum) {
                return false;
            }

            // SHA-256 hash hesapla
            return TravelExpense::Encryption::sha256Hash(data, size, checksum);
        }

        bool verifyCodeChecksum(const void* data, size_t size, const char* expectedChecksum) {
            if (!data || size == 0 || !expectedChecksum) {
                return false;
            }

            // Mevcut checksum'u hesapla
            char currentChecksum[65];
            if (!calculateCodeChecksum(data, size, currentChecksum)) {
                return false;
            }

            // Checksum'ları karşılaştır (case-insensitive)
            return (strncasecmp(currentChecksum, expectedChecksum, 64) == 0);
        }

        bool calculateFileChecksum(const char* filePath, char* checksum) {
            if (!filePath || !checksum) {
                return false;
            }

            // Dosyayı oku
            std::ifstream file(filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                return false;
            }

            // Dosya boyutunu al
            size_t fileSize = static_cast<size_t>(file.tellg());
            file.seekg(0, std::ios::beg);

            // Dosya içeriğini oku
            std::vector<uint8_t> buffer(fileSize);
            if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
                return false;
            }
            file.close();

            // SHA-256 hash hesapla
            return TravelExpense::Encryption::sha256Hash(buffer.data(), fileSize, checksum);
        }

        bool verifyFileChecksum(const char* filePath, const char* expectedChecksum) {
            if (!filePath || !expectedChecksum) {
                return false;
            }

            // Mevcut dosya checksum'unu hesapla
            char currentChecksum[65];
            if (!calculateFileChecksum(filePath, currentChecksum)) {
                return false;
            }

            // Checksum'ları karşılaştır (case-insensitive)
            return (strncasecmp(currentChecksum, expectedChecksum, 64) == 0);
        }

        bool calculateSelfChecksum(char* checksum) {
            if (!checksum) {
                return false;
            }

#ifdef _WIN32
            // Windows: GetModuleFileName ile executable path'i al
            char exePath[MAX_PATH];
            if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) == 0) {
                return false;
            }
            return calculateFileChecksum(exePath, checksum);
#else
            // Linux: /proc/self/exe symbolic link'ini oku
            char exePath[1024];
            ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
            if (len == -1) {
                return false;
            }
            exePath[len] = '\0';
            return calculateFileChecksum(exePath, checksum);
#endif
        }

        bool verifySelfChecksum(const char* expectedChecksum) {
            if (!expectedChecksum) {
                return false;
            }

            // Mevcut self checksum'unu hesapla
            char currentChecksum[65];
            if (!calculateSelfChecksum(currentChecksum)) {
                return false;
            }

            // Checksum'ları karşılaştır (case-insensitive)
            return (strncasecmp(currentChecksum, expectedChecksum, 64) == 0);
        }

        // Periyodik checksum kontrolü thread fonksiyonu
        static void periodicChecksumThread() {
            while (g_periodicCheckActive.load()) {
                if (g_checksumCallback) {
                    if (!g_checksumCallback()) {
                        // Checksum doğrulama başarısız
                        handleTamperDetected(true); // Uygulamayı sonlandır
                        break;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(g_checkIntervalMs));
            }
        }

        bool startPeriodicChecksum(ChecksumCallback callback, uint32_t intervalMs) {
            if (!callback || intervalMs == 0) {
                return false;
            }

            // Eğer zaten çalışıyorsa, durdur
            if (g_periodicCheckActive.load()) {
                stopPeriodicChecksum();
            }

            g_checksumCallback = callback;
            g_checkIntervalMs = intervalMs;
            g_periodicCheckActive = true;

            // Thread başlat
            g_periodicCheckThread = new std::thread(periodicChecksumThread);

            return true;
        }

        bool stopPeriodicChecksum() {
            if (!g_periodicCheckActive.load()) {
                return false;
            }

            // Thread'i durdur
            g_periodicCheckActive = false;
            if (g_periodicCheckThread) {
                if (g_periodicCheckThread->joinable()) {
                    g_periodicCheckThread->join();
                }
                delete g_periodicCheckThread;
                g_periodicCheckThread = nullptr;
            }

            g_checksumCallback = nullptr;
            g_checkIntervalMs = 0;

            return true;
        }

        // ============================================================================
        // ANTI-DEBUG - Debugger Tespiti ve Önleme
        // ============================================================================

        bool isDebuggerPresent() {
#ifdef _WIN32
            // Windows: IsDebuggerPresent() kullan
            return ::IsDebuggerPresent() != FALSE;
#else
            // Linux: ptrace kontrolü
            return !checkPtrace();
#endif
        }

        bool isRemoteDebuggerPresent() {
#ifdef _WIN32
            // Windows: CheckRemoteDebuggerPresent() kullan
            BOOL isDebuggerPresent = FALSE;
            if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent)) {
                return isDebuggerPresent != FALSE;
            }
            return false;
#else
            // Linux'ta remote debugger kontrolü için parent process kontrolü
            // Eğer parent process bir debugger ise, bu tespit edilebilir
            // Basit implementasyon: her zaman false döndür
            return false;
#endif
        }

        bool checkPtrace() {
#ifdef _WIN32
            // Windows'ta ptrace yok
            return true; // Debugger yok say
#else
            // Linux: ptrace kendini attach etmeyi dene
            // Eğer başarılı olursa, başka bir debugger yok demektir
            pid_t pid = fork();
            if (pid == -1) {
                return false; // Fork başarısız, debugger olabilir
            }

            if (pid == 0) {
                // Child process: ptrace kendini attach et
                if (ptrace(PTRACE_ATTACH, getppid(), nullptr, nullptr) == 0) {
                    // Başarılı: debugger yok
                    ptrace(PTRACE_DETACH, getppid(), nullptr, nullptr);
                    exit(0);
                }
                else {
                    // Başarısız: debugger var
                    exit(1);
                }
            }
            else {
                // Parent process: child'ı bekle
                int status;
                waitpid(pid, &status, 0);
                return WIFEXITED(status) && WEXITSTATUS(status) == 0;
            }
#endif
        }

        bool detectDebuggerProcesses() {
#ifdef _WIN32
            // Windows: Bilinen debugger process isimlerini kontrol et
            const char* debuggerProcesses[] = {
                "windbg.exe",
                "ollydbg.exe",
                "x64dbg.exe",
                "x32dbg.exe",
                "ida.exe",
                "ida64.exe",
                "idaq.exe",
                "idaq64.exe",
                "ghidra.exe",
                "cheatengine-x86_64.exe",
                "cheatengine-i386.exe",
                "devenv.exe", // Visual Studio (debug modda)
                nullptr
            };

            // Load Process32FirstA and Process32NextA from kernel32.dll dynamically
            // This avoids linker issues with these functions
            HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
            if (!kernel32) {
                kernel32 = LoadLibraryA("kernel32.dll");
                if (!kernel32) {
                    return false;
                }
            }

            PFN_Process32FirstA pfnProcess32FirstA = (PFN_Process32FirstA)GetProcAddress(kernel32, "Process32FirstA");
            PFN_Process32NextA pfnProcess32NextA = (PFN_Process32NextA)GetProcAddress(kernel32, "Process32NextA");

            if (!pfnProcess32FirstA || !pfnProcess32NextA) {
                // Functions not found (shouldn't happen on Windows, but handle gracefully)
                if (kernel32 != GetModuleHandleA("kernel32.dll")) {
                    FreeLibrary(kernel32);
                }
                return false;
            }

            // Process snapshot al
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snapshot == INVALID_HANDLE_VALUE) {
                if (kernel32 != GetModuleHandleA("kernel32.dll")) {
                    FreeLibrary(kernel32);
                }
                return false;
            }

            PROCESSENTRY32A pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32A);

            if (pfnProcess32FirstA(snapshot, &pe32)) {
                do {
                    // Process ismini lowercase'e çevir
                    char processName[MAX_PATH];
                    size_t exeNameLen = strlen(pe32.szExeFile);
                    size_t copyLen = (exeNameLen < MAX_PATH - 1) ? exeNameLen : (MAX_PATH - 1);
                    memcpy(processName, pe32.szExeFile, copyLen);
                    processName[copyLen] = '\0';
                    for (int i = 0; processName[i] != '\0'; ++i) {
                        processName[i] = static_cast<char>(std::tolower(processName[i]));
                    }

                    // Debugger process isimlerini kontrol et
                    for (int i = 0; debuggerProcesses[i] != nullptr; ++i) {
                        if (strstr(processName, debuggerProcesses[i]) != nullptr) {
                            CloseHandle(snapshot);
                            if (kernel32 != GetModuleHandleA("kernel32.dll")) {
                                FreeLibrary(kernel32);
                            }
                            return true; // Debugger process tespit edildi
                        }
                    }
                } while (pfnProcess32NextA(snapshot, &pe32));
            }

            CloseHandle(snapshot);
            if (kernel32 != GetModuleHandleA("kernel32.dll")) {
                FreeLibrary(kernel32);
            }
            return false;
#else
            // Linux: Bilinen debugger process isimlerini kontrol et
            const char* debuggerProcesses[] = {
                "gdb",
                "lldb",
                "strace",
                "ltrace",
                "radare2",
                "ida",
                "ida64",
                "ghidra",
                "wireshark",
                "tcpdump",
                nullptr
            };

            // /proc dizinini oku
            DIR* proc = opendir("/proc");
            if (!proc) {
                return false;
            }

            struct dirent* entry;
            while ((entry = readdir(proc)) != nullptr) {
                // Process ID kontrolü
                if (entry->d_type != DT_DIR) {
                    continue;
                }

                char* endptr;
                pid_t pid = strtol(entry->d_name, &endptr, 10);
                if (*endptr != '\0') {
                    continue; // Process ID değil
                }

                // Process komut satırını oku
                char cmdlinePath[256];
                snprintf(cmdlinePath, sizeof(cmdlinePath), "/proc/%d/cmdline", pid);

                FILE* cmdline = fopen(cmdlinePath, "r");
                if (cmdline) {
                    char cmdlineBuf[256];
                    if (fgets(cmdlineBuf, sizeof(cmdlineBuf), cmdline)) {
                        // Process ismini lowercase'e çevir
                        for (int i = 0; cmdlineBuf[i] != '\0'; ++i) {
                            cmdlineBuf[i] = static_cast<char>(std::tolower(cmdlineBuf[i]));
                        }

                        // Debugger process isimlerini kontrol et
                        for (int i = 0; debuggerProcesses[i] != nullptr; ++i) {
                            if (strstr(cmdlineBuf, debuggerProcesses[i]) != nullptr) {
                                fclose(cmdline);
                                closedir(proc);
                                return true; // Debugger process tespit edildi
                            }
                        }
                    }
                    fclose(cmdline);
                }
            }

            closedir(proc);
            return false;
#endif
        }

        bool performAntiDebugCheck() {
            // Tüm anti-debug kontrollerini sırayla çalıştır
            if (isDebuggerPresent()) {
                return true;
            }

#ifdef _WIN32
            if (isRemoteDebuggerPresent()) {
                return true;
            }
#endif

            if (!checkPtrace()) {
                return true;
            }

            if (detectDebuggerProcesses()) {
                return true;
            }

            return false; // Debugger tespit edilmedi
        }

        void handleDebuggerDetected(bool terminate) {
            // Debugger tespit edildiğinde tepki ver
            if (terminate) {
                // Uygulamayı sonlandır
#ifdef _WIN32
                ExitProcess(1);
#else
                exit(1);
#endif
            }
            // Aksi halde sadece log (implementasyon detayına göre eklenebilir)
        }

        // ============================================================================
        // TAMPER DETECTION - Müdahale Tespiti
        // ============================================================================

        bool detectFileModification(const char* filePath, int64_t expectedModTime) {
            if (!filePath) {
                return false;
            }

#ifdef _WIN32
            // Windows: Dosya zaman bilgisini al
            HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile == INVALID_HANDLE_VALUE) {
                return false;
            }

            FILETIME ftCreate, ftAccess, ftWrite;
            if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
                CloseHandle(hFile);
                return false;
            }

            CloseHandle(hFile);

            // FILETIME'ı Unix timestamp'e çevir
            ULARGE_INTEGER uli;
            uli.LowPart = ftWrite.dwLowDateTime;
            uli.HighPart = ftWrite.dwHighDateTime;
            int64_t modTime = (uli.QuadPart / 10000000ULL) - 11644473600ULL;

            return modTime != expectedModTime;
#else
            // Linux: stat() ile dosya zaman bilgisini al
            struct stat st;
            if (stat(filePath, &st) != 0) {
                return false;
            }

            return static_cast<int64_t>(st.st_mtime) != expectedModTime;
#endif
        }

        bool detectFileSizeChange(const char* filePath, int64_t expectedSize) {
            if (!filePath) {
                return false;
            }

#ifdef _WIN32
            // Windows: Dosya boyutunu al
            HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile == INVALID_HANDLE_VALUE) {
                return false;
            }

            LARGE_INTEGER fileSize;
            if (!GetFileSizeEx(hFile, &fileSize)) {
                CloseHandle(hFile);
                return false;
            }

            CloseHandle(hFile);
            return fileSize.QuadPart != expectedSize;
#else
            // Linux: stat() ile dosya boyutunu al
            struct stat st;
            if (stat(filePath, &st) != 0) {
                return false;
            }

            return static_cast<int64_t>(st.st_size) != expectedSize;
#endif
        }

        bool checkBinaryIntegrity(const char* expectedChecksum) {
            if (!expectedChecksum) {
                return false;
            }

            // Kendi binary'sinin checksum'unu kontrol et
            return verifySelfChecksum(expectedChecksum);
        }

        bool performTamperCheck(const char* expectedSelfChecksum) {
            if (!expectedSelfChecksum) {
                return false;
            }

            // Binary integrity kontrolü
            if (!checkBinaryIntegrity(expectedSelfChecksum)) {
                return true; // Müdahale tespit edildi
            }

            // Self checksum kontrolü
            if (!verifySelfChecksum(expectedSelfChecksum)) {
                return true; // Müdahale tespit edildi
            }

            return false; // Müdahale tespit edilmedi
        }

        void handleTamperDetected(bool terminate) {
            // Müdahale tespit edildiğinde tepki ver
            if (terminate) {
                // Uygulamayı sonlandır
#ifdef _WIN32
                ExitProcess(2);
#else
                exit(2);
#endif
            }
            // Aksi halde sadece log (implementasyon detayına göre eklenebilir)
        }

        // ============================================================================
        // HOOK DETECTION - Hook Saldırı Tespiti
        // ============================================================================

        // Global state for hook detection
        static std::vector<void*> g_criticalFunctions;

        bool detectHookAttack() {
            // Kritik sistem fonksiyonlarını kontrol et
            return checkCriticalFunctionHooks();
        }
        bool detectFunctionHook(const void* func, const char* functionName) {
            if (!func || !functionName) {
                return false;
            }

            const uint8_t* addr = static_cast<const uint8_t*>(func);

            // İlk 5 baytı oku
            uint8_t prolog[5] = { 0 };
            std::memcpy(prolog, addr, sizeof(prolog));

            // Ortak hızlı kontroller
            // 0xE9: JMP rel32, 0xE8: CALL rel32, 0xFF 0x25: JMP [addr]
            if (prolog[0] == 0xE9 || prolog[0] == 0xE8 || (prolog[0] == 0xFF && prolog[1] == 0x25)) {
                return true;
            }

#ifdef _WIN32
            // Bazı hook’larda PUSH imm (0x68) veya FF ... varyasyonları görülebilir
            if (prolog[0] == 0xFF || prolog[0] == 0x68) {
                return true;
            }
#else
            // Linux tarafında da 0xE9/0xFF başlangıcı şüpheli sayılır
            if (prolog[0] == 0xE9 || prolog[0] == 0xFF) {
                return true;
            }
#endif

            return false; // Hook tespit edilmedi
        }

        bool checkCriticalFunctionHooks() {
            // Kritik fonksiyonların hook kontrolü
            // Bu örnekte birkaç kritik sistem fonksiyonunu kontrol ediyoruz

            bool hookDetected = false;

#ifdef _WIN32
            // Windows: Kritik API fonksiyonlarını kontrol et
            HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
            if (hKernel32) {
                // CreateFileA, ReadFile, WriteFile gibi kritik fonksiyonlar
                FARPROC createFileA = GetProcAddress(hKernel32, "CreateFileA");
                if (createFileA && detectFunctionHook(
                    reinterpret_cast<const void*>(reinterpret_cast<uintptr_t>(createFileA)),
                    "CreateFileA")) {
                    hookDetected = true;
                }

                FARPROC readFile = GetProcAddress(hKernel32, "ReadFile");
                if (readFile && detectFunctionHook(
                    reinterpret_cast<const void*>(reinterpret_cast<uintptr_t>(readFile)),
                    "ReadFile")) {
                    hookDetected = true;
                }

                FARPROC writeFile = GetProcAddress(hKernel32, "WriteFile");
                if (writeFile && detectFunctionHook(
                    reinterpret_cast<const void*>(reinterpret_cast<uintptr_t>(writeFile)),
                    "WriteFile")) {
                    hookDetected = true;
                }
            }
#else
            // Linux: Kritik libc fonksiyonlarını kontrol et
            // Not: Bu daha karmaşık ve dlopen/dladdr gerektirir
            // Basit bir kontrol için fonksiyon pointer'larını kontrol edebiliriz
            // Gerçek implementasyonda dlopen ile libc'yi açıp dlsym ile fonksiyon adreslerini alabiliriz
#endif

            // Kayıtlı kritik fonksiyonları kontrol et
            for (void* funcPtr : g_criticalFunctions) {
                if (detectFunctionHook(funcPtr, "CriticalFunction")) {
                    hookDetected = true;
                    break;
                }
            }

            return hookDetected;
        }

        void handleHookDetected(bool terminate) {
            // Hook tespit edildiğinde tepki ver
            if (terminate) {
                // Uygulamayı sonlandır
#ifdef _WIN32
                ExitProcess(3);
#else
                exit(3);
#endif
            }
            // Aksi halde sadece log (implementasyon detayına göre eklenebilir)
        }

        // ============================================================================
        // CONTROL FLOW COUNTER - Kontrol Akışı Sayacı Kontrolü
        // ============================================================================

        // Global state for control flow counter
        static std::atomic<uint32_t> g_controlFlowCounter(0);
        static std::atomic<bool> g_controlFlowInitialized(false);
        static uint32_t g_expectedControlFlowValue(0);

        bool initializeControlFlowCounter(uint32_t expectedValue) {
            g_controlFlowCounter.store(0);
            g_expectedControlFlowValue = expectedValue;
            g_controlFlowInitialized = true;
            return true;
        }

        bool incrementControlFlowCounter() {
            if (!g_controlFlowInitialized.load()) {
                return false;
            }

            // Thread-safe artırma
            g_controlFlowCounter.fetch_add(1);
            return true;
        }

        bool verifyControlFlowCounter(uint32_t expectedValue) {
            if (!g_controlFlowInitialized.load()) {
                return false;
            }

            uint32_t currentValue = g_controlFlowCounter.load();

            // Mevcut değerin beklenen değerle uyumlu olup olmadığını kontrol et
            // Genellikle mevcut değer beklenen değerden küçük veya eşit olmalı
            // (çünkü sayaç artıyor olabilir)
            // Ancak tam eşitlik kontrolü daha güvenli
            if (currentValue != expectedValue) {
                // Sayaç değeri beklenen değerle eşleşmiyor - müdahale tespit edildi
                return false;
            }

            return true;
        }

        uint32_t getControlFlowCounter() {
            return g_controlFlowCounter.load();
        }

        bool resetControlFlowCounter() {
            if (!g_controlFlowInitialized.load()) {
                return false;
            }

            g_controlFlowCounter.store(0);
            return true;
        }

        bool performControlFlowCheck(uint32_t expectedValue) {
            if (!g_controlFlowInitialized.load()) {
                return false;
            }

            // Kontrol akışı sayacını doğrula
            if (!verifyControlFlowCounter(expectedValue)) {
                // Kontrol akışı bütünlüğü bozulmuş - müdahale tespit edildi
                return false;
            }

            return true;
        }

        // ============================================================================
        // UNSAFE DEVICE DETECTION - Güvensiz Cihaz Tespiti
        // ============================================================================

        bool detectEmulator() {
#ifdef _WIN32
            // Windows: CPU ve sistem özelliklerini kontrol et
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);

            // CPU sayısı kontrolü (emulator'ler genellikle düşük CPU sayısına sahiptir)
            if (sysInfo.dwNumberOfProcessors < 2) {
                return true; // Şüpheli: Çok az CPU
            }

            // Bellek kontrolü (emulator'ler genellikle düşük bellek ile çalışır)
            MEMORYSTATUSEX memStatus;
            memStatus.dwLength = sizeof(MEMORYSTATUSEX);
            if (GlobalMemoryStatusEx(&memStatus)) {
                // 2 GB'dan az fiziksel bellek şüpheli olabilir
                if (memStatus.ullTotalPhys < 2ULL * 1024 * 1024 * 1024) {
                    return true; // Şüpheli: Çok az bellek
                }
            }

            // Registry kontrolü: Emulator imzaları
            HKEY hKey;
            const char* emulatorKeys[] = {
                "SOFTWARE\\Microsoft\\Virtual Machine\\Guest\\Parameters",
                "SYSTEM\\CurrentControlSet\\Services\\VBoxGuest",
                "SYSTEM\\CurrentControlSet\\Services\\VBoxSF",
                "SYSTEM\\CurrentControlSet\\Services\\VBoxMouse",
                "SYSTEM\\CurrentControlSet\\Services\\VBoxVideo",
                "SYSTEM\\CurrentControlSet\\Services\\VBoxService",
            };

            for (size_t i = 0; i < sizeof(emulatorKeys) / sizeof(emulatorKeys[0]); ++i) {
                if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, emulatorKeys[i], 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                    RegCloseKey(hKey);
                    return true; // Emulator tespit edildi
                }
            }

            // CPUID kontrolü: Hypervisor bit kontrolü
            int cpuInfo[4] = { 0 };
            __cpuid(cpuInfo, 1);
            // ECX bit 31: Hypervisor present bit
            if ((cpuInfo[2] & (1 << 31)) != 0) {
                // Hypervisor var, ancak bu emulator olmayabilir (legitimate virtualization)
                // Daha detaylı kontrol gerekebilir
            }

            return false;
#else
            // Linux: /proc/cpuinfo ve sistem özelliklerini kontrol et
            FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
            if (cpuinfo) {
                char line[256];
                int cpuCount = 0;
                bool hasHypervisor = false;

                while (fgets(line, sizeof(line), cpuinfo)) {
                    // CPU sayısını say
                    if (strncmp(line, "processor", 9) == 0) {
                        cpuCount++;
                    }

                    // Hypervisor imzalarını kontrol et
                    if (strstr(line, "hypervisor") != nullptr ||
                        strstr(line, "QEMU") != nullptr ||
                        strstr(line, "KVM") != nullptr ||
                        strstr(line, "VMware") != nullptr ||
                        strstr(line, "VirtualBox") != nullptr ||
                        strstr(line, "Xen") != nullptr) {
                        hasHypervisor = true;
                    }
                }
                fclose(cpuinfo);

                // Çok az CPU şüpheli
                if (cpuCount < 2) {
                    return true;
                }

                // Hypervisor varsa şüpheli (ancak legitimate virtualization olabilir)
                if (hasHypervisor) {
                    // Daha detaylı kontrol gerekebilir
                    // Bu basit implementasyonda true döndürüyoruz
                    return true;
                }
            }

            // /sys/class/dmi/id/product_name kontrolü
            FILE* productName = fopen("/sys/class/dmi/id/product_name", "r");
            if (productName) {
                char product[256];
                if (fgets(product, sizeof(product), productName)) {
                    // Emulator imzalarını kontrol et
                    if (strstr(product, "QEMU") != nullptr ||
                        strstr(product, "KVM") != nullptr ||
                        strstr(product, "VMware") != nullptr ||
                        strstr(product, "VirtualBox") != nullptr ||
                        strstr(product, "Xen") != nullptr) {
                        fclose(productName);
                        return true; // Emulator tespit edildi
                    }
                }
                fclose(productName);
            }

            return false;
#endif
        }

        bool detectRootJailbreak() {
#ifdef _WIN32
            // Windows: Administrator privileges kontrolü
            BOOL isAdmin = FALSE;
            PSID adminGroup = nullptr;
            SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

            if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
                CheckTokenMembership(nullptr, adminGroup, &isAdmin);
                FreeSid(adminGroup);
            }

            // Administrator privileges varsa root tespit edildi
            return isAdmin == TRUE;
#else
            // Linux: Root user kontrolü (UID 0)
            return getuid() == 0 || geteuid() == 0;
#endif
        }

        bool detectUnsafeDevice() {
            // Emulator tespiti
            if (detectEmulator()) {
                return true;
            }

            // Root/Jailbreak tespiti
            if (detectRootJailbreak()) {
                return true;
            }

            return false; // Cihaz güvenli
        }

        // ============================================================================
        // RASP INITIALIZATION - RASP Başlatma ve Yönetimi
        // ============================================================================

        bool initializeRASP(const char* expectedSelfChecksum, bool enablePeriodicCheck, uint32_t checkIntervalMs) {
            if (!expectedSelfChecksum) {
                return false;
            }

            // Beklenen self checksum'u sakla
            TravelExpense::SafeString::safeCopy(g_expectedSelfChecksum, sizeof(g_expectedSelfChecksum), expectedSelfChecksum);

            // Self checksum callback fonksiyonu
            auto selfChecksumCallback = []() -> bool {
                return verifySelfChecksum(g_expectedSelfChecksum);
                };

            // Periyodik checksum kontrolünü başlat (eğer aktifse)
            if (enablePeriodicCheck && checkIntervalMs > 0) {
                if (!startPeriodicChecksum(selfChecksumCallback, checkIntervalMs)) {
                    return false;
                }
            }

            // Anti-debug kontrolü yap
            if (performAntiDebugCheck()) {
                handleDebuggerDetected(true); // Debugger tespit edildi, uygulamayı sonlandır
                return false;
            }

            // Tamper detection kontrolü yap
            if (performTamperCheck(expectedSelfChecksum)) {
                handleTamperDetected(true); // Müdahale tespit edildi, uygulamayı sonlandır
                return false;
            }

            g_raspActive = true;
            return true;
        }

        bool shutdownRASP() {
            // Periyodik checksum kontrolünü durdur
            stopPeriodicChecksum();

            g_raspActive = false;
            return true;
        }

        bool isRASPActive() {
            return g_raspActive.load();
        }

        bool getRASPStatus(bool* debuggerDetected, bool* tamperDetected, bool* checksumValid) {
            if (!debuggerDetected || !tamperDetected || !checksumValid) {
                return false;
            }

            // Anti-debug kontrolü
            *debuggerDetected = performAntiDebugCheck();

            // Tamper detection kontrolü
            *tamperDetected = performTamperCheck(g_expectedSelfChecksum);

            // Checksum doğrulama
            *checksumValid = verifySelfChecksum(g_expectedSelfChecksum);

            return true;
        }

    } // namespace RASP
} // namespace TravelExpense


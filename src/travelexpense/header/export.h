/**
 * @file export.h
 * @brief Seyahat Gideri Takibi - DLL Export/Import Macros
 * 
 * Bu dosya, Windows ve Linux platformlarında DLL/SO export/import
 * için gerekli macro tanımlamalarını içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef EXPORT_H
#define EXPORT_H

#ifdef _WIN32
    #ifdef TRAVELEXPENSE_LIB_EXPORTS
        #define TRAVELEXPENSE_API __declspec(dllexport)
    #else
        #define TRAVELEXPENSE_API __declspec(dllimport)
    #endif
#else
    // Linux/Unix için visibility attribute
    #ifdef TRAVELEXPENSE_LIB_EXPORTS
        #define TRAVELEXPENSE_API __attribute__((visibility("default")))
    #else
        #define TRAVELEXPENSE_API
    #endif
#endif

// Bazı fonksiyonlar için export gerekmiyorsa
#define TRAVELEXPENSE_INTERNAL

#endif // EXPORT_H


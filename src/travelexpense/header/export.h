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
        /** @brief Windows DLL export macro - DLL build edildiğinde kullanılır */
        #define TRAVELEXPENSE_API __declspec(dllexport)
    #else
        /** @brief Windows DLL import macro - DLL kullanıldığında kullanılır */
        #define TRAVELEXPENSE_API __declspec(dllimport)
    #endif
#else
    // Linux/Unix için visibility attribute
    #ifdef TRAVELEXPENSE_LIB_EXPORTS
        /** @brief Linux/Unix shared library export - SO build edildiğinde kullanılır */
        #define TRAVELEXPENSE_API __attribute__((visibility("default")))
    #else
        /** @brief Linux/Unix shared library - SO kullanıldığında kullanılır */
        #define TRAVELEXPENSE_API
    #endif
#endif

/**
 * @brief Internal fonksiyonlar için export gerekmiyorsa kullanılır
 * 
 * Bu macro, sadece library içinde kullanılan fonksiyonlar için kullanılır.
 * Dışarıdan erişilemez ve export edilmez.
 */
#define TRAVELEXPENSE_INTERNAL

#endif // EXPORT_H


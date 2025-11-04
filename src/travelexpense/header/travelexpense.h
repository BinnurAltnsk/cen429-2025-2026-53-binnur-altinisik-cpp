/**
 * @file travelexpense.h
 * @brief Seyahat Gideri Takibi - Ana Kütüphane Header Dosyası
 * 
 * Bu dosya, seyahat gideri takibi uygulamasının tüm modüllerini içeren ana header dosyasıdır.
 * Bu dosyayı include etmek tüm modüllere erişim sağlar.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef TRAVELEXPENSE_H
#define TRAVELEXPENSE_H

// Ortak veri tipleri ve modeller
#include "commonTypes.h"

// Yardımcı fonksiyonlar
#include "helpers.h"

// Dosya I/O yardımcı fonksiyonlar
#include "fileIO.h"

// Veritabanı modülü
#include "database.h"

// Güvenlik modülleri
#include "security.h"
#include "encryption.h"
#include "softhsm.h"
#include "codehardening.h"
#include "rasp.h"
#include "sessionManager.h"
#include "fingerprinting.h"
#include "tls.h"

// Migration modülü (Binary → SQLite)
#include "migration.h"

// Modüller
#include "userAuth.h"
#include "tripManager.h"
#include "expenseManager.h"
#include "budgetManager.h"
#include "reportGenerator.h"

// Varlık Yönetimi
#include "assetManager.h"

#endif // TRAVELEXPENSE_H

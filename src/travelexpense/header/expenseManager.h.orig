/**
 * @file expenseManager.h
 * @brief Seyahat Gideri Takibi - Gider Yönetimi Modülü
 *
 * Bu dosya, gider yönetimi işlemlerinin fonksiyon bildirimlerini içerir.
 *
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef EXPENSE_MANAGER_H
#define EXPENSE_MANAGER_H

#include "commonTypes.h"
#include "export.h"
#include <vector>

namespace TravelExpense { // LCOV_EXCL_LINE

/**
 * @namespace ExpenseManager
 * @brief Gider yönetimi modülü
 *
 * Bu modül, seyahat giderlerinin kaydedilmesi, güncellenmesi, silinmesi
 * ve listelenmesi işlemlerini yönetir. Kategori bazlı gider takibi
 * ve bütçe kontrolü sağlar.
 */
namespace ExpenseManager {
/**
 * @brief Yeni gider kaydet
 *
 * Belirtilen gider bilgilerini veritabanına kaydeder ve benzersiz bir expenseId
 * oluşturur. Gider, seyahat ID'si, kategori, miktar ve tarih bilgilerini içerir.
 *
 * @note Bu fonksiyon, bütçe kontrolü yapmaz. Bütçe kontrolü için
 * BudgetManager::checkBudgetLimit() fonksiyonu ayrıca çağrılmalıdır.
 *
 * @param expense Gider bilgileri (const referans, Expense struct'ı)
 *                 İçerik: tripId, category, amount, date, description vb.
 *                 Tüm alanlar geçerli olmalı (tripId > 0, amount >= 0 vb.).
 * @param expenseId Oluşturulan giderin benzersiz ID'si (çıktı parametresi)
 * @return ErrorCode Başarı durumu (Success, InvalidInput, FileNotFound vb.)
 */
TRAVELEXPENSE_API ErrorCode logExpense(const Expense &expense, int32_t &expenseId);

/**
 * @brief Seyahate ait giderleri listele
 *
 * Belirtilen seyahat ID'sine ait tüm giderleri veritabanından alır ve
 * expenses vektörüne doldurur. Giderler tarih sırasına göre sıralanabilir.
 *
 * @note Bu fonksiyon, expenses vektörünü temizler ve yeni giderlerle doldurur.
 * Eğer seyahat için gider bulunamazsa, expenses vektörü boş kalır.
 *
 * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
 * @param expenses Gider listesi (çıktı parametresi, std::vector<Expense>)
 *                 Fonksiyon çağrıldığında temizlenir ve yeni giderlerle doldurulur.
 * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
 */
TRAVELEXPENSE_API ErrorCode getExpenses(int32_t tripId, std::vector<Expense> &expenses);

/**
 * @brief Gider bilgisini güncelle
 *
 * Belirtilen expenseId'ye sahip gideri, yeni gider bilgileriyle günceller.
 * Kategori, miktar, tarih ve açıklama gibi alanlar değiştirilebilir.
 *
 * @note Bu fonksiyon, mevcut gider kaydını günceller. Eğer expenseId
 * geçersizse veya gider bulunamazsa, FileNotFound hatası döner.
 *
 * @param expenseId Güncellenecek giderin benzersiz ID'si (geçerli bir expenseId olmalı, 0'dan büyük)
 * @param expense Güncellenmiş gider bilgileri (const referans, Expense struct'ı)
 *                İçerik: tripId, category, amount, date, description vb.
 *                Tüm alanlar geçerli olmalı.
 * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
 */
TRAVELEXPENSE_API ErrorCode updateExpense(int32_t expenseId, const Expense &expense);

/**
 * @brief Gideri sil
 *
 * Belirtilen expenseId'ye sahip gideri veritabanından kalıcı olarak siler.
 *
 * @note Bu fonksiyon, gideri kalıcı olarak siler. Silinen gider geri getirilemez.
 * Eğer expenseId geçersizse veya gider bulunamazsa, FileNotFound hatası döner.
 *
 * @param expenseId Silinecek giderin benzersiz ID'si (geçerli bir expenseId olmalı, 0'dan büyük)
 * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
 */
TRAVELEXPENSE_API ErrorCode deleteExpense(int32_t expenseId);

/**
 * @brief Gider detayını al
 *
 * Belirtilen expenseId'ye sahip giderin detaylı bilgilerini veritabanından alır
 * ve expense parametresine doldurur.
 *
 * @note Bu fonksiyon, tek bir giderin detaylarını getirir.
 * Eğer gider bulunamazsa, FileNotFound hatası döner.
 *
 * @param expenseId Gider ID (geçerli bir expenseId olmalı, 0'dan büyük)
 * @param expense Gider bilgisi (çıktı parametresi, Expense struct'ı)
 *                İçerik: expenseId, tripId, category, amount, date, description vb.
 * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
 */
TRAVELEXPENSE_API ErrorCode getExpense(int32_t expenseId, Expense &expense);
}

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // EXPENSE_MANAGER_H

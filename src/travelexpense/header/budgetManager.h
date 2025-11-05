/**
 * @file budgetManager.h
 * @brief Seyahat Gideri Takibi - Bütçe Yönetimi Modülü
 * 
 * Bu dosya, bütçe yönetimi işlemlerinin fonksiyon bildirimlerini içerir.
 * 
 * @author Binnur Altınışık
 * @date 2025
 */

#ifndef BUDGET_MANAGER_H
#define BUDGET_MANAGER_H

#include "commonTypes.h"
#include "export.h"

namespace TravelExpense { // LCOV_EXCL_LINE

    /**
     * @namespace BudgetManager
     * @brief Bütçe yönetimi modülü
     * 
     * Bu modül, seyahat bütçelerinin oluşturulması, güncellenmesi ve
     * kontrol edilmesi işlemlerini yönetir. Kategori bazlı bütçe takibi
     * ve bütçe limiti kontrolü sağlar.
     */
    namespace BudgetManager {
        /**
         * @brief Seyahat için bütçe belirle
         * 
         * Belirtilen seyahat için toplam bütçe ve kategori bazlı bütçeleri ayarlar.
         * Bütçe, veritabanına kaydedilir ve benzersiz bir budgetId ile döndürülür.
         * 
         * @note Bu fonksiyon, seyahat için yeni bir bütçe kaydı oluşturur.
         * Eğer seyahat için zaten bir bütçe varsa, yeni bütçe eski bütçeyi
         * geçersiz kılmaz, ayrı bir kayıt olarak saklanır.
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param totalBudget Toplam bütçe miktarı (TL cinsinden, 0'dan büyük olmalı)
         * @param categoryBudgets Kategori bazlı bütçeler dizisi (4 eleman: Accommodation, Transportation, Food, Other)
         *                        nullptr ise InvalidInput döner. Her kategori bütçesi 0 veya pozitif olmalı.
         *                        Toplamı totalBudget'ı aşmamalı (opsiyonel kontrol).
         * @param budgetId Oluşturulan bütçenin benzersiz ID'si (çıktı parametresi)
         * @return ErrorCode Başarı durumu (Success, InvalidInput, FileNotFound vb.)
         */
        TRAVELEXPENSE_API ErrorCode setBudget(int32_t tripId, double totalBudget, 
                           const double* categoryBudgets, int32_t& budgetId);

        /**
         * @brief Seyahat bütçesini görüntüle
         * 
         * Belirtilen seyahat ID'sine ait bütçe bilgilerini veritabanından alır
         * ve budget parametresine doldurur. Bütçe bilgisi, toplam bütçe ve
         * kategori bazlı bütçeleri içerir.
         * 
         * @note Bu fonksiyon, seyahat için kayıtlı en son bütçeyi döndürür.
         * Eğer seyahat için bütçe bulunamazsa, FileNotFound hatası döner.
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param budget Bütçe bilgisi (çıktı parametresi, Budget struct'ı doldurulur)
         *               İçerik: totalBudget, categoryBudgets[4], budgetId, tripId vb.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode getBudget(int32_t tripId, Budget& budget);

        /**
         * @brief Bütçeyi güncelle
         * 
         * Belirtilen budgetId'ye sahip bütçeyi, yeni bütçe bilgileriyle günceller.
         * Toplam bütçe ve kategori bazlı bütçeler değiştirilebilir.
         * 
         * @note Bu fonksiyon, mevcut bütçe kaydını günceller. Eğer budgetId
         * geçersizse veya bütçe bulunamazsa, FileNotFound hatası döner.
         * Güncelleme işlemi, veritabanında mevcut kaydı değiştirir.
         * 
         * @param budgetId Güncellenecek bütçenin benzersiz ID'si (geçerli bir budgetId olmalı, 0'dan büyük)
         * @param budget Güncellenmiş bütçe bilgileri (const referans, Budget struct'ı)
         *               İçerik: totalBudget, categoryBudgets[4] vb. Tüm alanlar geçerli olmalı.
         * @return ErrorCode Başarı durumu (Success, FileNotFound, InvalidInput vb.)
         */
        TRAVELEXPENSE_API ErrorCode updateBudget(int32_t budgetId, const Budget& budget);

        /**
         * @brief Bütçe limiti kontrolü
         * 
         * Belirtilen seyahat ve kategori için bütçe limitini kontrol eder.
         * Mevcut harcamaları veritabanından alır ve yeni harcama miktarıyla
         * toplamın kategori bütçesini aşıp aşmadığını kontrol eder.
         * 
         * @note Bu fonksiyon, yeni bir gider eklenmeden önce bütçe kontrolü
         * yapmak için kullanılır. Eğer seyahat için bütçe bulunamazsa,
         * false döner (bütçe olmadığı için limit aşılmış kabul edilir).
         * 
         * @param tripId Seyahat ID (geçerli bir tripId olmalı, 0'dan büyük)
         * @param category Gider kategorisi (Accommodation, Transportation, Food, Other)
         *                 Bu kategori için bütçe limiti kontrol edilir.
         * @param amount Kontrol edilecek harcama miktarı (TL cinsinden, 0 veya pozitif olmalı)
         *               Bu miktar, mevcut harcamalara eklendiğinde limit aşılıyor mu kontrol edilir.
         * @return true Bütçe limiti içinde (harcama yapılabilir), false Limit aşıldı (harcama yapılamaz)
         */
        TRAVELEXPENSE_API bool checkBudgetLimit(int32_t tripId, ExpenseCategory category, double amount);
    }

} // namespace TravelExpense // LCOV_EXCL_LINE

#endif // BUDGET_MANAGER_H



# 📏 NodeMCU LCD Web Panel

WiFi ile LCD kontrolü, log sistemi, özel karakterler ve mobil uyumlu web arayüz — hepsi tek pakette!

---

## 🧠 Proje Hakkında

Bu proje, bir **NodeMCU ESP8266** ile **16x2 LCD ekranı** WiFi üzerinden kontrol etmeni sağlar.

* LCD'ye yazı gönderebilir
* Kayan yazı yazdırabilir
* Kalp, gülücük, ok gibi özel karakterler ekleyebilir
* Yazıları **log** olarak kaydedebilirsin
* Tüm bunlar mobil uyumlu bir web arayüzden yapılır

---

## 🔢 Özellikler

* ✅ Web tabanlı LCD yazı kontrolü
* ✅ Kayan yazı efekti (otomatik kayar)
* ✅ \[HEART], \[SMILE], \[ARROW] gibi karakter desteği
* ✅ Maksimum 16 karakter sınırı (LCD taşmaz)
* ✅ Gönderimde gecikmeli yazma desteği
* ✅ SPIFFS loglama (dosyayla tutar)
* ✅ Log temizleme butonu
* ✅ HTML + CSS sade arayüz
* ✅ Mobil cihazlarda tam uyumluluk
* ✅ Şifreli giriş (varsayılan: `admin123`)

---

## 🔌 Devre Bağlantısı

 <img src="https://content.instructables.com/FXN/UFQ5/J4YFYB8H/FXNUFQ5J4YFYB8H.png?auto=webp&fit=bounds&frame=1">

| LCD Pin | NodeMCU Pin |
| ------- | ----------- |
| VCC     | Vin         |
| GND     | GND         |
| SDA     | D2 (GPIO4)  |
| SCL     | D1 (GPIO5)  |

---

## 🚀 Kurulum

1. Arduino IDE'ye ESP8266 kart paketini ekle
2. Aşağıdaki kütüphaneleri yükle:

   * `ESP8266WiFi.h`
   * `ESP8266WebServer.h`
   * `LiquidCrystal_I2C.h`
   * `FS.h` (SPIFFS)
3. `.ino` dosyasını NodeMCU'ya yükle
4. SPIFFS içeriğini `ESP8266 Sketch Data Upload` ile yükle
5. NodeMCU kendini Access Point modunda başlatacak: `LCD_Panel`
6. WiFi ağına bağlan: Şifre `12345678`
7. Tarayıcıya `192.168.4.1` yaz ve web panel açılsın
8. Parola: `admin123`

---

## 📃 Log Sistemi

Tüm yazılar `/log.txt` dosyasına SPIFFS ile yazılır:

* Tarih-saat damgası
* Hangi satır(a) yazıldığı
* Yazı içeriği

Loglar web panelde listelenebilir ya da `Logları Temizle` butonuyla temizlenebilir.

---

## 🔐 Güvenlik

* Varsayılan şifre: `admin123`
* Koddan değiştirilebilir:

```cpp
const String webPassword = "admin123";
```

---

## 🌌 Kullanım Alanları

* Anlık duyuru ekranı
* WiFi ile kontrol edilen cihaz projeleri
* Kodlama ve IoT eğitimi
* Basit dashboard paneli

---

## 👨‍💻 Geliştirici

**Ömer Faruk (@omerfaruk-shp)**

> ✨ GitHub: [github.com/omerfaruk-shp](https://github.com/omerfaruk-shp)

---

## 📜 Lisans

Bu proje MIT lisanslıdır.
Kullan, dağıt, geliştir ama emeğe saygılı ol ❤

---

<p align="center">
  <img src="https://media.tenor.com/y2JXkY1pXkwAAAAM/cat-computer.gif" width="180">
</p>

<p align="center"><b>“Basitlik, en büyük karmaşıklıktır.”</b></p>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>  // SPIFFS

LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WebServer server(80);

const char* ssid = "LCD_Panel";
const char* wifiPass = "12345678";
const String webPassword = "admin123";

// Özel karakterler
byte heart[8]  = {0b00000,0b01010,0b11111,0b11111,0b11111,0b01110,0b00100,0b00000};
byte smiley[8] = {0b00000,0b01010,0b00000,0b00000,0b10001,0b01110,0b00000,0b00000};
byte arrow[8]  = {0b00100,0b00110,0b00111,0b11111,0b00111,0b00110,0b00100,0b00000};

// Fonksiyon bildirimleri
void handleRoot();
bool isAuthenticated() {
  return server.hasArg("pass") && server.arg("pass") == webPassword;
}

void saveToLog(String line, String text) {
  File file = SPIFFS.open("/log.txt", "a");
  if (file) {
    unsigned long ms = millis();
    unsigned long sec = (ms / 1000) % 60;
    unsigned long min = (ms / 60000) % 60;
    unsigned long hour = (ms / 3600000);
    String log = "[" + String(hour) + ":" + String(min) + ":" + String(sec) + "] ";
    log += "(Satır " + line + ") → " + text + "\n";
    file.print(log);
    file.close();
  }
}

String readLog() {
  String html = "<h4>🗒 Gönderilenler</h4><ul class='list'>";
  File file = SPIFFS.open("/log.txt", "r");
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      html += "<li>" + line + "</li>";
    }
    file.close();
  } else {
    html += "<li>Log dosyası bulunamadı.</li>";
  }
  html += "</ul><br><a href='/clear?pass=" + webPassword + "' class='btn btn-danger'>🧹 Logları Temizle</a>";
  return html;
}

void handleClear() {
  if (!isAuthenticated()) return handleRoot();
  SPIFFS.remove("/log.txt");
  server.sendHeader("Location", "/log?pass=" + webPassword);
  server.send(303);
}

String loginPage() {
  return R"====(
  <!DOCTYPE html><html lang="tr">
  <head><meta charset="UTF-8"><title>Giriş</title></head>
  <body style='text-align:center;margin-top:40px;font-family:sans-serif;'>
    <h2>🔐 LCD Panel Giriş</h2>
    <form method='get'>
      Parola: <input name='pass' type='password'><br><br>
      <input type='submit' value='Giriş'>
    </form>
  </body></html>
  )====";
}

String controlPage() {
  return R"====(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <title>LCD Kontrol Paneli</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; background: #f5f5f5; padding: 20px; color: #333; }
    h2 { text-align: center; }
    .card {
      background: white; padding: 20px; border-radius: 8px;
      max-width: 500px; margin: auto; box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    label { font-weight: bold; display: block; margin-top: 10px; }
    input, select {
      width: 100%; padding: 8px; margin-top: 5px;
      margin-bottom: 10px; border-radius: 4px; border: 1px solid #ccc;
    }
    button, .btn {
      display: inline-block; background: #0080ff; color: white;
      padding: 10px 15px; margin: 5px 0; text-decoration: none;
      border-radius: 5px; border: none; cursor: pointer;
    }
    .btn-danger { background: #e74c3c; }
    .btn-success { background: #2ecc71; }
    .btn-warning { background: #f1c40f; color: black; }
    .btn-info { background: #3498db; }
    .btn-dark { background: #444; }
    .list { list-style: none; padding-left: 0; }
    .list li {
      background: #eee; margin-bottom: 5px;
      padding: 8px; border-radius: 4px;
    }
    .actions { display: flex; flex-wrap: wrap; gap: 8px; margin-top: 10px; }
  </style>
  <script>
    function appendChar(charCode) {
      const textInput = document.querySelector("input[name='text']");
      textInput.value += charCode;
      textInput.focus();
    }
  </script>
</head>
<body>
  <h2>📟 LCD Kontrol Paneli</h2>
  <div class="card">
    <form action="/gonder" method="get">
      <input type="hidden" name="pass" value=")====" + webPassword + R"====(">
      <label>Satır</label>
      <select name="line">
        <option value="1">1</option>
        <option value="2">2</option>
      </select>
      <label>Yazı</label>
      <input name="text" placeholder="LCD'ye yazılacak metin">
      <label>Gecikme (sn)</label>
      <input name="delay" type="number" value="0" min="0">
      <button type="submit" class="btn">Gönder</button>
    </form>

    <div class="actions">
      <button onclick="appendChar('[HEART]')" class="btn btn-danger">❤️ Kalp</button>
      <button onclick="appendChar('[SMILE]')" class="btn btn-warning">😊 Gülücük</button>
      <button onclick="appendChar('[ARROW]')" class="btn btn-success">➡️ Ok</button>
    </div>

    <hr>
    <h4>Kayan Yazı</h4>
    <form action="/gonder" method="get">
      <input type="hidden" name="pass" value=")====" + webPassword + R"====(">
      <input type="hidden" name="line" value="1">
      <input type="hidden" name="scroll" value="1">
      <input type="text" name="text" placeholder="Kayan yazı gir">
      <button class="btn btn-info">▶ Kayan Yazı Gönder</button>
    </form>

    <hr>
    <a href="/log?pass=)====" + webPassword + R"====(" class="btn btn-dark">🗒 Gönderilenleri Gör</a>
  </div>
</body>
</html>
  )====";
}

void handleRoot() {
  if (!isAuthenticated()) {
    server.send(200, "text/html", loginPage());
    return;
  }
  server.send(200, "text/html", controlPage());
}

void handleLogPage() {
  if (!isAuthenticated()) return handleRoot();
  server.send(200, "text/html", controlPage() + readLog());
}

void scrollText(String msg, int row) {
  msg += "                ";
  for (int i = 0; i < msg.length() - 15; i++) {
    lcd.setCursor(0, row);
    lcd.print(msg.substring(i, i + 16));
    delay(300);
  }
}

void handleSend() {
  if (!isAuthenticated()) return handleRoot();

  String text = server.arg("text");
  int line = server.arg("line").toInt();
  int delaySec = server.arg("delay").toInt();

  if (delaySec > 0) delay(delaySec * 1000);

  // Özel karakterleri dönüştür
  text.replace("[HEART]", String(char(0)));
  text.replace("[SMILE]", String(char(1)));
  text.replace("[ARROW]", String(char(2)));

  // Kayan yazı kontrolü
  if (server.hasArg("scroll") && server.arg("scroll") == "1") {
    scrollText(text, line - 1);
  }
  else if (text.startsWith("[SCROLL]")) {
    scrollText(text.substring(8), line - 1);
  }
  else if (text.length() > 16) {
    saveToLog(String(line), "(HATA: 16+ karakter)");
  }
  else if (text.length() > 0) {
    lcd.setCursor(0, line - 1);
    lcd.print("                ");
    lcd.setCursor(0, line - 1);
    lcd.print(text);
    saveToLog(String(line), text);
  }

  server.sendHeader("Location", "/?pass=" + webPassword);
  server.send(303);
}

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, arrow);

  WiFi.softAP(ssid, wifiPass);
  SPIFFS.begin();

  server.on("/", handleRoot);
  server.on("/gonder", handleSend);
  server.on("/log", handleLogPage);
  server.on("/clear", handleClear);
  server.begin();

  Serial.println("Web panel IP: " + WiFi.softAPIP().toString());
}

void loop() {
  server.handleClient();
}

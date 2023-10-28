#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>

// Configura WiFi
const char* ssid = "xxxxxx";
const char* password = "xxxxxx";

// Configura NTP
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", gmtOffset_sec, daylightOffset_sec);

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 0};
Timezone EuropeMadrid(CEST, CET);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 14, 12, U8X8_PIN_NONE);

int prevSecond = -1;
bool timeInitialized = false;
bool wifiConnected = false;

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  Serial.begin(115200);
  Serial.println("");
  Serial.print("Conectando a ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  displayInitialMessage(false);

  unsigned long startTime = millis();
  bool errorDisplayed = false;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    if (millis() - startTime >= 10000 && !errorDisplayed) {
      // Si han pasado 10 segundos y no se ha conectado, muestra un mensaje de error
      displayInitialMessage(true);
      errorDisplayed = true;
    }
  }

  if (errorDisplayed) {
    return; // Salir del setup si se ha mostrado un error
  }

  Serial.println("");
  Serial.print("Conectado a WiFi con dirección IP: ");
  Serial.println(WiFi.localIP());
  wifiConnected = true;

  // Muestra la información del WiFi y la dirección IP durante 5 segundos
  displayWiFiInfo(5000);

  // Inicializa la hora desde Internet solo una vez al inicio
  timeClient.begin();
  timeClient.forceUpdate();
  time_t epochTime = timeClient.getEpochTime();
  setTime(epochTime);
  timeInitialized = true;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!timeInitialized) {
      // Si la hora no se ha inicializado, intenta obtenerla
      timeClient.update();
      if (timeClient.getSeconds() > 0) {
        time_t epochTime = timeClient.getEpochTime();
        setTime(epochTime);
        timeInitialized = true;
      }
    }

    if (timeInitialized) {
      unsigned long epochTime = now();  // Obtiene la hora local actual
      time_t local = EuropeMadrid.toLocal(epochTime);
      displayTime(local);
    }
  }
  delay(1000);
}

void displayInitialMessage(bool error) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Connecting to Wi-Fi...");
  
  if (error) {
    // Se muestra este texto si no se ha logrado la conexión a internet.
    u8g2.drawStr(0, 26, "Error connecting to");
    u8g2.drawStr(0, 35, "Wi-Fi. Please make");
    u8g2.drawStr(0, 44, "sure that this device");
    u8g2.drawStr(0, 53, "is in range and Wi-Fi");
    u8g2.drawStr(0, 62, "data is correct.");
  }
  u8g2.sendBuffer();
}

void displayWiFiInfo(unsigned long duration) {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, ssid);

  const char* textIpAdd = "IP Address:";
  int dirXIp = (128 - u8g2.getStrWidth(textIpAdd)) / 2;
  u8g2.drawStr(dirXIp, 25, textIpAdd);
  String ipAddress = WiFi.localIP().toString();
  int ipX = (128 - u8g2.getStrWidth(ipAddress.c_str())) / 2;
  u8g2.drawStr(ipX, 35, ipAddress.c_str());

  const char* textSignal = "WIFI strength:";
  int dirXSignal = (128 - u8g2.getStrWidth(textSignal)) / 2;
  u8g2.drawStr(dirXSignal, 50, textSignal);
  int signalStrength = WiFi.RSSI();
  String signalStrengthStr = String(signalStrength) + " dBm";
  int signalX = (128 - u8g2.getStrWidth(signalStrengthStr.c_str())) / 2;
  u8g2.drawStr(signalX, 60, signalStrengthStr.c_str());

  u8g2.sendBuffer();
  delay(duration);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}

void displayTime(time_t t) {
  u8g2.clearBuffer();

  // Muestra la fecha en el formato "dd/mm/YYYY"
  u8g2.setFont(u8g2_font_ncenB08_tr);
  String dateStr = "";
  dateStr += day(t);
  dateStr += "/";
  dateStr += month(t);
  dateStr += "/";
  dateStr += year(t);
  u8g2.drawStr(0, 10, dateStr.c_str());

  // Muestra los segundos arriba a la derecha
  u8g2.setFont(u8g2_font_ncenB08_tr);
  char secStr[3];
  sprintf(secStr, "%02d", second(t));
  int secX = 128 - u8g2.getStrWidth(secStr);
  u8g2.drawStr(secX, 10, secStr);

  // Muestra la hora en la parte azul
  u8g2.setFont(u8g2_font_inb30_mr);
  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", hour(t), minute(t), second(t));
  u8g2.drawStr(2, 50, timeStr);

  u8g2.sendBuffer();
}
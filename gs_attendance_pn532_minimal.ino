/*
 * GS Attendance NFC System - ESP32 + PN532 (Minimal Setup)
 * 
 * This is a simplified version for PN532 with minimal components:
 * - ESP32 (30-pin)
 * - PN532 NFC Module  
 * - Breadboard and wires
 * - Optional: Red LED, Buzzer
 * 
 * Only 4 connections needed:
 * PN532 VCC → ESP32 3.3V
 * PN532 GND → ESP32 GND
 * PN532 SDA → ESP32 GPIO 21
 * PN532 SCL → ESP32 GPIO 22
 * 
 * Author: Generated for GNMS Backend System
 * Date: October 2025
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

// Pin Definitions - Minimal Setup
#define PN532_SDA  21   // I2C SDA pin
#define PN532_SCL  22   // I2C SCL pin
#define LED_BUILTIN 2   // Built-in LED (Green)
#define LED_RED    4    // Optional Red LED
// #define BUZZER     13   // Uncomment if you have a buzzer

// WiFi Configuration - CHANGE THESE!
const char* ssid = "Redmi 12C"; // your WiFi SSID        
const char* password = "aakee2002"; // your WiFi password

// API Configuration - CHANGE THESE!
const char* apiBaseUrl = "http://10.84.233.172:8081"; // Your server IP and port
const char* apiEndpoint = "/api/gs-attendance/toggle";
const String gsOfficeId = "83:F0:6F:06";  // Your office ID

// PN532 NFC Module (I2C Mode)
Adafruit_PN532 nfc(PN532_SDA, PN532_SCL);
//Adafruit_PN532 nfc(Wire);

// System Variables
unsigned long lastCardRead = 0;
const unsigned long cardReadDelay = 3000; // 3 seconds delay between same card reads
String lastCardUID = "";
bool wifiConnected = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║     GS Attendance NFC System        ║");
  Serial.println("║     ESP32 + PN532 (Minimal)         ║");
  Serial.println("╚══════════════════════════════════════╝");
  
  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  #ifdef BUZZER
  pinMode(BUZZER, OUTPUT);
  #endif
  
  // LED test
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_RED, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_RED, LOW);
  
  // Initialize I2C and PN532
  Wire.begin();
  setupPN532();
  
  // Connect to WiFi
  setupWiFi();
  
  printSystemInfo();
  Serial.println("\n✓ System ready - Place NFC card near reader...\n");
}

void loop() {
  // Check WiFi connection
  checkWiFiConnection();
  
  // Check for NFC card
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
    handleCardRead(uid, uidLength);
  }
  
  delay(250); // Small delay for stability
}

void setupWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_RED, !digitalRead(LED_RED)); // Blink red LED
    attempts++;
  }
  
  digitalWrite(LED_RED, LOW);
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    
    // Success indication
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
    
    #ifdef BUZZER
    tone(BUZZER, 1000, 200);
    delay(250);
    tone(BUZZER, 1200, 200);
    #endif
    
  } else {
    wifiConnected = false;
    Serial.println("\n✗ WiFi connection failed!");
    
    // Error indication
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_RED, HIGH);
      delay(100);
      digitalWrite(LED_RED, LOW);
      delay(100);
    }
    
    #ifdef BUZZER
    tone(BUZZER, 400, 300);
    delay(100);
    tone(BUZZER, 300, 300);
    #endif
  }
}

void setupPN532() {
  Serial.println("Initializing PN532...");
  
  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("✗ PN532 not found!");
    Serial.println("Check connections:");
    Serial.println("- VCC → 3.3V");
    Serial.println("- GND → GND"); 
    Serial.println("- SDA → GPIO 21");
    Serial.println("- SCL → GPIO 22");
    Serial.println("- Switches: SEL0=ON, SEL1=OFF");
    
    while (true) {
      digitalWrite(LED_RED, HIGH);
      delay(500);
      digitalWrite(LED_RED, LOW);
      delay(500);
    }
  }
  
  Serial.println("✓ PN532 found!");
  Serial.print("Chip: PN5"); 
  Serial.print((versiondata>>24) & 0xFF, HEX);
  Serial.print(" Firmware: "); 
  Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print(".");
  Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Configure to read RFID tags
  nfc.SAMConfig();
  Serial.println("PN532 configured for NFC reading");
}

void handleCardRead(uint8_t* uid, uint8_t uidLength) {
  String cardUID = getCardUID(uid, uidLength);
  unsigned long currentTime = millis();
  
  // Prevent rapid successive reads
  if (cardUID == lastCardUID && (currentTime - lastCardRead) < cardReadDelay) {
    return;
  }
  
  lastCardUID = cardUID;
  lastCardRead = currentTime;
  
  Serial.println("\n╔═══ NFC Card Detected ═══╗");
  Serial.print("║ UID: ");
  Serial.print(cardUID);
  for (int i = cardUID.length(); i < 15; i++) Serial.print(" ");
  Serial.println(" ║");
  Serial.print("║ Length: ");
  Serial.print(uidLength);
  Serial.println(" bytes             ║");
  Serial.print("║ Time: ");
  Serial.print(currentTime);
  for (int i = String(currentTime).length(); i < 10; i++) Serial.print(" ");
  Serial.println("   ║");
  Serial.println("╚════════════════════════════╝");
  
  // Visual feedback
  digitalWrite(LED_BUILTIN, HIGH);
  
  #ifdef BUZZER
  tone(BUZZER, 1000, 100);
  #endif
  
  // Send to API
  sendAttendanceData(cardUID);
  
  digitalWrite(LED_BUILTIN, LOW);
}

void sendAttendanceData(String rfid) {
  if (!wifiConnected) {
    Serial.println("✗ No WiFi - Cannot send data");
    digitalWrite(LED_RED, HIGH);
    delay(1000);
    digitalWrite(LED_RED, LOW);
    return;
  }
  
  HTTPClient http;
  String url = String(apiBaseUrl) + apiEndpoint + "?rfid=" + rfid + "&gsOfficeId=" + gsOfficeId;
  
  Serial.println("Sending to API...");
  Serial.println("URL: " + url);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("User-Agent", "ESP32-PN532-Attendance/1.0");
  http.setTimeout(10000);
  
  int httpCode = http.POST("");
  
  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);
    Serial.print("Response: ");
    Serial.println(response);
    
    if (httpCode == 200) {
      Serial.println("✓ Success!");
      
      // Success LED pattern
      for (int i = 0; i < 2; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(300);
        digitalWrite(LED_BUILTIN, LOW);
        delay(300);
      }
      
      // Different sounds for IN/OUT
      if (response.indexOf("IN") != -1) {
        Serial.println("Status: CHECKED IN");
        #ifdef BUZZER
        tone(BUZZER, 800, 500);
        #endif
      } else if (response.indexOf("OUT") != -1) {
        Serial.println("Status: CHECKED OUT");
        #ifdef BUZZER
        tone(BUZZER, 600, 200);
        delay(100);
        tone(BUZZER, 600, 200);
        #endif
      }
      
    } else {
      Serial.println("✗ Server Error");
      digitalWrite(LED_RED, HIGH);
      delay(500);
      digitalWrite(LED_RED, LOW);
      
      #ifdef BUZZER
      tone(BUZZER, 400, 300);
      #endif
    }
  } else {
    Serial.print("✗ HTTP Error: ");
    Serial.println(httpCode);
    
    // Error LED pattern
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_RED, HIGH);
      delay(150);
      digitalWrite(LED_RED, LOW);
      delay(150);
    }
    
    #ifdef BUZZER
    tone(BUZZER, 300, 500);
    #endif
  }
  
  http.end();
}

String getCardUID(uint8_t* uid, uint8_t uidLength) {
  String uidString = "";
  for (uint8_t i = 0; i < uidLength; i++) {
    if (uidString.length() > 0) uidString += ":";
    if (uid[i] < 0x10) uidString += "0";
    uidString += String(uid[i], HEX);
  }
  uidString.toUpperCase();
  return uidString;
}

void checkWiFiConnection() {
  static unsigned long lastCheck = 0;
  
  if (millis() - lastCheck > 5000) { // Check every 5 seconds
    lastCheck = millis();
    
    if (WiFi.status() != WL_CONNECTED && wifiConnected) {
      wifiConnected = false;
      Serial.println("✗ WiFi disconnected!");
      digitalWrite(LED_RED, HIGH);
    } else if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
      wifiConnected = true;
      Serial.println("✓ WiFi reconnected!");
      digitalWrite(LED_RED, LOW);
    }
  }
}

void printSystemInfo() {
  Serial.println("\n╔═══════════════════════════════════════╗");
  Serial.println("║           SYSTEM INFO                 ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.print("║ ESP32 Chip: Rev ");
  Serial.print(ESP.getChipRevision());
  Serial.println("                     ║");
  Serial.print("║ CPU: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz                       ║");
  Serial.print("║ Free RAM: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes               ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.println("║           CONFIGURATION               ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.print("║ WiFi: ");
  Serial.print(ssid);
  for (int i = strlen(ssid); i < 29; i++) Serial.print(" ");
  Serial.println("║");
  Serial.print("║ Server: ");
  Serial.print(apiBaseUrl);
  for (int i = strlen(apiBaseUrl); i < 27; i++) Serial.print(" ");
  Serial.println("║");
  Serial.print("║ Office: ");
  Serial.print(gsOfficeId);
  for (int i = gsOfficeId.length(); i < 27; i++) Serial.print(" ");
  Serial.println("║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.println("║           PIN SETUP                   ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.println("║ PN532 SDA: GPIO 21                   ║");
  Serial.println("║ PN532 SCL: GPIO 22                   ║");
  Serial.println("║ Built-in LED: GPIO 2                 ║");
  Serial.println("║ Red LED: GPIO 4 (Optional)           ║");
  #ifdef BUZZER
  Serial.println("║ Buzzer: GPIO 13 (Connected)          ║");
  #else
  Serial.println("║ Buzzer: Not connected                ║");
  #endif
  Serial.println("╚═══════════════════════════════════════╝");
}
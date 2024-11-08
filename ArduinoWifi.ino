//#include <dummy.h>

//#include <Arduino_DebugUtils.h>

//#include <Arduino_ESP32_OTA.h>

//#include <ArduinoHttpClient.h>

#include <UnoWiFiDevEd.h>

#include <WiFiS3.h>

#include <WiFi.h>
//#include <HTTPClient.h>

const char* ssid = "Netlab-OIL430";
const char* password = "DesignChallenge";    
String serverPath = "https://nickvanhooff.com/DuurzameScannerApi/public/api/products";  // Vervang door je eigen API-URL



// Definieer het statische IP-adres, subnetmasker en gateway
IPAddress local_IP(192, 168, 68, 249); // Vervang dit door het gewenste IP-adres
IPAddress gateway(192, 168, 68, 1);     // Vervang dit door het IP-adres van je router
IPAddress subnet(255, 255, 255, 0);    // Vervang dit door je subnetmasker

void setup() {
    Serial.begin(9600);

    // Probeer het statische IP-adres in te stellen
    WiFi.config(local_IP, gateway, subnet);
    // Verbinden met het WiFi-netwerk
    WiFi.begin(ssid, password);

    // Wacht tot de verbinding is gemaakt
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Verbinden met WiFi...");
    }

    Serial.print("Verbonden met WiFi!");
    Serial.print("IP adres: ");
    Serial.println(WiFi.localIP());

    
}

void loop() {
    // Je code hier
    if (WiFi.status() == WL_CONNECTED) {
        delay(1000);
         Serial.print("IP adres: ");
        Serial.println(WiFi.localIP());
    }
}
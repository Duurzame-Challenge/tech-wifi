#include <R4HttpClient.h>

//#include <dummy.h>

//#include <Arduino_DebugUtils.h>

//#include <Arduino_ESP32_OTA.h>

#include <ArduinoHttpClient.h>

#include <UnoWiFiDevEd.h>

#include <WiFiS3.h>

#include <WiFi.h>
//#include <HTTPClient.h>

const char* ssid = "Redmi 13C";
const char* password = "mika_2004";    
String serverPath = "https://nickvanhooff.com";  // Vervang door je eigen API-URL

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverPath, 443);

// Definieer het statische IP-adres, subnetmasker en gateway
IPAddress local_IP(192, 168, 68, 249); // Vervang dit door het gewenste IP-adres
IPAddress gateway(192, 168, 68, 1);     // Vervang dit door het IP-adres van je router
IPAddress subnet(255, 255, 255, 0);    // Vervang dit door je subnetmasker

void setup() {
    Serial.begin(9600);

    // Probeer het statische IP-adres in te stellen
   // WiFi.config(local_IP, gateway, subnet);
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
        GetRequest();
    }
}

void GetRequest()
{
  Serial.println("making GET request");
  client.get("/DuurzameScannerApi/public/api/products");

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  delay(5000);
}
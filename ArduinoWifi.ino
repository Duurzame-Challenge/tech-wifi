#include <WiFiS3.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

const char *ssid = "NICK";
const char *password = "password";
const char *server = "nickvanhooff.com"; // Server without "http://"
const int port = 80;                     // Use 80 for HTTP, 443 for HTTPS

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

// Define static IP configuration
IPAddress local_IP(192, 168, 68, 249); // Desired static IP
IPAddress gateway(192, 168, 68, 1);    // Gateway IP (usually your router)
IPAddress subnet(255, 255, 255, 0);    // Subnet mask

void setup()
{
    Serial.begin(2000000);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }

    Serial.println("Connected to Wi-Fi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        delay(1000);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        GetRequest();
    }
    else
    {
        Serial.println("Wi-Fi not connected");
    }
    delay(8000); // Wait 10 seconds before the next request
}

void GetRequest()
{
    Serial.println("Attempting to connect to server...");

    if (client.connect(server, port))
    {
        Serial.println("Connected to server. Making GET request...");

        client.beginRequest();
        client.get("/DuurzameScannerApi/public/api/products");
        client.sendHeader("Content-Type", "application/json");
        client.sendHeader("Accept", "application/json");
        client.endRequest();

        int statusCode = client.responseStatusCode();
        Serial.print("Status code: ");
        Serial.println(statusCode);

        if (statusCode == 200)
        {
            Serial.println("Request successful! Reading response as a buffer:");

            // Buffer to store response chunks
            const size_t bufferSize = 512;
            char buffer[bufferSize];

            while (client.available())
            {
                int len = client.readBytes(buffer, bufferSize - 1);
                buffer[len] = '\0';   // Null-terminate the buffer
                Serial.print(buffer); // Print the buffer
            }
            Serial.println(); // Newline for better readability
        }
        else
        {
            Serial.println("Request failed. Check server or endpoint.");
        }
    }
    else
    {
        Serial.println("Failed to connect to server.");
    }
    client.stop();
}

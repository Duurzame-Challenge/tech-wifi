#include <WiFiS3.h>
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

const char *ssid = "Mika";
const char *password = "password";
const char *server = "nickvanhooff.com";
const int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, port);

// Static IP configuration
IPAddress local_IP(192, 168, 68, 249);
IPAddress gateway(192, 168, 68, 1);
IPAddress subnet(255, 255, 255, 0);

const int PIN_LED_GREEN = 1;
const int PIN_LED_RED = 2;
const int PIN_MOTOR = 3;

void setup()
{
    Serial.begin(2000000);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_MOTOR, OUTPUT);
    // Connect to Wi-Fi
    connectToWiFi();
}

void loop()
{
    // Check for Wi-Fi connection
    if (WiFi.status() == WL_CONNECTED)
    {
        // Check if there is incoming data from the barcode scanner
        if (Serial.available() > 0)
        {
          digitalWrite(PIN_MOTOR, HIGH);
            // Read the scanned barcode from Serial
            String barcode = Serial.readStringUntil('\n');
            barcode.trim(); // Remove any extraneous newline or whitespace
            Serial.print("Scanned Barcode: ");
            Serial.println(barcode);
            // Send the barcode to the server
            searchProductByBarcode(barcode);
        }
    }
    else
    {
        Serial.println("Wi-Fi not connected");
    }
    delay(100); // Small delay to avoid overloading the loop
}

void connectToWiFi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void searchProductByBarcode(const String &barcode)
{
    Serial.println("Searching for product by barcode...");

    // Ensure the client is disconnected before a new request
    client.stop();

    if (client.connect(server, port))
    {
        sendPostRequest(barcode);
        int statusCode = client.responseStatusCode();
        Serial.print("Status code: ");
        Serial.println(statusCode);

        if (statusCode == 200)
        {
            handleResponse();
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

void sendPostRequest(const String &barcode)
{
    client.beginRequest();
    client.post("/DuurzameScannerApi/public/api/product-by-barcode");

    // Set headers
    client.sendHeader("Content-Type", "application/json");
    client.sendHeader("Accept", "application/json");

    // Send JSON body with the barcode
    String jsonBody = "{\"barcode\": \"" + barcode + "\"}";
    Serial.print("Sending JSON data: ");
    Serial.println(jsonBody);

    client.sendHeader("Content-Length", jsonBody.length());
    client.beginBody();
    client.print(jsonBody);
    client.endRequest();
}

void handleResponse()
{
    digitalWrite(PIN_MOTOR, LOW);

    Serial.println("Reading response...");

    // Read and store the full response
    String response = "";
    const size_t bufferSize = 512;
    char buffer[bufferSize];

    while (client.available())
    {
        int len = client.readBytes(buffer, bufferSize - 1);
        buffer[len] = '\0';
        response += buffer;
    }

    // Print the full response
    Serial.println("Full response:");
    Serial.println(response);

    // Check for sustainability information
    if (response.indexOf("\"sustainabilities\":[]") == -1)
    {
        Serial.println("Sustainability information found:");
        printSustainabilityDetails(response);
    }
    else
    {
        Serial.println("No sustainability information found for this product.");
    }

    // Check for allergen information
    if (response.indexOf("\"allergens\":[]") == -1)
    {
        Serial.println("Allergen information found:");
        printAllergenDetails(response);
    }
    else
    {
        Serial.println("No allergen information found for this product.");
    }

    // Check for alternative products
    if (response.indexOf("\"alternatives\":[]") == -1)
    {
        Serial.println("Alternative products found:");
        printAlternativeDetails(response);
    }
    else
    {
        Serial.println("No alternative products found for this product.");
    }

    Serial.println();
}

void printSustainabilityDetails(const String &response)
{
    int sustainabilitiesStart = response.indexOf("\"sustainabilities\":");
    if (sustainabilitiesStart != -1)
    {
        String sustainabilities = response.substring(sustainabilitiesStart, response.indexOf("]", sustainabilitiesStart) + 1);
        Serial.println("Sustainabilities:");
        Serial.println(sustainabilities);

        blinkLED(PIN_LED_GREEN);
    }
    else
    {
      Serial.println("No sustainabilities");
      blinkLED(PIN_LED_GREEN);
    }
}

void printAllergenDetails(const String &response)
{
    int allergensStart = response.indexOf("\"allergens\":");
    if (allergensStart != -1)
    {
        String allergens = response.substring(allergensStart, response.indexOf("]", allergensStart) + 1);
        Serial.println("Allergens:");
        Serial.println(allergens);
    }
}

void printAlternativeDetails(const String &response)
{
    int alternativesStart = response.indexOf("\"alternatives\":");
    if (alternativesStart != -1)
    {
        String alternatives = response.substring(alternativesStart, response.indexOf("]", alternativesStart) + 1);
        Serial.println("Alternatives:");
        Serial.println(alternatives);
    }
}

void blinkLED(int ledPin)
{
    for (int i = 0; i < 3; i++) // Knipper 3 keer
    {
        digitalWrite(ledPin, HIGH);
        delay(200); // Lampje blijft 200 ms aan
        digitalWrite(ledPin, LOW);
        delay(200); // Lampje blijft 200 ms uit
    }
}
// void vibrateMOTOR(int motor)
// {
//   digitalWrite(motor, HIGH);
//   delay (3000);
//   digitalWrite(motor, LOW);

// }


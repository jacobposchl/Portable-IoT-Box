#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>  // Include the ArduinoJson library

const char* ssid1 = "mar's iPhone";       // First Wi-Fi network name
const char* password1 = "marissa2";  // First Wi-Fi password

const char* ssid2 = "SpectrumSetup-FD";   // Second Wi-Fi network name
const char* password2 = "priorerror976";  // Second Wi-Fi password

String apiKey = "F3VPYTFZ542GCTUO";     // ThingSpeak API Key
const char* server = "api.thingspeak.com";  // ThingSpeak server

// Initialize the LCD with ESP8266 pins
LiquidCrystal lcd(D2, D1, D6, D7, D5, D8);  // RS, E, D4, D5, D6, D7

WiFiClient client;  // Create a WiFi client object

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("LOADING...");

  // Try connecting to the first Wi-Fi
  if (!connectToWiFi(ssid1, password1)) {
    // If connection to the first Wi-Fi fails, try the second one
    if (!connectToWiFi(ssid2, password2)) {
      // If both fail, display an error
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WiFi Failed");
      Serial.println("WiFi connection failed.");
      return;
    }
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/channels/2658952/fields/1.json?api_key=" + apiKey + "&results=1";
    
    // Debug: Printing the URL for verification
    Serial.print("Requesting URL: ");
    Serial.println(url);
    
    http.begin(client, url);  
    int httpCode = http.GET();

    // Debug: Print HTTP code received
    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if (httpCode == 200) {  // Check for a successful response
      String payload = http.getString();
      
      // Debug: Print the entire ThingSpeak payload
      Serial.println("Payload received from ThingSpeak:");
      Serial.println(payload);

      // Create a JSON document to parse the payload
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, payload);

      // Debug: Check if JSON parsing was successful
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
      }

      // Extract the value of "field1"
      const char* message = doc["feeds"][0]["field1"];
      
      if (message) {
        Serial.println("Extracted message:");
        Serial.println(message);  // Debug: Print the extracted message

        // Display the message on the LCD with scrolling and multi-line handling
        displayScrollingText(message);
      } else {
        Serial.println("No data found in field1");
      }
    } else {
      Serial.print("Error fetching data from ThingSpeak. HTTP Code: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(5000);  // Update every 5 seconds
}

// Function to connect to WiFi
bool connectToWiFi(const char* ssid, const char* password) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 10) {
    delay(1000);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected to ");
    lcd.setCursor(0, 1);
    lcd.print(ssid);
    return true;  // Return true if connection is successful
  } else {
    Serial.println("\nFailed to connect to WiFi");
    return false;  // Return false if connection fails
  }
}

// Function to display scrolling text and handle multi-line display
void displayScrollingText(const char* text) {
  int textLength = strlen(text);

  // Debug: Display the length of the text
  Serial.print("Message length: ");
  Serial.println(textLength);

  // Display the first 16 characters on the first row
  lcd.clear();
  if (textLength <= 16) {
    lcd.setCursor(0, 0);
    lcd.print(text);
    Serial.println("Displaying text on first row only.");
  } else {
    // If the text is longer than 16 characters, scroll or display on two lines
    for (int i = 0; i < textLength - 15; i++) {
      lcd.clear();
      
      // Display first 16 characters on the first row
      lcd.setCursor(0, 0);
      lcd.print(text + i);  // Display from the i-th character onward

      // Debug: Print what is being shown on the first row
      Serial.print("First row display: ");
      Serial.println(text + i);

      // Display remaining characters on the second row (if applicable)
      if (i + 16 < textLength) {
        lcd.setCursor(0, 1);
        lcd.print(text + i + 16);

        // Debug: Print what is being shown on the second row
        Serial.print("Second row display: ");
        Serial.println(text + i + 16);
      }

      delay(1500);  // Adjust delay for scrolling speed
    }
  }
}

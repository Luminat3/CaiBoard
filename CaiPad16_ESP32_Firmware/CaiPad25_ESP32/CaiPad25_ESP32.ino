#include <Keypad.h>
#include <BleKeyboard.h>
#include <RF24.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Define NRF24L01 pins
#define CE_PIN 4
#define CSN_PIN 5

// Set up RF24
RF24 radio(CE_PIN, CSN_PIN);

// NRF24L01 address (shared between transmitter and receiver)
const byte address[6] = "00001";

// Keypad configuration
const byte ROWS = 4; // Number of rows
const byte COLS = 4; // Number of columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {15, 2, 16, 17}; // Row pins connected to the ESP32
byte colPins[COLS] = {26, 27, 13, 14}; // Column pins connected to the ESP32
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Initialize BLE Keyboard
BleKeyboard bleKeyboard("AcaiPad16", "JAN", 100);

// WiFi credentials
const char* ssid = "my_ap";
const char* password = "my_ap_password";

// Function to read sensor data
int readDaya() {
  // Replace with actual sensor reading code
  return analogRead(34); // Example pin
}

void setup() {
  Serial.begin(115200);

  // Start BLE keyboard
  bleKeyboard.begin();

  // Initialize NRF24L01
  if (!radio.begin()) {
    Serial.println("NRF24L01 failed to initialize");
    while (1);
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.println("Setup complete");
}

void loop() {
  char key = keypad.getKey(); // Read the pressed key from the matrix keypad

  if (key) {
    Serial.println("Key pressed: " + String(key));

    // Send keypress over NRF24L01
    bool success = radio.write(&key, sizeof(key));
    if (success) {
      Serial.println("Key sent via NRF24: " + String(key));
    } else {
      Serial.println("Failed to send key via NRF24");
    }

    // Optionally send keypress via Bluetooth
    if (bleKeyboard.isConnected()) {
      bleKeyboard.print(key);
      Serial.println("Key sent via BLE: " + String(key));
    }
  }

  // Send HTTP request every 10 seconds
  static unsigned long lastHttpRequest = 0;
  if (millis() - lastHttpRequest >= 10000) {
    lastHttpRequest = millis();

    int daya = readDaya();
    HTTPClient http;
    http.begin("https://acaipad.k31.my.id/daya");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String tipe = bleKeyboard.isConnected() ? "BLE" : "WIFI";
    String httpRequestData = "password=abv&daya=" + String(daya) + "&tipe=" + tipe;
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  }

  delay(100); // Short delay to debounce keys
}

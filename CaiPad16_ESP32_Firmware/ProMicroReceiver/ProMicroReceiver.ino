#include <RF24.h>
#include <Keyboard.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

// NRF24L01 address (shared with the transmitter)
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);

  // Initialize NRF24L01
  if (!radio.begin()) {
    Serial.println("NRF24L01 failed to initialize");
    while (1);
  }
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  // Start USB Keyboard
  Keyboard.begin();
}

void loop() {
  char receivedKey;

  // Check if a key was received
  if (radio.available()) {
    radio.read(&receivedKey, sizeof(receivedKey));
    Serial.println("Received key: " + String(receivedKey));

    // Send the keypress to the PC via USB
    Keyboard.print(receivedKey);
  }

  delay(10);
}

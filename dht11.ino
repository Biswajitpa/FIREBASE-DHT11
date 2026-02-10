#include <WiFi.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
// ==== WiFi credentials ====
#define WIFI_SSID "Biswajit"
#define WIFI_PASSWORD "12345678"
// ==== Firebase credentials ====
#define API_KEY "AIzaSyCNXpCpi6Prn-YnwQESytkLX22aXa22hZQ"
#define DATABASE_URL "https://dhtesp32-f92da-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Replace with your Firebase RTDB URL
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to Wi-Fi");
  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  // Anonymous sign-in
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Anonymous SignUp successful");
  } else {
    Serial.printf("SignUp failed: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.setInt(&fbdo, "/Test/value", random(0, 100))) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.println("Failed to send data: " + fbdo.errorReason());
    }
  }
}

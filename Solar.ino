#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h> 

// Wi-Fi credentials
#define WIFI_SSID "POCO F6"           
#define WIFI_PASSWORD "12345678"   

// Firebase setup
FirebaseData firebaseData;              
FirebaseAuth auth;                      
FirebaseConfig config;  
const int sensorPin = A0;   
String currentPath = "/sensor/currentData";
String voltagePath = "/sensor/voltageData";
String powerPath = "/sensor/powerData";
String efficiencyPath = "/sensor/efficiencyData";              

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi!");
  config.host = "Host id " ;
  config.signer.tokens.legacy_token = "Legacy tokens" //higher version try to use the web api , if this is depreceated
  Firebase.begin(&config, &auth);
  if (Firebase.ready()) {
    Serial.println("Firebase connected and ready!");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void loop() {
int sensorValue = analogRead(sensorPin); 
int randomVoltage = random(32, 48); 
float voltage = sensorValue * (3.3 / 1023.0);
float power = randomVoltage * voltage; 
float inputPower = 90.0; 
float efficiency = (power / inputPower) * 100; 
int roundedPower = round(power); 
int roundedEfficiency = round(efficiency);
    if (Firebase.setInt(firebaseData, currentPath, voltage)) {
        Serial.print("Current data sent: ");
        Serial.println(voltage);  
    } else {
        Serial.print("Failed to send current data: ");
        Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setFloat(firebaseData, voltagePath, randomVoltage)) {
        Serial.print("Voltage data sent: ");
        Serial.println(randomVoltage);  
    } else {
        Serial.print("Failed to send voltage data: ");
        Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setFloat(firebaseData, powerPath, roundedPower)) {
        Serial.print("Power data sent: ");
        Serial.println(power);  
    } else {
        Serial.print("Failed to send power data: ");
        Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setFloat(firebaseData, efficiencyPath, roundedEfficiency)) {
        Serial.print("Efficiency data sent: ");
        Serial.println(efficiency); 
    } else {
        Serial.print("Failed to send efficiency data: ");
        Serial.println(firebaseData.errorReason());
    }

    delay(5000); 
}

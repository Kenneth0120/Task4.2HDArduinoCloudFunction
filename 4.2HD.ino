#include <WiFiNINA.h>
#include <Firebase_ESP_Client.h>

// Provide your WiFi credentials
#define WIFI_SSID "SmartHome"
#define WIFI_PASSWORD "router666"

// For Firebase ESP Client library
#define FIREBASE_PROJECT_ID "sit210-4-2hd"
#define API_KEY "AIzaSyB9v8Lq2K6e7KcGPixv8MxkkI0gSQYzSK8"
#define DATABASE_URL "https://sit210-4-2hd-default-rtdb.firebaseio.com/" // e.g., "https://your-project-id.firebaseio.com/"

const int redLED = 2;   // Red LED connected to digital pin 2
const int greenLED = 3; // Green LED connected to digital pin 3
const int blueLED = 4;

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
bool ledStatus = false;
bool ledStatusRed = false;
bool ledStatusGreen = false;
bool ledStatusBlue = false;

void setup() {
  pinMode (redLED, OUTPUT);
  pinMode (greenLED, OUTPUT);
  pinMode (blueLED, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print ("Connecting to Wi-Fi");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUP OK");
    signupOK = true;
  }
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Connected to Firebase");
  } else {
    Serial.println("Failed to connect to Firebase");
  }

}

void loop() {
  // Fetch all LED states in a single request
  if (Firebase.RTDB.getJSON(&fbdo, "/")) { // Get the root of the JSON tree, which includes all LED states
    if (fbdo.dataType() == "json") {
      FirebaseJson &json = fbdo.jsonObject();
      FirebaseJsonData jsonData;

      // Get blue LED state
      json.get(jsonData, "blue");
      ledStatusBlue = jsonData.boolValue;
      digitalWrite(blueLED, ledStatusBlue);

      // Get red LED state
      json.get(jsonData, "red");
      ledStatusRed = jsonData.boolValue;
      digitalWrite(redLED, ledStatusRed);

      // Get green LED state
      json.get(jsonData, "green");
      ledStatusGreen = jsonData.boolValue;
      digitalWrite(greenLED, ledStatusGreen);
    }
  } else {
    Serial.println("Failed to fetch data: " + fbdo.errorReason());
  }
}

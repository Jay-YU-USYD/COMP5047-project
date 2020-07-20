
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "gitfit-1eeb0.firebaseio.com"
#define FIREBASE_AUTH "TqYPd2XaI7XUgs2fxuG614FhMRR8xHCV4HO8cNt5"
#define WIFI_SSID "Ywj"
#define WIFI_PASSWORD "19950521"

uint8_t pin_led5 = D5;
//uint8_t pin_led3 = D3;

void setup() {
  pinMode(pin_led5, OUTPUT);
//  pinMode(pin_led3, OUTPUT);
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  digitalWrite(pin_led5, HIGH);
  delay(500);
  digitalWrite(pin_led5, LOW);
  delay(500);
  digitalWrite(pin_led5, HIGH);
  delay(500);
  digitalWrite(pin_led5, LOW);
  delay(500);
  digitalWrite(pin_led5, HIGH);
  delay(500);
  digitalWrite(pin_led5, LOW);
  delay(500);
}

int n = 0;
int g = 0;
void loop() {
  n = Firebase.getInt("Steps");
  g = Firebase.getInt("TargetSteps");

  if(n >= g){
    delay(100);
    digitalWrite(pin_led5,HIGH);
  }
  if(n < g){
    delay(100);
    digitalWrite(pin_led5,LOW);
  }
}

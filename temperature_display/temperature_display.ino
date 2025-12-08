#include <WiFi.h>
#include <HTTPClient.h>

#include "./secrets/wifi_creadentials.h"
#include "./secrets/urls.h"

#define DATA_PIN 23
#define CLOCK_PIN 18
#define LATCH_PIN 5

#define DISPLAY_NUM 2

#define TENS_ENABLER_PIN 26
#define ONES_ENABLER_PIN 27

#define MEASUREMENT_DELAY 10000

#define TEMPERATURE_SENSOR_PIN 34

// server settings
String server_url = SERVER_URL;

// display settings
int enabler_pins[2] = {
  TENS_ENABLER_PIN,
  ONES_ENABLER_PIN
};

byte digits[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

byte dash = 0b01000000; // dash

// temperature variables
float temperature;
unsigned long last_count_time = MEASUREMENT_DELAY + 1;

void setup() {
  // set-up shift register pins
  // pinMode(DATA_PIN, OUTPUT);
  // pinMode(CLOCK_PIN, OUTPUT);
  // pinMode(LATCH_PIN, OUTPUT);

  // set-up mosfet pins
  // pinMode(TENS_ENABLER_PIN, OUTPUT);
  // pinMode(ONES_ENABLER_PIN, OUTPUT);

  // digitalWrite(TENS_ENABLER_PIN, LOW);
  // digitalWrite(ONES_ENABLER_PIN, LOW);

  // temperature sensor pin
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);

  // debugging
  Serial.begin(115200);

  // wi-fi connection
  connectToWiFi();
}

void loop() {
  // check if update needed
  if (millis() - last_count_time >= MEASUREMENT_DELAY) {
    last_count_time = millis();

    temperature = getTemperature();
    
    // temperature
    Serial.print("Temperature: ");
    Serial.println(temperature);

    // send request
    HTTPClient http;
    http.setReuse(false);
    int begin = http.begin(server_url);

    Serial.print("wifi status: ");
    Serial.println(WiFi.status());

    Serial.print("begin: ");
    Serial.println(begin);

    http.addHeader("Content-Type", "application/json");
    String payload = "{\"value\":" + String(temperature) + "}";
    int httpCode = http.POST(payload);

    Serial.print("httpCode: ");
    Serial.println(httpCode);

    int connected = http.connected();
    Serial.print("connected: ");
    Serial.println(connected);

    http.end();
  }

  // display current temperature
  // displayNumber(temperature);
}

// wi-fi functions
void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
}

// temperature functions

float getTemperature() {
  int input = analogRead(TEMPERATURE_SENSOR_PIN);
  float voltage = input * (3.3 / 4095.0);
  float temperature = voltage * 100.0;
  return temperature;
}

// display functions

void displayNumber(int number) {
  if (number < 0 || number >= 100) {
  	displayDash(TENS_ENABLER_PIN);
    delayMicroseconds(1000);
    
    displayDash(ONES_ENABLER_PIN);
    delayMicroseconds(1000);
    
    return;
  }
  
  int tens = (number / 10) % 10;
  int ones = number % 10;

  // tens digit
  displayDigit(tens, TENS_ENABLER_PIN);
  delayMicroseconds(1000);

  // ones digit
  displayDigit(ones, ONES_ENABLER_PIN);
  delayMicroseconds(1000);
}

void displayDash(int enabler_pin) {
  // turn off all displays
  disableDisplay(TENS_ENABLER_PIN);
  disableDisplay(ONES_ENABLER_PIN);
  
  // write data to shift-register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, dash);
  digitalWrite(LATCH_PIN, HIGH);

  // enable specified display
  enableDisplay(enabler_pin);
}

void displayDigit(int digit, int enabler_pin) {
  // turn off all displays
  disableDisplay(TENS_ENABLER_PIN);
  disableDisplay(ONES_ENABLER_PIN);
  
  // write data to shift-register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digits[digit]);
  digitalWrite(LATCH_PIN, HIGH);

  // enable specified display
  enableDisplay(enabler_pin);
}

void enableDisplay(int enabler_pin) {
  digitalWrite(enabler_pin, HIGH);
}

void disableDisplay(int enabler_pin) {
  digitalWrite(enabler_pin, LOW);
}
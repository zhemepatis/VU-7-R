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
double temperature;

// interrupt variables
hw_timer_t* measurement_timer = NULL;
volatile bool measurement_flag = true;

hw_timer_t* display_timer = NULL;
volatile bool display_refresh_flag = true;
volatile int display_digit_index = 0;   // 0 = tens, 1 = ones

void setup() {
  // interrupt set-up
  setupMeasurementTimer();
  setupDisplayTimer();

  // set-up shift register pins
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // set-up mosfet pins
  pinMode(TENS_ENABLER_PIN, OUTPUT);
  pinMode(ONES_ENABLER_PIN, OUTPUT);

  digitalWrite(TENS_ENABLER_PIN, LOW);
  digitalWrite(ONES_ENABLER_PIN, LOW);

  // temperature sensor pin
  pinMode(TEMPERATURE_SENSOR_PIN, INPUT);

  // debugging
  Serial.begin(115200);

  // wi-fi connection
  setupWiFi();
}

void loop() {
  if (measurement_flag) {
    measurement_flag = false;

    temperature = getTemperature();

    Serial.print("Temperature: ");
    Serial.println(temperature);

    sendTemperatureToServer(temperature);
  }

  if (display_refresh_flag) {
    display_refresh_flag = false;
    refreshDisplay();
  }
}

// timer interrupts

void IRAM_ATTR onMeasurementTimer() {
  measurement_flag = true;
}

void setupMeasurementTimer() {
  measurement_timer = timerBegin(1000000);   
  timerAttachInterrupt(measurement_timer, &onMeasurementTimer);
  timerAlarm(measurement_timer, 10000000, true, 0);
}

void IRAM_ATTR onDisplayTimer() {
  display_refresh_flag = true;
}

void setupDisplayTimer() {
  display_timer = timerBegin(200000);
  timerAttachInterrupt(display_timer, &onDisplayTimer);
  timerAlarm(display_timer, 1000, true, 0); // 1 kHz
}

// wi-fi functions

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // try to connect to wi-fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // acknowledge
  Serial.print("Connected as ");
  Serial.println(WiFi.localIP());
}

void sendTemperatureToServer(double value) {
  HTTPClient http;
  http.begin(server_url);

  http.addHeader("Content-Type", "application/json");
  String payload = "{\"Temperature\":" + String(value) + "}";
  http.POST(payload);

  http.end();
}

// temperature functions

float getTemperature() {
  int input = analogRead(TEMPERATURE_SENSOR_PIN);
  float voltage = input * (3.3 / 4095.0);
  float temperature = voltage * 100.0;
  return temperature;
}

// display functions

void refreshDisplay() {
    static int current_digit = 0;

    int temp = (int)temperature;
    if (temp < 0 || temp > 99) temp = -1;

    int tens = temp / 10;
    int ones = temp % 10;

    disableDisplay(TENS_ENABLER_PIN);
    disableDisplay(ONES_ENABLER_PIN);

    digitalWrite(LATCH_PIN, LOW);

    if (current_digit == 0) {
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (temp < 0 ? dash : digits[tens]));
      digitalWrite(LATCH_PIN, HIGH);
      enableDisplay(TENS_ENABLER_PIN);
    } 
    else {
      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, (temp < 0 ? dash : digits[ones]));
      digitalWrite(LATCH_PIN, HIGH);
      enableDisplay(ONES_ENABLER_PIN);
    }

    current_digit ^= 1;
}

void enableDisplay(int enabler_pin) {
  digitalWrite(enabler_pin, HIGH);
}

void disableDisplay(int enabler_pin) {
  digitalWrite(enabler_pin, LOW);
}
#define TEMP_SENSOR_PIN A0

#define MEASURE_BTN_PIN 2
#define DEBOUNCE_DELAY 20 // ms

#define SOURCE_VOLTAGE 5.0 // volts

bool temperature_measurement_flag = true;

float temperature;

void setup() {
  Serial.begin(9600);

  pinMode(MEASURE_BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MEASURE_BTN_PIN), onMeasureButtonPress, FALLING);
}

void loop() {
  if (temperature_measurement_flag) {
    int input = analogRead(TEMP_SENSOR_PIN);
    float voltage = input * SOURCE_VOLTAGE / 1023.0; 
    
    temperature = voltage * 100.0;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    temperature_measurement_flag = false;
  }
}

void onMeasureButtonPress() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > DEBOUNCE_DELAY) {
    temperature_measurement_flag = true;
  }

  last_interrupt_time = interrupt_time;
}
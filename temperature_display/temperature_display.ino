#define DATA_PIN 8
#define CLOCK_PIN 10
#define LATCH_PIN 9

#define DISPLAY_NUM 2

#define TENS_ENABLER_PIN 7
#define ONES_ENABLER_PIN 6

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

int counter = 0;

void setup() {
  // set-up shift register pins
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // set-up mosfet pins
  pinMode(TENS_ENABLER_PIN, OUTPUT);
  pinMode(ONES_ENABLER_PIN, OUTPUT);

  digitalWrite(TENS_ENABLER_PIN, LOW);
  digitalWrite(ONES_ENABLER_PIN, LOW);
}

void loop() {
  // TODO:
  // add temperature logic
  // add timer

  displayNumber(counter);
  delay(500); // increment every second
  counter++;
  if (counter > 99) counter = 0;
}

void displayNumber(int number) {
  int ones = number % 10;
  int tens = (number / 10) % 10;

  // refresh quickly
  for (int i = 0; i < 200; i++) {
    // tens digit
    displayDigit(tens, TENS_ENABLER_PIN);
    delayMicroseconds(1000);

    // ones digit
    displayDigit(ones, ONES_ENABLER_PIN);
    delayMicroseconds(1000);
  }
}

void displayDigit(int digit, int enabler_pin) {
  // turn off all displays
  for (int i = 0; i < DISPLAY_NUM; ++i) {
    int curr_pin = enabler_pins[i];
    disableDisplay(curr_pin);
  }
  
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
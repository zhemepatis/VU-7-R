#include <stdlib.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MD_MAX72xx.h>

#define MAX_INPUT_VALUE 1023
#define INPUT_SENSITIVITY 15

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1

#define EEPROM_ADDR_SETTINGS 0
#define CURRENT_SETTINGS_VERSION 1
#define DEFAULT_BRIGHTNESS 5

#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 7

#define PAUSE_BTN_PIN 2
#define BRIGHTNESS_BTN_PIN 3
#define DEBOUNCE_DELAY 200

#define FIRST_PLAYER_PADDLE_PIN A0
#define SECOND_PLAYER_PADDLE_PIN A1

#define MATRIX_HEIGHT 8
#define MATRIX_WIDTH 8
#define PADDLE_LENGTH 2

struct Coordinates {
  unsigned int row;
  unsigned int col;
};

struct MovementVector {
  int horizontal;
  int vertical;
};

struct Paddle {
  Coordinates position; // upper pixel of paddle
  unsigned int length;

  unsigned int pin;
  unsigned int raw_value;
};

struct Ball {
  Coordinates position;
  MovementVector direction;
};

struct GameSettings {
  uint8_t version;
  uint8_t brightness_level;
};

GameSettings settings;

unsigned int brightness_level;
MD_MAX72XX display = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// game objects
Paddle paddle_1;
Paddle paddle_2;
Ball ball;

// flags
volatile bool frame_change_flag = false;

volatile bool paddle_flag = false;
volatile bool ball_flag = false;
volatile bool delay_flag = false;

volatile bool round_start_flag = true;
volatile bool toggle_pause_flag = true;

volatile bool brightness_changed_flag = false;

bool pause_state = false;

void setup() {
  // timer interrupt set up
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 2499;
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  interrupts();

  // external interrupt set up
  pinMode(PAUSE_BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PAUSE_BTN_PIN), onPauseButtonPress, FALLING);

  pinMode(BRIGHTNESS_BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_BTN_PIN), onBrightnessButtonPress, FALLING);
  
  // display setup
  display.begin();
  loadSettings();
  display.clear();

  // for a bit of randomness
  pinMode(A5, INPUT);
  randomSeed(analogRead(A5) ^ micros());

  // game object initialisation
  paddle_1 = {
    {0, 3},
    PADDLE_LENGTH,
    FIRST_PLAYER_PADDLE_PIN,
    analogRead(FIRST_PLAYER_PADDLE_PIN)
  };

  paddle_2 = {
    {7, 3},
    PADDLE_LENGTH,
    SECOND_PLAYER_PADDLE_PIN,
    analogRead(SECOND_PLAYER_PADDLE_PIN)
  };

  // debugging
  Serial.begin(9600);
}

void loop() {
  if (brightness_changed_flag) {
    brightness_changed_flag = false;
    saveBrightness();
    display.control(MD_MAX72XX::INTENSITY, brightness_level);
  }

  if (toggle_pause_flag) {
    toggle_pause_flag = false;
    pause_state = !pause_state;

    // check what was the state before
    if (pause_state) {
      display.clear();
      displayPause();
      display.update();
    }
    else {
      delay_flag = true;
      displayGameFrame(ball, paddle_1, paddle_2);
    }
  }

  if (pause_state) {
    return;
  }
  
  if (paddle_flag) {
    paddle_flag = false;

    bool paddle_1_moved = updatePaddlePosition(&paddle_1);
    bool paddle_2_moved = updatePaddlePosition(&paddle_2);

    frame_change_flag = paddle_1_moved || paddle_2_moved;
  }

  if (round_start_flag) {
    round_start_flag = false;
    frame_change_flag = true;
    
    ball = spawnBall();
  }
  else if (ball_flag) {
    ball_flag = false;
    frame_change_flag = true;

    updateBallPosition(&ball, paddle_1, paddle_2);
  }

  if (frame_change_flag) {
    frame_change_flag = false;
    displayGameFrame(ball, paddle_1, paddle_2);
  }
}

// interrupt functions

ISR(TIMER1_COMPA_vect) {
  static unsigned int paddle_counter = 0;
  static unsigned int ball_counter = 0;
  static unsigned int delay_counter = 0;
  
  if (delay_flag) {
    delay_counter += 10;

    if (delay_counter >= 3000) {
      delay_flag = false;
      delay_counter = 0;
    }
  } 
  else {
    paddle_counter += 10;
    ball_counter += 10;
  }

  if (paddle_counter >= 50) {
    paddle_flag = true;
    paddle_counter = 0;
  }

  if (ball_counter >= 1000) {
    ball_flag = true;
    ball_counter = 0;
  }
}

void onPauseButtonPress() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > DEBOUNCE_DELAY) {
    toggle_pause_flag = true;
  }
  last_interrupt_time = interrupt_time;
}

void onBrightnessButtonPress() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > DEBOUNCE_DELAY) {
    brightness_level = (brightness_level + 1) % 16;
    brightness_changed_flag = true;
  }

  last_interrupt_time = interrupt_time;
}

// EEPROM functions
void loadSettings() {
  GameSettings stored;
  EEPROM.get(EEPROM_ADDR_SETTINGS, stored);

  if (stored.version != CURRENT_SETTINGS_VERSION || stored.brightness_level > 15) {
    settings.version = CURRENT_SETTINGS_VERSION;
    settings.brightness_level = DEFAULT_BRIGHTNESS;
    EEPROM.put(EEPROM_ADDR_SETTINGS, settings);
  } 
  else {
    settings = stored;
  }

  brightness_level = settings.brightness_level;
  display.control(MD_MAX72XX::INTENSITY, brightness_level);
}

void saveBrightness() {
  settings.brightness_level = brightness_level;
  EEPROM.put(EEPROM_ADDR_SETTINGS, settings);
}

// game objects update functions

Ball spawnBall() {
  // generate ball position
  unsigned int row = random(3, 5);
  unsigned int col = random(0, MATRIX_HEIGHT);
  Coordinates position = {row, col};

  // generate ball movement direction
  MovementVector direction;
  int horizontal, vertical;

  if (position.row == 3) {
    horizontal = -1;
    vertical = (random(0, 2) == 1) ? -1 : 1;
    direction = { horizontal, vertical };
  }
  else if (position.row == 4) {
    horizontal = 1;
    vertical = (random(0, 2) == 1) ? -1 : 1;
    direction = { horizontal, vertical };
  }

  // create ball struct
  Ball ball = { position, direction };
  return ball;
}

void updateBallPosition(Ball* ball_ptr, Paddle paddle_1, Paddle paddle_2) {
  Ball& ball = *ball_ptr;

  if ((ball.position.row == 0) || (ball.position.row == (MATRIX_WIDTH - 1))) {
    round_start_flag = true;
    return;
  }

  // check if ball bounces off the wall
  if ((ball.position.col == 0 && ball.direction.vertical == -1) || (ball.position.col == 7 && ball.direction.vertical == 1)) {
    ball.direction.vertical *= -1;
  }

  // check if ball bounces off the one of paddles
  paddleBounceIfNeeded(ball_ptr, paddle_1);
  paddleBounceIfNeeded(ball_ptr, paddle_2);
  
  // change ball status
  Coordinates new_position = {
    ball.position.row + ball.direction.horizontal,
    ball.position.col + ball.direction.vertical
  };

  ball.position = new_position;
  return ball;
}

bool paddleBounceIfNeeded(Ball* ball_ptr, Paddle paddle) {
  Ball& ball = *ball_ptr;

  unsigned int paddle_row = paddle.position.row;
  unsigned int paddle_top = paddle.position.col;
  unsigned int paddle_bottom = paddle.position.col + (paddle.length - 1);

  // check if ball is going to collide
  if ((ball.position.row != (paddle_row + 1)) && (ball.position.row != (paddle_row - 1))) {
    return false;
  }

  // check if collision is direct
  if (ball.position.col > paddle_top && ball.position.col <= paddle_bottom && ball.direction.vertical == -1) {
    ball.direction.horizontal *= -1;
    return true;
  }
  
  if (ball.position.col >= paddle_top && ball.position.col < paddle_bottom && ball.direction.vertical == 1) {
    ball.direction.horizontal *= -1;
    return true;
  }

  // check if ball hits the edge
  if((ball.position.col == (paddle_top - 1)) && ((ball.position.col + ball.direction.vertical) == paddle_top)) {
    ball.direction.horizontal *= -1;
    ball.direction.vertical *= -1;
    return true;
  }

  if((ball.position.col == (paddle_bottom + 1)) && ((ball.position.col + ball.direction.vertical) == paddle_bottom)) {
    ball.direction.horizontal *= -1;
    ball.direction.vertical *= -1;
    return true;
  }

  return false;
}

boolean updatePaddlePosition(Paddle* paddle_ptr) {
  Paddle& paddle = *paddle_ptr;
  
  unsigned int input = analogRead(paddle.pin);
  unsigned int scaled_input = max(0, input - 2);

  unsigned int new_position = scaled_input / (MAX_INPUT_VALUE / (MATRIX_HEIGHT - 1));
  bool has_changed = (new_position != paddle.position.col) && (abs(paddle.raw_value - input) >= INPUT_SENSITIVITY);

  if (has_changed) {
    paddle.position.col = new_position;
    paddle.raw_value = input;
  }

  return has_changed;
}

// display functions

void displayGameFrame(Ball ball, Paddle paddle_1, Paddle paddle_2) {
  display.clear();

  displayPaddle(paddle_1);
  displayPaddle(paddle_2);
  displayBall(ball);

  display.update();
}

void displayBall(Ball ball) {
  display.setPoint(ball.position.row, ball.position.col, true);
}

void displayPaddle(Paddle paddle) {
  unsigned int row_value = 0;

  for (int i = 0; i < paddle.length; ++i) {
    row_value |= (1 << paddle.position.col + i);
  }

  display.setRow(paddle.position.row, row_value);
}

void displayPause() {
  display.setRow(0, 0xFF);
  display.setColumn(0, 0xFF);

  display.setRow(2, 0xBD);
  display.setRow(5, 0xBD);

  display.setRow(MATRIX_HEIGHT - 1, 0xFF);
  display.setColumn(MATRIX_HEIGHT - 1, 0xFF);
}

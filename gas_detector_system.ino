#define POWER_SUPPLY 5 // volts

#define GAS_SENSOR_PIN 0
#define MOTOR_PIN 2
#define BUZZER_PIN 4
#define BTN_PIN 7

#define RESISTANCE_L 10000 // ohms
#define RESISTANCE_O 1123.31 // ohms
#define A 3773.58
#define B -1.54
#define GAS_THRESHOLD 10000 // ppm

#define DEBOUNCE_DELAY 20 // ms

long lastDebounceTime = 0;
int lastBtnState = HIGH;
int btnState = HIGH;

bool ventilationIsOn = false;

void setup()
{
	Serial.begin(9600);
  	
  	pinMode(MOTOR_PIN, OUTPUT);
  	pinMode(BUZZER_PIN, OUTPUT);
  	pinMode(BTN_PIN, INPUT_PULLUP);	
}

void loop()
{
  	int btnInput = digitalRead(BTN_PIN);
  
	int gasInput = analogRead(GAS_SENSOR_PIN);
  	
  	float voltage = gasInput / 1023.0 * POWER_SUPPLY;
  	float resistance_s = RESISTANCE_L * (POWER_SUPPLY / voltage - 1) / voltage;
  	float ratio = resistance_s / RESISTANCE_O;
  	float ppm = A * pow(ratio, B);
      
  	Serial.println(ppm);
  	
  	if (btnInput != lastBtnState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY && btnInput != btnState)
    {
        btnState = btnInput;
        
        if (btnState == LOW)
        {
            deactivateAlarm();
        }
    }

    if (!ventilationIsOn && ppm >= GAS_THRESHOLD)
    {
    	activateVentilation();
     	activateAlarm();
    }
    else if (ventilationIsOn && ppm < GAS_THRESHOLD)
    {
        deactivateVentilation();
    }

    lastBtnState = btnInput;
}

void activateVentilation()
{
	digitalWrite(MOTOR_PIN, HIGH);
    ventilationIsOn = true;
}

void activateAlarm()
{
	tone(BUZZER_PIN, 330);
}

void deactivateVentilation()
{
	digitalWrite(MOTOR_PIN, LOW);
    ventilationIsOn = false;
}

void deactivateAlarm()
{
	noTone(BUZZER_PIN);
}

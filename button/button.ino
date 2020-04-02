const int ledPin = LED_BUILTIN;
const int redPin = 5;
const int yellowPin = 6;
const int greenPin = 7;
const int buttonInterruptPin = 11; // interrupt pin
const int bit0Pin = 8;
const int bit1Pin = 9;
const int buttonPin = 10;

const int STATE_PUSHES_ALLOWED = 0x0;
const int STATE_PUSHES_BLOCKED  = 0x1;
const int STATE_PUSHES_UNKNOWN = 0x2;
const int STATE_BUSY           = 0x3;

volatile bool pressed = false;

char ledState = LOW;
char redState = LOW;
char yellowState = LOW;
char greenState = LOW;
char buttonState = LOW;

unsigned long previousMillis = 0;

const long interval = 250; // 250ms blink

void handleButtonPress() { // send a button pressed signal
  buttonState = !buttonState;
  digitalWrite(buttonPin, buttonState);
  digitalWrite(ledPin, HIGH);
}

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buttonInterruptPin, INPUT_PULLUP);
  pinMode(bit0Pin, INPUT);
  pinMode(bit1Pin, INPUT);
  pinMode(buttonPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonInterruptPin), handleButtonPress, RISING);
}

void loop() {
  unsigned long currentMillis = millis();

  int state = digitalRead(bit1Pin) << 1 | digitalRead(bit0Pin);

  if ( state == STATE_PUSHES_ALLOWED ) {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  }

  if ( state == STATE_PUSHES_BLOCKED ) {
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  }

  if ( state == STATE_PUSHES_UNKNOWN ) {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
  }

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    if ( state == STATE_BUSY ) {
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, ledState);
      digitalWrite(greenPin, LOW);
    }
    digitalWrite(ledPin, ledState); // debug
  }
}

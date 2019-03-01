#include <Servo.h>

const int MOTOR_PIN = D2;
const int BUTTON_PIN = D3;
const int LED_PIN = D4;

const int BUTTON_PRESSED = 0;

Servo servo;
bool isEnabled = false;
unsigned long lastPressTime = 0;
long motorTimeCounter = 0;

void setup() {
  Serial.begin(115200);
  initPin();
}

void initPin() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  handleButton();
  if(isEnabled) {
    moveMotor();
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
}

void handleButton() {
  bool isButtonPressed = digitalRead(BUTTON_PIN) == BUTTON_PRESSED;
  
  if (isButtonPressed && lastPressTime == 0) {
    lastPressTime = millis();
  } 
  else if (!isButtonPressed && lastPressTime !=0){
    unsigned long timeDiff = millis() - lastPressTime;
    if (timeDiff < 1000) {
      onButtonClick();
    }
    else {
      onButtonLongClick();
    }
    lastPressTime = 0;
  }
}

void onButtonClick() {
  isEnabled = !isEnabled;
  motorTimeCounter = 0;
  disableServo();
}

void onButtonLongClick() {
  resetMotor();
}

void moveMotor() {
  if (motorTimeCounter < 500) {
    enableServo();
  }
  // 100000 = 1초
  else if (motorTimeCounter < 100000 * 50) {
    disableServo();
  }
  else {
    disableServo();
    motorTimeCounter = 0;
  }
  motorTimeCounter++;
}

void enableServo() {
  if (!servo.attached()) {
    servo.attach(MOTOR_PIN);
  }
  servo.write(60);
}

void enableServoBackward() {
  if (!servo.attached()) {
    servo.attach(MOTOR_PIN);
  }
  servo.write(110);
}

void disableServo() {
  servo.detach();
}

void resetMotor() {
  isEnabled = false;
  enableServo();
  delay(1400);
  enableServoBackward();
  delay(1000);
  disableServo();
}

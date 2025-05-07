#include <Servo.h>

const int
red = 11,
blue = 10,
green = 9,
buzzer = 8,
servoPin = 3,
maxAttempts = 3;

int
redVal = 0,
blueVal = 0,
greenVal = 0,
angle = 0,
attempts = 0,
lockdownPeriod = 5 * 1000;

Servo servo;

enum State { 
  IDLE,
  WAITING,
  CORRECT,
  UNLOCKED,
  INCORRECT,
  LOCKED_OUT
};

const String correctPassword = "1234";

State state = IDLE;

void setup()
{
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  servo.attach(servoPin);
}

void loop()
{
  switch (state) {
    case IDLE:
    	idle();
    	break;
    
    case WAITING:
    	waitForPass();
    	break;
    
    case CORRECT:
    	unlock();
    	break;
    
    case UNLOCKED:
    	handleUnlocked();
    	break;
    
    case INCORRECT:
    	handleIncorrect();
    	break;
    
    case LOCKED_OUT:
    	lockOut();
    	break;
  }
}

void idle() {
  Serial.print(maxAttempts - attempts);
  Serial.print("/");
  Serial.print(maxAttempts);
  Serial.print(" attempts left. Please enter your password.");
  state = WAITING;
}

void waitForPass() {
  analogWrite(red, 255);
  analogWrite(green, 255);
  analogWrite(blue, 0);
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n');
    userInput.trim();
    
    Serial.println();
    Serial.print("You entered: ");
    Serial.print(userInput);

    if (userInput == correctPassword) {
      state = CORRECT;
    } else {
      state = INCORRECT;
    }
  }
}

void unlock() {
  Serial.println();
  Serial.print("Unlocked!");
  Serial.println();
  
  analogWrite(red, 0);
  analogWrite(green, 255);
  analogWrite(blue, 0);
  
  servo.write(0);
  state = UNLOCKED;
}

void handleIncorrect() {
  Serial.println();
  Serial.print("Incorrect!");
  Serial.println();
  
  attempts++;
  if (attempts >= maxAttempts) {
    state = LOCKED_OUT;
  }
  else state = IDLE;
}

void lockOut() {
  Serial.println("LOCKED OUT!");
  analogWrite(red, 255);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  
  for (int i = lockdownPeriod; i > 0; i -= 1000) {
    Serial.println();
    Serial.print(i / 1000);
    Serial.print("s left");
    
 	tone(buzzer, 1000);  
    delay(500);                  
    noTone(buzzer);  
    delay(500);    
  }
  
  lockdownPeriod += 5000;
  attempts = 0;
  state = IDLE;
  Serial.println();
}

void handleUnlocked() {
  return;
}

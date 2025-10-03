int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 7;
int p = 8;

int d4 = 9;
int d3 = 10;
int d2 = 11;
int d1 = 12;

int buttonPin = 13;

const long INITIAL_VALUE = 10;
long n = INITIAL_VALUE * 100;
long savedValue = 0;

int del = 0;
unsigned long lastDecrementTime = 0;
unsigned long decrementInterval = 10;

int lastButtonState = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

enum State {
  WAITING,  
  RUNNING, 
  STOPPED 
};

State currentState = WAITING;

void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop()
{
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) handleButtonPress();
    }
  }
  lastButtonState = reading;

  if (currentState == RUNNING) {
    printValue(n);
    unsigned long currentMillis = millis();
    if (currentMillis - lastDecrementTime >= decrementInterval) {
      lastDecrementTime = currentMillis;
      if (n > 0) {
        n--;
      } else {
        currentState = STOPPED;
        savedValue = 0;
        Serial.println("FAILED");
      }
    }
  }

  showNumber(n);
}

void handleButtonPress()
{
  switch(currentState) {
    case WAITING:
      currentState = RUNNING;
      Serial.println("RUNNING");
      break;
      
    case RUNNING:
      currentState = STOPPED;
      savedValue = n;
      Serial.println("STOP");
      printValue(savedValue);
      break;
      
    case STOPPED:
      n = INITIAL_VALUE * 100;
      currentState = WAITING;
      Serial.println("RESET");
      break;
  }
}

void showNumber(long value) {
  int dA = (value / 1000) % 10;  // 10
  int dB = (value / 100) % 10;   // 1
  int dC = (value / 10) % 10;    // 0.1
  int dD = value % 10;           // 0.01

  clearLEDs();
  pickDigit(1);
  pickNumber(dA);
  delayMicroseconds(del);

  clearLEDs();
  pickDigit(2);
  pickNumber(dB);
  digitalWrite(p, HIGH);
  delayMicroseconds(del);
  digitalWrite(p, LOW);

  clearLEDs();
  pickDigit(3);
  pickNumber(dC);
  delayMicroseconds(del);

  clearLEDs();
  pickDigit(4);
  pickNumber(dD);
  delayMicroseconds(del);
}

void pickDigit(int x)
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
 
  switch(x)
  {
    case 1: digitalWrite(d1, LOW); break;
    case 2: digitalWrite(d2, LOW); break;
    case 3: digitalWrite(d3, LOW); break;
    case 4: digitalWrite(d4, LOW); break;
  }
}

void pickNumber(int x)
{
  switch(x)
  {
    case 0: zero(); break;
    case 1: one(); break;
    case 2: two(); break;
    case 3: three(); break;
    case 4: four(); break;
    case 5: five(); break;
    case 6: six(); break;
    case 7: seven(); break;
    case 8: eight(); break;
    case 9: nine(); break;
    default: clearLEDs(); break;
  }
}

void clearLEDs()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void zero(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,LOW);
}
void one(){
  digitalWrite(a,LOW);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
}
void two(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,LOW);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,LOW);
  digitalWrite(g,HIGH);
}
void three(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,HIGH);
}
void four(){
  digitalWrite(a,LOW);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
}

void five(){
  digitalWrite(a,HIGH);
  digitalWrite(b,LOW);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,LOW);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
}
void six(){
  digitalWrite(a,HIGH);
  digitalWrite(b,LOW);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
}
void seven(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
}
void eight(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
}
void nine(){
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(e,LOW);
  digitalWrite(f,HIGH);
  digitalWrite(g,HIGH);
}

void printValue(long val) {
  Serial.print(val / 100);
  Serial.print('.');
  int cents = val % 100;
  if (cents < 10) Serial.print('0');
  Serial.println(cents);
}

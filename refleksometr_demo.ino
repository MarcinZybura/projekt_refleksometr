#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int counterUp = 3;
int counterDown = 2;
int startButton = 5;
int btn1 = 1;
int btn4 = 4;

bool graZakonczona;
int currentLED;
int obecnaDioda;
int LightLED;

unsigned long poprzedniCzas = 0;
unsigned long czasGry = 10000;

int LED1 = 8;
int LED2 = 9;
int LED3 = 10;
int LED4 = 11;
int LED5 = 12;

void setup() {
  lcd.begin();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  pinMode(startButton, INPUT_PULLUP);
  pinMode(counterUp, INPUT_PULLUP);
  pinMode(counterDown, INPUT_PULLUP);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);

  UstawCzas();
}

void loop() {
  if (graZakonczona == true) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    delay(350);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    delay(150);
  }
}

void UstawCzas() {
  lcd.print("Witaj Graczu!");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Wybierz czas gry");
  lcd.setCursor(0, 1);

  digitalWrite(LED1, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);

  do {
    if (digitalRead(counterUp) == LOW) {
      czasGry = czasGry + 10000;
    }
    if (digitalRead(counterDown) == LOW && czasGry > 10) {
      czasGry = czasGry - 10000;
    }
    lcd.setCursor(0, 1);
    lcd.print("Czas: ");
    lcd.print(czasGry / 1000);
    lcd.print("s");

    delay(50);

    digitalWrite(LED5, HIGH);
    delay(300);
    digitalWrite(LED5, LOW);
    delay(200);

  } while (digitalRead(startButton) == HIGH);

  lcd.clear();

  for (int i = 5; i > 0; i--) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    lcd.setCursor(3, 0);
    lcd.print("---->");
    lcd.print(i);
    lcd.print("<----");

    delay(800);
    lcd.clear();
  }

  lcd.setCursor(2, 0);
  lcd.print("---Start!---");

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  lcd.clear();
  czas();
}

void czas() {
  lcd.clear();
  poprzedniCzas = millis();

  while (czasGry > 0) {
    zapal_led();

    if (millis() - poprzedniCzas >= 1000) {
      czasGry -= 1000;
      poprzedniCzas = millis();
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Koniec Gry!");
  delay(3000);
  lcd.clear();

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  restart();
}

void zapal_led() {
  randomSeed(millis());
  LightLED = random(8, 13);

  switch (LightLED) {
    case 8:
      currentLED = LED1;
      obecnaDioda = btn1;
      break;
    case 9:
      currentLED = LED2;
      obecnaDioda = counterDown;
      break;
    case 10:
      currentLED = LED3;
      obecnaDioda = counterUp;
      break;
    case 11:
      currentLED = LED4;
      obecnaDioda = btn4;
      break;
    case 12:
      currentLED = LED5;
      obecnaDioda = startButton;
      break;
  }

  digitalWrite(currentLED, HIGH);

  unsigned long startMillis = millis();
  unsigned long lastMillis = millis();

  boolean buttonState = digitalRead(obecnaDioda);
  boolean lastButtonState = buttonState;

  while (buttonState == HIGH && millis() - startMillis < 5000) {
    // Debouncing dla przycisku
    buttonState = digitalRead(obecnaDioda);
    if (buttonState != lastButtonState) {
      startMillis = millis();
    }
  
    lcd.clear();
    lcd.print("Pozostalo: ");
    lcd.print(czasGry / 1000);

    lastButtonState = buttonState;
  }

  digitalWrite(currentLED, LOW);
}




void restart() {
  lcd.setCursor(0, 0);
  lcd.print("Graj ponownie: 3");
  lcd.setCursor(0, 1);
  lcd.print("Zakoncz gre: 2");
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);

  int choice = 0;

  while (choice == 0) {
    if (digitalRead(counterUp) == LOW) {
      choice = 1;
    } else if (digitalRead(counterDown) == LOW) {
      choice = 2;
    }
  }

  switch (choice) {
    case 1:
      czasGry = 10000;
      UstawCzas();
      lcd.clear();
      break;
    case 2:
      graZakonczona = true;
      lcd.clear();
      lcd.print("Dzieki za gre!");
      break;
  }
}

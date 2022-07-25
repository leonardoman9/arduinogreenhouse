#include <dht.h>
#include <LiquidCrystal.h>

long minuti(int a);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define PHOTO_IN A0
#define DHT11_PIN 7
#define MOIST A1
#define REFRESH_RATE 20

int frame=0;
unsigned long previousMillis = 0;
unsigned long interval = minuti(1);
unsigned long intervalLong = minuti(1);
int lightMax = 600;
int fanState = LOW;
int temperature;
int humidity;
int moist;

void setup() {
  Serial.begin(9600);
  Serial.println("START");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("t:");
  lcd.setCursor(5,0);
  lcd.print("m:");
  lcd.setCursor(13,0);
  lcd.print("h:");

  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  printTH();
  lightControl();
  fanControl();
  moistControl();
}

void lightControl() {
  int lightVal = analogRead(PHOTO_IN);
  if (lightVal < lightMax) {
    digitalWrite(10, HIGH);
  }
  else {
    digitalWrite(10, LOW);
  }
}

void moistControl(){
  int moistVal = analogRead(MOIST);
  Serial.println(moistVal);
  if(moistVal!= moist){
    moist = moistVal;

    lcd.setCursor(5,1);
    lcd.print("    ");
    lcd.setCursor(5,1);
    lcd.print(moistVal);
   }
}

void fanControl() {
  unsigned long currentMillis = millis();
  if (fanState == LOW) { //se la ventola è spenta
    if (currentMillis - previousMillis >= intervalLong) {
      previousMillis = currentMillis;
      digitalWrite(13, HIGH);
      fanState = HIGH;
    }
  }
  else if (fanState == HIGH) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(13, LOW);
      fanState = LOW;
    }
  }
  Serial.print("Cambio di stato della ventola fra ");
  Serial.print(intervalLong / 1000 - (millis() / 1000 - previousMillis / 1000));
  Serial.println(" secondi");
}

void printTH() {
  int chk = DHT.read11(DHT11_PIN);
  int t = (int) DHT.temperature;
  int h = (int) DHT.humidity;
  if (frame==REFRESH_RATE) {
    lcd.clear();
    lcd.begin(16, 2);
     lcd.setCursor(0, 0);
    lcd.print("t:");
    lcd.setCursor(5,0);
    lcd.print("m:");
    lcd.setCursor(13,0);
    lcd.print("h:");

    frame=0;
    Serial.println("LCD pulito");
    }
  if (t!=temperature || h != humidity){
          frame++;

    temperature = t;
    humidity = h;
    lcd.setCursor(0, 1);
    lcd.print("     ");
    lcd.setCursor(0, 1);
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(13, 1);
    lcd.print("    ");
    lcd.setCursor(13, 1);
    lcd.print(h);
    lcd.print("%");
  }

}

long minuti(int a) {
  return a * 60000;
}

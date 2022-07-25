#include <dht.h>
#include <LiquidCrystal.h>


//ins
#define PHOTO_IN A0
#define MOIST A1
#define DHT11_PIN 7
//outs
#define FAN 13
#define WATER 9
#define LIGHT 10

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
void lcdInitialize();
void refreshLcd();
void lightControl();
void printTH();
void lightControl();
void fanControl();
void moistControl();
void checkWater();
long minuti(int a);

const int REFRESH_RATE = 20;
int frame=0;
unsigned long previousMillis = 0;
unsigned long interval = minuti(1);
unsigned long intervalLong = minuti(1);
int lightMax = 600;
int fanState = LOW;
int moistMin = 400;
int temperature;
int humidity;
int moist;


void setup() {
  Serial.begin(9600);
  lcdInitialize();
  for (int i=0; i<=13; i++){
    digitalWrite(i, LOW);
  }
  pinMode(FAN, OUTPUT);
  pinMode(WATER,OUTPUT);
  pinMode(WATER, OUTPUT);
}

void loop() {
  //stampa temperatura e umidità aria
  printTH();
  //controllo luce
  lightControl();
  //controllo ventilazione
  fanControl();
  //controllo umidità terreno e aggiornamento lcd
  moistControl();
  //reimpostazione lcd
  if (frame==REFRESH_RATE) refreshLcd();
}


void lcdInitialize(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("t:");
  lcd.setCursor(5,0);
  lcd.print("m:");
  lcd.setCursor(13,0);
  lcd.print("h:");
}

void refreshLcd(){ 
    lcd.clear();
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    frame=0;
    Serial.println("LCD pulito");
}

void lightControl() {
  int lightVal = analogRead(PHOTO_IN);
  if (lightVal < lightMax) {
    digitalWrite(WATER, HIGH);
  }
  else {
    digitalWrite(WATER, LOW);
  }
}
void printTH() {
  int chk = DHT.read11(DHT11_PIN);
  int t = (int) DHT.temperature;
  int h = (int) DHT.humidity;
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
void moistControl(){
  int moistVal = analogRead(MOIST);
  if(moistVal!= moist){
    frame++;
    moist = 1023-moistVal;
    lcd.setCursor(5,1);
    lcd.print("    ");
    lcd.setCursor(5,1);
    lcd.print(moist);
   }
   if(moistMin-moist>0){
    digitalWrite(WATER, HIGH);
   }else {
    digitalWrite(WATER, LOW);
  }
}

void fanControl() {
  unsigned long currentMillis = millis();
  if (fanState == LOW) { //se la ventola è spenta
    if (currentMillis - previousMillis >= intervalLong) {
      previousMillis = currentMillis;
      digitalWrite(FAN, HIGH);
      fanState = HIGH;
    }
  }
  else if (fanState == HIGH) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      digitalWrite(FAN, LOW);
      fanState = LOW;
    }
  }
  //per qualche motivo, queste tre righe sono fondamentali per il funzioamento dell'lcd
  Serial.print("Cambio di stato della ventola fra ");
  Serial.print(intervalLong / 1000 - (millis() / 1000 - previousMillis / 1000));
  Serial.println(" secondi");
}

void checkWater(){
 //TODO

}


long minuti(int a) {
  return a * 60000;
}

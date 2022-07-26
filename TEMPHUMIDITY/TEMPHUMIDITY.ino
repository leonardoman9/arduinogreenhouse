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
//parametri utili
#define DELAY_TIME 2000   
#define REFRESH_RATE 30


const bool WATER_ON = true;

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
float moistToPercentage(int m);
long minuti(int a);
 
bool irrigated = false;
int frame=0;
unsigned long previousMillis = 0;
unsigned long intervalLong = minuti(1);
const int lightMax = 600;
int fanState = LOW;
const int moistMin = 400;
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
  pinMode(LIGHT, OUTPUT);
}

void loop() {
  Serial.println("-------------------------------");
  //stampa temperatura e umidità aria
  printTH();
  //controllo luce
  lightControl();
  //controllo umidità terreno e aggiornamento lcd
  moistControl();
  //controllo ventilazione
  fanControl();
  
  //reimpostazione lcd
  if (irrigated) lcdInitialize();
  if (frame==REFRESH_RATE) refreshLcd();
  delay(DELAY_TIME);
}
float moistToPercentage(int m)
{
  return (100*(float)m/1023);
}
void moistControl(){
  int moistVal = analogRead(MOIST);
  if(moistVal!= moist){
    frame++;
    moist = 1023-moistVal;
    lcd.setCursor(5,1);
    lcd.print("    ");
    lcd.setCursor(5,1);
    lcd.print(moistToPercentage(moist));
    lcd.print("%");
   }
   
    while (moistMin-moist>0){
      Serial.println("IRRIGATING!");
      irrigated = true;
      if(WATER_ON) digitalWrite(WATER, HIGH);
      moist = 1023-analogRead(MOIST);
      
    }
    digitalWrite(WATER, LOW);
    Serial.print("Moist: ");
    Serial.print(moistToPercentage(moist));
    Serial.println("%");
}

void lcdInitialize(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("t:");
  lcd.setCursor(5,0);
  lcd.print("m:");
  lcd.setCursor(13,0);
  lcd.print("h:");

  //t
   lcd.setCursor(0, 1);
    lcd.print("     ");
    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print((char)223);
    lcd.print("C");
   //moist
    lcd.setCursor(5,1);
    lcd.print("    ");
    lcd.setCursor(5,1);
    lcd.print(moistToPercentage(moist));
    lcd.print("%");
    //h
     lcd.setCursor(13, 1);
    lcd.print("    ");
    lcd.setCursor(13, 1);
    lcd.print(humidity);
    lcd.print("%");
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
    digitalWrite(LIGHT, HIGH);
  }
  else {
    digitalWrite(LIGHT, LOW);
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
  Serial.print("t: ");
  Serial.print(t);
  Serial.println("°C");
  Serial.print("h: ");
  Serial.print(h);
  Serial.println("%");
}


void fanControl() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis>= intervalLong){
    previousMillis = currentMillis;
    fanState = !fanState;
    digitalWrite(FAN, fanState);
  }
  //per qualche motivo, queste tre righe sono fondamentali per il funzioamento dell'lcd
  Serial.print("Fan: ");
  switch(fanState){
    case 0: Serial.print("OFF"); break;
    case 1: Serial.print("ON"); break;
    default: Serial.print("error"); break;
  }
  Serial.print(", time left: ");
  Serial.print(intervalLong / 1000 - (millis() / 1000 - previousMillis / 1000));
  Serial.println(" seconds");
}

void checkWater(){
 //TODO

}
long minuti(int a) {
  return a * 60000;
}

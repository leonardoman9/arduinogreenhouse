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
#define BUZZER 8
//parametri utili

#define LIGHT_MAX 600
#define MOIST_MIN 512
#define DELAY_TIME 1000   
#define REFRESH_RATE 30
#define IRRIGATION_TIME_MAX 5000
const bool WATER_ON = true;


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
void lcdWrite();
void refreshLcd();
void lightControl();
void readTH();
void lightControl();
void fanControl();
void moistControl();
void checkWater();
void setOutputs();
float toPercentage(int m);
long minutes(int a);
 
bool irrigated = false;
int frame=0;
unsigned long previousMillis = 0;
unsigned long irrigationStart;
long irrigationTime;
unsigned long intervalLong = minutes(5);
int fanState = HIGH;
int temperature;
int humidity;
int moist;


void setup() {
  Serial.begin(9600);
  setOutputs();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting...");
  for (int i=0; i<=13; i++){
    digitalWrite(i, LOW);
  }
}

void loop() {
  
  Serial.println("-------------------------------");
  //stampa temperatura e umidità aria
  readTH();
  //controllo umidità terreno e aggiornamento lcd
  moistControl();
  //controllo luce
  lightControl();
  //controllo ventilazione
  fanControl();
  //reimpostazione lcd
  lcdWrite();
  if (frame==REFRESH_RATE) refreshLcd();
  delay(DELAY_TIME);
}
float toPercentage(int m)
{
  return (100*(float)m/1023);
}
void setOutputs(){
  pinMode(FAN, OUTPUT);
  pinMode(WATER,OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void moistControl(){
  int moistVal = analogRead(MOIST);
  if(moistVal!= moist){
    frame++;
    moist = 1023-moistVal;
   
   }
   irrigationStart = millis();
    while (MOIST_MIN-moist>0){
      checkWater();
    }
    irrigationTime= 0;
    digitalWrite(WATER, LOW);
    Serial.print("Moist: ");
    Serial.print(toPercentage(moist));
    Serial.print("%, threshold: ");
    Serial.print(toPercentage(MOIST_MIN));
    Serial.println("%");
}

void checkWater(){
 irrigationTime = millis() - irrigationStart;
  Serial.print((float)irrigationTime/1000);
  Serial.print(" seconds");
 if(irrigationTime > IRRIGATION_TIME_MAX){
      digitalWrite(WATER, LOW);
      lcd.clear();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("REFILL WATER");
      lcd.setCursor(0,1);
      lcd.print("AND RESTART");
      Serial.println(" !!!REFILL WATER!!!");
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(5000);
 } else {
      Serial.println(" IRRIGATING...");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("IRRIGATING!");
      lcd.setCursor(0,1);
      lcd.print("Time left: ");
      lcd.print(5-((int)irrigationTime/1000));
      lcd.print("s");
      irrigated = true;
      if(WATER_ON) digitalWrite(WATER, HIGH);
      moist = 1023-analogRead(MOIST);
 }

}
void lcdWrite(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("t:");
  lcd.setCursor(6,0);
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
    lcd.setCursor(6,1);
    lcd.print("    ");
    lcd.setCursor(6,1);
    lcd.print((int)toPercentage(moist));
    lcd.print("%");
    //h
     lcd.setCursor(13, 1);
    lcd.print("    ");
    lcd.setCursor(13, 1);
    lcd.print(humidity);
    lcd.print("%");
}

void refreshLcd(){ 
    lcdWrite();
    frame=0;
    Serial.println("**********************");
    Serial.println("LCD REFRESHED");
    Serial.println("**********************");
}

void lightControl() {
  int lightVal = analogRead(PHOTO_IN);
  if (lightVal < LIGHT_MAX) {
    digitalWrite(LIGHT, HIGH);
    Serial.print("Light: ON");
  }
  else {
    digitalWrite(LIGHT, LOW);
    Serial.print("Light: OFF");
  }
  Serial.print(", Light lvl: ");
  Serial.print(toPercentage(lightVal));
  Serial.print("%");
  Serial.print(", threshold: ");
  Serial.print(toPercentage(LIGHT_MAX));
  Serial.println("%");
}
void readTH() {
  int chk = DHT.read11(DHT11_PIN);
  int t = (int) DHT.temperature;
  int h = (int) DHT.humidity;
  if (t!=temperature || h != humidity){
    frame++;
    temperature = t; 
    humidity = h;
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
  }
  digitalWrite(FAN, fanState);
  //per qualche motivo, queste righe sono fondamentali per il funzioamento dell'lcd
  Serial.print("Fan: ");
  switch(fanState){
    case 0: Serial.print("OFF"); break;
    case 1: Serial.print("ON"); break;
    default: Serial.print("error"); break;
  }
  Serial.print(", time left: ");
  Serial.print(intervalLong/ 1000 - (millis() / 1000 - previousMillis / 1000));
  Serial.println(" seconds");
}


long minutes(int a) {
  return a * 60000;
}

#include <TimeLib.h>

#include <dht.h>
#include <LiquidCrystal.h>

 int t, h;
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define DHT11_PIN 7
unsigned long previousMillis = 0;    
const long interval = 1000*1*60*10; //10 minuti
const long intervalLong = 1000*1*60*60*2;   //2 ore

  int state=LOW;
void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 lcd.print("Temp:  Umidita':");
 pinMode(13, OUTPUT);    // sets the digital pin 13 as output
}

void loop() {
  time_t t = now();
   int chk = DHT.read11(DHT11_PIN);
   delay(50);
  printTH();
  fanControl();
  
}

void fanControl(){
   unsigned long currentMillis = millis();
 if (state==LOW){   //se la ventola è spenta
  if (currentMillis - previousMillis >= intervalLong) {     //si accende dopo 20 secondi
      previousMillis = currentMillis;
    digitalWrite(13, HIGH);
      state = HIGH;
  }
 }
 else if (state==HIGH) {    //se la ventola è accesa
  if (currentMillis - previousMillis >= interval) {   //si spegne dopo 10 secondi
      previousMillis = currentMillis;
    digitalWrite(13, LOW);
      state = LOW;
  }
 }
}

void printTH(){
   t = (int) DHT.temperature;
   h = (int) DHT.humidity;
  lcd.setCursor(1, 1);
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(13,1);
  lcd.print(h);  
  lcd.print("%");
}

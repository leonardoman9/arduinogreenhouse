#include <dht.h>
#include <LiquidCrystal.h>
int t, h;
long minuti(int a);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define PHOTO_IN A0
#define DHT11_PIN 7
unsigned long previousMillis = 0;    
unsigned long interval = minuti(1); 
unsigned long intervalLong = minuti(1); 
int lightMax = 600;
int state=LOW;
void setup() {
 Serial.begin(9600);
 Serial.println("START");
 lcd.begin(16, 2);
 lcd.print("Temp:  Umidita':");
 pinMode(13, OUTPUT);    
 pinMode(10, OUTPUT);
}

void loop() {
   int chk = DHT.read11(DHT11_PIN);
   delay(50);
  printTH();
  lightControl();
  fanControl();
}

void lightControl(){

 int lightVal = analogRead(PHOTO_IN);
   Serial.println(lightVal);
  if (lightVal<lightMax){
  digitalWrite(10, HIGH);
  }
  else {
    digitalWrite(10,LOW);
  }
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
  Serial.print("Cambio di stato della ventola fra ");
  Serial.print(intervalLong/1000 -(millis()/1000 - previousMillis/1000));
  Serial.println(" secondi");
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

long minuti(int a){
  return a*60000;
}

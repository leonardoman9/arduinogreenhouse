#include <dht.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define DHT11_PIN 7
int t;
int h;
void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 lcd.print("Temp:  Umidita':");
 pinMode(13, OUTPUT);    // sets the digital pin 13 as output
}

void loop() {
   int chk = DHT.read11(DHT11_PIN);
   delay(50);
      t = (int) DHT.temperature;
      h = (int) DHT.humidity;
  printTH();

  if(t>25) {
     digitalWrite(13, HIGH); // sets the digital pin 13 on
  } else {
    digitalWrite(13, LOW);  // sets the digital pin 13 off
    }
  
}

void printTH(){
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  lcd.setCursor(1, 1);
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(13,1);
  lcd.print(h);  
  lcd.print("%");
}

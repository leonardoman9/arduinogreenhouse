#include <dht.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
dht DHT;
#define DHT11_PIN 7

void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 lcd.print("Temp: Umidita':");
}

void loop() {
   int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
   delay(50);
  lcd.setCursor(0, 1);
  int  t = (int) DHT.temperature;
  int h = (int) DHT.humidity;
  Serial.println(t);
  Serial.println(h);
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(7,1);
  lcd.print(h);  
  lcd.print("%");
}

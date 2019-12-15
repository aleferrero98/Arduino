#include <SPI.h>
#include <LoRa.h>

#include <Wire.h> // Librería comunicación I2C
#include <LiquidCrystal_I2C.h> // Librería LCD I2C
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
String lectura1,lectura2;
void setup() {
  Serial.begin(9600);
  while (!Serial);
   
  Serial.println("Electrones de Laplace");

  if (!LoRa.begin(433E6)) {
    Serial.println("Error LoRa!");
    while (1);
  }

  lcd.begin(16,2);
}

void loop() {
  // try to parse packet
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Dato recibido: '");
  lectura1="";
   lcd.setCursor(0,0);
    // read packet
    while (LoRa.available()) {
      char i =(char)LoRa.read();
      Serial.print(i);
      lectura1+=i;
       
    }
    lcd.clear();
lcd.print(lectura1);
lcd.setCursor(0,1);
lcd.print(lectura2);
lectura2=lectura1;
    // print RSSI of packet
    Serial.print("' con RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("' con SNR ");
    Serial.println(LoRa.packetSnr());
  }
}

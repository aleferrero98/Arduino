// include the library code:
#include <LiquidCrystal.h>
#include <DHT11.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
int pinTemp=8;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT11 dht11(pinTemp);

// DEFINICION DE CARACTERES PERSONALIZADOS
byte grado[8] = {
  0b00000110,
  0b00001001,
  0b00001001,
  0b00000110,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  lcd.createChar (0, grado);
       int err;
       float temp, hum;
       if((err = dht11.read(hum, temp)) == 0)    // Si devuelve 0 es que ha leido bien
          {
              lcd.setCursor(0,0);
              lcd.write("Temp: "); // Podemos poner tanto write como print
              lcd.print(temp);
              lcd.write((byte)0);
              lcd.write("C");
              lcd.setCursor(0,1);
              lcd.write("Humedad: "); 
              lcd.print(hum);
              lcd.write("%");

             Serial.print("Temperatura: ");
             Serial.print(temp);
             Serial.print(" Humedad: ");
             Serial.print(hum);
             Serial.println();
          }
       else
          {
             lcd.clear();
             lcd.print("Error Num:");
             lcd.print(err);

             Serial.println();
             Serial.print("Error Num :");
             Serial.print(err);
             Serial.println();
          }
   
    
    delay(1000);
}

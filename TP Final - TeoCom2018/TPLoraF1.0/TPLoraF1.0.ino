
//Definimos las librerias que necesitamos utilizar:

#include <SPI.h>
#include <LoRa.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#include <LowPower.h>
 
#define ONE_WIRE_BUS A3     //Definimos la entrada del dato del sensor de temp

//Creamos una conexión oneWIRE para poder comunicarnos con el sensor de temp con este protocolo

OneWire oneWire(ONE_WIRE_BUS);

//Le decimos a nuestro sensor Dallas que nos vamos a comunicar usando el protocolo oneWire
 
DallasTemperature sensors(&oneWire);

float Temp=0;      //Definimos la variable Temperatura y le asignamos en principio un valor de cero

//int contador=1;

void setup() {

  Serial.begin(9600);   //Arrancamos el puerto serie para poder leer los datos enviados
  while (!Serial);

  Serial.println("Electrones de Laplace");

  if (!LoRa.begin(433E6)) {                    //Se inicializa la librería  con la frecuencia de transmición de 433MHz 
    Serial.println("Error de comunicacion");   //en el caso de de que no se pueda inicializar la transmición
    while (1);
  }

  //iniciamos la libreria del sensor();
  sensors.begin();

  //inicializamos el display  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  delay(100);
  display.clearDisplay();
  LoRa.setTxPower(20); //  2-20  defecto 17
  LoRa.setSpreadingFactor(7);  // 6-12 defecto 7   (igual recep)
  LoRa.setSignalBandwidth(125E3);//  defecto 125E3    (igual recep)
    //BW 7.8E3 10.4E3 15.6E3 20.8E3 31.25E3 41.7E3 62.5E3 125E3 250E3
  LoRa.setCodingRate4(5); //5-8 defeto 5 
  LoRa.setPreambleLength(8); //  6-65535 defecto 8
  LoRa.setSyncWord(0x12);// defecto 0x12

    Temp = sensorTemp();
}

void loop() {
 //establece el arduino en LowPower por 28 segundos
  for (int i = 0 ;  i  <  3 ; i++)
             LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  
  Temp = sensorTemp();                           // almacenamos la temperatura en Temp 
  
  //visualización monitor serial local
  Serial.print("Temperatura Local:  "); 
  Serial.println(Temp);

  // envio de datos

  //se inicializa la secuencia de envio 
  LoRa.beginPacket();  
  LoRa.print("{\"1234\": ");
  LoRa.print("t:"); 
  LoRa.print(Temp,1);
  LoRa.print("}");
  //LoRa.print("c:"); 
  //LoRa.print(contador);
  LoRa.endPacket();               //Se finaliza la secuencia de envio 
  LoRa.sleep();                   //SleepMode
  wDisplay();                     //llamaos la función que escribe en el display
 // delay(3000);
 contador++;
}

//Función para obtención de la Temperatura
float sensorTemp (){
  
   sensors.requestTemperatures();             //Solicitamos la temperatura al Sensor
   float temp = sensors.getTempCByIndex(0);   //Obtenemos la temperatura
   return temp;                               //Devolvemos el valor de la temperatura
 
}


 //la siguiente funcion es la usada para escribir el display local 
 
void wDisplay(){
  
  display.setTextSize(1);                   // establece el tamaño del texto del primer renglon
  display.setTextColor(WHITE);              //color texto
  display.setCursor(0,0);                   //ubica el cursor en la esquina superior izquierda
  display.println("Electrones de Laplace"); //texto preimer renglon + salto de linea 
  display.println("Temp: ");                //texto segundo renglon + salto de linea
  display.setTextSize(2);                   //esablece tamaño de texto 
  display.print("   ");                     //
  display.print(Temp,2);                    // dato de temperatura con 2 decimales
  display.print(" C");
  display.display();
  delay(10);
  display.clearDisplay();                   //limpiamos el display

}
  

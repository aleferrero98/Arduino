#define PWMA 5
#define AIN2 6
#define AIN1 7
#define STBY 8
#define BIN1 9
#define BIN2 10
#define PWMB 11

#include <avr/power.h>
#include <avr/sleep.h>

const int pinMotorA[3] = { PWMA, AIN2, AIN1 };
const int pinMotorB[3] = { PWMB, BIN1, BIN2 };
 
enum moverDireccion{
   adelante,
   atras
};
 
enum girarDireccion {
   derecha,
   izquierda
};
int velocidad = 128; //velocidad al 50%
int estado = 'c';//inicia detenido
int estadoAnterior = 'c';
int velocGiro = 64;
 /*
  * Configura los pines como salida para controlar el driver para motores tb6612
  */
void setup()
{
  /*
   * PWMA - D5
   * AIN2 - D6
   * AIN1 - D7
   * STBY - D8
   * BIN1 - D9
   * BIN2 - D10
   * PWMB - D11
   */
   pinMode(AIN2, OUTPUT);
   pinMode(AIN1, OUTPUT);
   pinMode(PWMA, OUTPUT);
   pinMode(BIN1, OUTPUT);
   pinMode(BIN2, OUTPUT);
   pinMode(PWMB, OUTPUT);
   Serial.begin(9600);
   //enableMotors();
}
 
void loop()
{
    if(Serial.available()>0){ // lee el bluetooth y almacena en estado
      estado = Serial.read();
    }else{
      //estado = 'c';//si no se apreta boton, se detiene
    }
    switch(estado){
      case 't':
        mover(adelante);
        break;
      case 'u':
        girar(izquierda);
        break;
      case 'v':
        detener();
        break;
      case 'w':
        girar(derecha);
        break;
      case 'x':
        mover(atras);
        break;
      case 'p':
        enableMotors();//enciende el auto
        estado = 'c';//detenido
        break;
      case 'z':
        delay(100);//delay necesario
        disableMotors();
        estado = 'c';//detenido
        sleepNow();//modo de bajo consumo
        break;
      default:
        actualizarVelocidad(estado);
        estado=estadoAnterior;
        //delay(100);
    }
    estadoAnterior = estado;
}
 
//Funciones que controlan el vehiculo
void mover(int direccion)
{
   if (direccion == adelante)
   {
      moverMotorAdelante(pinMotorA);
      moverMotorAdelante(pinMotorB);
   }
   else if(direccion == atras)
   {
      moverMotorAtras(pinMotorA);
      moverMotorAtras(pinMotorB);
   }
}
 
void girar(int direccion)
{
   if (direccion == derecha)
   {
      //moverMotorAtras(pinMotorA);
      moverMotorAdelante(pinMotorB);
      stopMotor(pinMotorA);
   }
   else if(direccion == izquierda)
   {
      moverMotorAdelante(pinMotorA);
      stopMotor(pinMotorB);
     //moverMotorAtras(pinMotorB); 
   }
}
 
void fullStop()
{
   disableMotors();
   stopMotor(pinMotorA);
   stopMotor(pinMotorB);
}
 
//Funciones que controlan los motores
void moverMotorAdelante(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], velocidad);
}
 
void moverMotorAtras(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
 
   analogWrite(pinMotor[0], velocidad);
}
void detener(){
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}
 
void stopMotor(const int pinMotor[3])
{
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
 
   analogWrite(pinMotor[0], 0);
}
 
void enableMotors()
{
   digitalWrite(STBY, HIGH);
}
 
void disableMotors()
{
   digitalWrite(STBY, LOW);
}
/*
 * Actualiza la velocidad de los motores (valor del duty cycle del PWM) mediante el valor pasado por parametro (en porcentaje)
 */
void actualizarVelocidad(int vel){
  if(0<=vel<=100){
    velocidad = (vel*255)/100;
  }
}

void sleepNow()
{
/* Now is the time to set the sleep mode. In the Atmega8 datasheet
 * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
 * there is a list of sleep modes which explains which clocks and
 * wake up sources are available in which sleep modus.
 *
 * In the avr/sleep.h file, the call names of these sleep modus are to be found:
 *
 * The 5 different modes are:
 * SLEEP_MODE_IDLE -the least power savings
 * SLEEP_MODE_ADC
 * SLEEP_MODE_PWR_SAVE
 * SLEEP_MODE_STANDBY
 * SLEEP_MODE_PWR_DOWN -the most power savings
 *
 * the power reduction management <avr/power.h> is described in
 * http://www.nongnu.org/avr-libc/user-manual/group_avr_power.html
 */

    set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
    
    sleep_enable(); // enables the sleep bit in the mcucr register
    // so sleep is possible. just a safety pin
    
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();
    
    sleep_mode(); // here the device is actually put to sleep!!
    
    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    sleep_disable(); // first thing after waking from sleep:
    // disable sleep...
    
    power_all_enable();
}


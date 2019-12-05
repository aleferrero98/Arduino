/*
 * Mide la distancia entre el nivel de agua y el desagote del lavarropas en la pileta de lavar los platos
 * Si la distnacia es menor a cierto valor en cm suena la alarma
 * Los valores limite en cm varian de 0 a 9 cm y se setean con un pulsador. Dicho valor aparece en un display de 7 segmentos
 */
#define EchoPin 11
#define TriggerPin 12
#define BUZZER 13
#define BOTON 2
#define segA 5
#define segB 4
#define segC 8
#define segD 9
#define segE 10
#define segF 6
#define segG 7
#define umbralAntirebote 200
volatile int conta = 0;//limite en cm
volatile bool sonando = false;
long startTime = 0;
int contaPeligro = 0;

void setup() {
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);
   pinMode(BOTON, INPUT_PULLUP);//resistor de pull-up para entrada del pulsador
   pinMode(BUZZER, OUTPUT);
//segmentos del display 7seg
   pinMode(segA, OUTPUT);
   pinMode(segB, OUTPUT);
   pinMode(segC, OUTPUT);
   pinMode(segD, OUTPUT);
   pinMode(segE, OUTPUT);
   pinMode(segF, OUTPUT);
   pinMode(segG, OUTPUT);
   attachInterrupt(digitalPinToInterrupt(BOTON), ISR_boton, FALLING);//Interrup por flanco descendente
}
 
void loop() {
    int distancia = ping();
    Serial.print("Distancia: ");
    Serial.println(distancia);
    if(distancia<=conta){
      contaPeligro++;
      if(contaPeligro>=3){//si 3 mediciones seguidas supera el umbral, suena la alarma
        sonarAlarma();
      }
    }else{
      contaPeligro=0;
    }
    mostrarNum(conta);
    delay(1000);
}
void sonarAlarma(){
  digitalWrite(BUZZER,HIGH);
  sonando=true;
}
/*
 * Funcion que calcula y devuelve la distancia en cm mediante el uso del sensor ultrasonico
 */
int ping() {
   long duration, distanceCm;  
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
   return distanceCm;
}
/*
 * Muestra en un display de 7 segmentos el numero indicado
 */
void mostrarNum(int num){
  switch(num){
    case 0:
      display(1,1,1,1,1,1,0);
      break;
    case 1:
      display(0,1,1,0,0,0,0);
      break;
    case 2:
      display(1,1,0,1,1,0,1);
      break;
    case 3:
      display(1,1,1,1,0,0,1);
      break;
    case 4:
      display(0,1,1,0,0,1,1);
      break;
    case 5:
      display(1,0,1,1,0,1,1);
      break;
    case 6:
      display(1,0,1,1,1,1,1);
      break;
    case 7:
      display(1,1,1,0,0,0,0);
      break;
    case 8:
      display(1,1,1,1,1,1,1);
      break;
    case 9:
      display(1,1,1,0,0,1,1);
      break;
   default:
      display(0,0,0,0,0,0,0);
  }
}
void display(int a, int b, int c, int d, int e, int f, int g){
  digitalWrite (segA,a);
  digitalWrite (segB,b);  
  digitalWrite (segC,c);
  digitalWrite (segD,d);
  digitalWrite (segE,e);
  digitalWrite (segF,f);
  digitalWrite (segG,g);
}
/*
 * Rutina de servicio de interrupcion del boton, sirve para apagar la alarma y para setear el umbral de 0 a 9
 */
void ISR_boton(){
if(millis() - startTime > umbralAntirebote){
      if(sonando){
        digitalWrite(BUZZER,LOW);//si esta sonando, el boton solo apaga la alarma, no incrementa conta
        sonando=false;
      }else{
        conta++;
        if(conta==10){
          conta=0;
        }
      }
      startTime = millis();
      mostrarNum(conta);
   }
}

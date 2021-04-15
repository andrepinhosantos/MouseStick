#define horzPin  A0  // Analog output of horizontal joystick pin
#define vertPin  A1  // Analog output of vertical joystick pin
#define pinDT  16
#define pinSCK  10
#define pinBotao 4
#include <HX711.h>
HX711 scale;
int selPin = 7;  // select button pin of joystick
int offPin = 5; // turn off the mouse function
int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
int valorC1,valorC2;
int i=0;
float val=0;
float medida;
float leitura_ambiente;
float medida_anterior;
int state_sopro=0;
//float leitura_sopro;
int state_calibracao=0;

void setup() {
  Serial.begin(9600);
  pinMode(horzPin, INPUT);  // Set both analog pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(selPin, INPUT);  // set button select pin as input
  pinMode(offPin,INPUT);
  scale.begin(pinDT, pinSCK);
  for (i=0;i<=50;i++){ 
    val=analogRead(pinDT);
    medida = val+medida;
    //Serial.print("Calibrando:");
    //Serial.print(i);
    //Serial.println("%");
  } 
  scale.set_scale(medida);
  delay(1000);
}

void loop() {

if(state_calibracao >= 20){
  medida=0;
  val=0;
      for (i=0;i<=50;i++){ 
    val=analogRead(pinDT);
    medida = val+medida;
    //Serial.print("Calibrando:");
   // Serial.print(2*i);
    //Serial.println("%");
  } 
  Serial.print("Medida:");
  Serial.println(medida);
  Serial.println(" ______________________________________________________________");
  Serial.println("|                        CALIBRACAO COMPLETA                   |");
  Serial.println("|______________________________________________________________|");
  state_calibracao=0;
  }
    scale.set_scale(medida);
  //medida_anterior=scale.get_units(5)+0.075;
  leitura_ambiente=scale.get_units(3)+0.425;
  //leitura_ambiente = leitura_ambiente-0.35*leitura_ambiente;
  //leitura_sopro=scale.get_units(1)-leitura_ambiente*0.95+0.4;
  horzValue=analogRead(horzPin);
  vertValue=analogRead(vertPin);
  valorC1=digitalRead(15);
  valorC2=digitalRead(9);
  Serial.println("          ");
  Serial.print(" Valor potenciometro eixo x e eixo y:             ");
  Serial.print(horzValue);
  Serial.print("        ");
  Serial.println(vertValue);
  Serial.print(" Clique esquerdo do mouse:");
  Serial.print("     ");
  Serial.println(valorC2);
  Serial.print(" Clique de Seguranca           ");
  Serial.println(valorC1);
  //Serial.print(" Valor sensor sopro:");
  //Serial.print(leitura_sopro,3);
  //Serial.print("      ");
  //Serial.println("      ");
  Serial.print(" Valor sensor ambiente:");
  Serial.print(leitura_ambiente,3);
  Serial.print("      ");
  Serial.println("      ");
  
  if(leitura_ambiente>0.01){
  state_sopro=1;
    if(state_sopro==1){
  Serial.println("           ");
  Serial.println(" Sopro detectado.");
  Serial.println( "            ");
   }
  state_sopro=0;
  }
  if(valorC1==1){
  Serial.println("                                    ");
  Serial.println("     PANIC BUTTON PRESSED          ");
  Serial.println("                                    ");
  Serial.println(" Restarting the program.");
  Serial.println("                       ");
  delay(20000);
  for (i=0;i<=50;i++){ 
  val=analogRead(pinDT);
  medida = val+medida;
  Serial.print(" Loading:");
  Serial.print(2*i);
  Serial.println("%");
  }
  }
  state_calibracao=state_calibracao+1;
  delay(500);
}

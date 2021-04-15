#include <Mouse.h>
#include <HX711.h>
#define horzPin  A0  // Analog output of horizontal joystick pin
#define vertPin  A1  // Analog output of vertical joystick pin
#define pinDT  16
#define pinSCK  10
#define pinBotao 4

// INSTANCIANDO OBJETOS
HX711 scale;

int selPin = 7;  // select button pin of joystick
int offPin = 8; // turn off the mouse function
int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 100;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 1;
int mouseLeftFlag = 0;
float val=0;
float medida;
float leitura;
float leitura_sopro;
int state_sopro;
int i;
int state_calibracao;
int state_stop=0;

//int invertMouse = 1;        //Invert joystick based on orientation
int invertMouse = -1;         //Noninverted joystick based on orientation

void setup()
{
  pinMode(horzPin, INPUT);  // Set both analog pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(selPin, INPUT);  // set button select pin as input
  pinMode(offPin,INPUT);
  digitalWrite(selPin, HIGH);  // Pull button select pin high
  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  delay(1000);  // short delay to let outputs settle
  vertZero = analogRead(vertPin);  // get the initial values
  horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these
  Mouse.begin(); //Init mouse emulation
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

void loop()
{
  //Calibração sensor de pressão
  if(state_calibracao >= 20){
    for (i=0;i<=10;i++){ 
    val=analogRead(pinDT);
    medida = val+medida;
  } 
  state_calibracao=0;
  }
  scale.set_scale(medida);
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Lógica funcionamento do mouse
  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0); // move mouse on y axis
  if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0); // move mouse on x axis

  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((digitalRead(selPin)) && (mouseClickFlag)) // if the joystick button is not pressed
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
  }
  
  //leitura=scale.get_units(3);// Média de 3 leituras do sensor de pressão sem sopro.
  leitura_sopro=scale.get_units(3);// Média de 3 leituras do sensor durante sopro do usuário.
  if (leitura_sopro > 0)
  {
    state_sopro=1;
    if(state_sopro ==1)
    {
    Mouse.click(MOUSE_RIGHT);
    }
  }
  state_sopro==0;
  ////////////////////////////////////////////////////////////////////////////////////////////
  // Lógica de Segurança
  //state_stop=digitalRead(offPin);
  //if(state_stop==1){
  //Mouse.end();
  //delay(20000);
  //for (i=0;i<=50;i++){ 
  //val=analogRead(pinDT);
  //medida = val+medida;
  //}
  //}
  state_calibracao=state_calibracao+1;
 }

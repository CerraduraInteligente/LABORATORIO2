#include <infrarrojo.h>
#include <Wire.h>

// Apertura con Clave y opcion de cambio de Clave
// by: www.elprofegarcia.com

#include <LiquidCrystal_I2C.h> // Incluir la libreria LiquidCrystal_I2C
#include <LiquidCrystal_PCF8574.h> // Incluir la libreria LiquidCrystal_I2C
#include <Keypad.h>            // Incluir la libreria Keypad
#include <Servo.h>             // Incluir la libreria Servo
int estado=0;                  // 0=cerrado 1=abierto
Servo servo11;                 // Crea el objeto servo11 con las caracteristicas de Servo
const byte FILAS = 4;          // define numero de filas
const byte COLUMNAS = 4;       // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {22,23,24,25};         // pines correspondientes a las filas {9,8,7,6}
byte pinesColumnas[COLUMNAS] = {26,27,28,29};  // pines correspondientes a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto teclado
char TECLA;                        // almacena la tecla presionada
char CLAVE[5];                     // almacena en un array 4 digitos ingresados
char CLAVE_MAESTRA[5] = "0000";    // almacena en un array la contraseña inicial
byte INDICE = 0;                   // indice del array
LiquidCrystal_I2C lcd(0x20,16,2);  // dependiendo del fabricante del I2C el codigo 0x27 cambiar a
                                   // 0x3F , 0x20 , 0x38 , 
                                   


void setup()
{
 Serial.begin(9600);    //VELOCIDAD COMUNICACION SERIAL
 pinMode(9,INPUT);
  
 lcd.init();                          // inicializa el LCD
 lcd.backlight();
 servo11.attach(10,660,1400);        // Asocia el servo1 al pin 10, define el min y max del ancho del pulso 
 servo11.write(150);                // Gira el servo a 150 grados Cierra la puerta
 limpia();
}

void loop(){
  TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable
  
  if (TECLA)                  // comprueba que se haya presionado una tecla
  {
    CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
    INDICE++;                 // incrementa indice en uno
    lcd.print(TECLA);         // envia al LCD la tecla presionada
  }

  if(INDICE == 4)             // si ya se almacenaron los 4 digitos
  {
    if(!strcmp(CLAVE, CLAVE_MAESTRA))    // compara clave ingresada con clave maestra
        abierto();              
    else 
        error();
    
    INDICE = 0;
  }
  
  if(estado==1 && (digitalRead(11)==0))     // si esta abierta y se pulsa boton de Nueva Clave
         nueva_clave();   

  if(estado==1 && (digitalRead(9)==0))    // si esta abierta y se pulsa boton de Nueva Clave  (estado.lectura(VALOR)==1)
         sensorIR();   

}

///////////////////// Error //////////////////////////////
void error(){   
      lcd.setCursor(0,1);
      lcd.print("CLAVE INCORRECTA"); 
      limpia();
}
/////////////////////abierto o cerrado //////////////////////////////
void abierto(){  
   if(estado==0){ 
      estado=1;
      lcd.setCursor(0,1);
      lcd.print("ABIERTO            ");
      servo11.write(30);                 // Gira el servo a 30 grados  abre la puerta
   }
   else{
    estado=0;
    lcd.setCursor(0,1);
    lcd.print("CERRADO              ");  // imprime en el LCD que esta cerrada
    servo11.write(150);                // Gira el servo a 150 grados  cierra la puerta
   } 
 limpia();
}

/////////////////////Nueva_Clave //////////////////////////////
void nueva_clave(){ 
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("NUEVA CLAVE:");
  INDICE=0;
  while (INDICE<=3) {
   TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable TECLA
   if (TECLA)                 // comprueba que se haya presionado una tecla
    {
      CLAVE_MAESTRA[INDICE] = TECLA;    // almacena en array la tecla presionada
      CLAVE[INDICE] = TECLA;
      INDICE++;                 // incrementa indice en uno
      lcd.print(TECLA);         // envia a monitor serial la tecla presionada
    }   
  }
 estado=0;
  lcd.setCursor(0,1);
  lcd.print("CLAVE CAMBIADA");
  delay(500);
  limpia();
}

///////////////////// limpia //////////////////////////////
void limpia(){  
 lcd.setCursor(0,0);
 lcd.print("CLAVE :          ");
 lcd.setCursor(9,0);
}

///////////////////// sensor IR //////////////////////////////
void sensorIR(){ 
 estado=0; 
 lcd.setCursor(0,1);
 lcd.print("CERRADO");
 lcd.setCursor(0,0);
 lcd.print("CLAVE:  ");
}

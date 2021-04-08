#include <LiquidCrystal_I2C.h> // Incluir la libreria LiquidCrystal_I2C
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
byte pinesFilas[FILAS] = {29,28,27,26};         // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {25,24,23,22};  // pines correspondientes a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto teclado
char TECLA;                        // almacena la tecla presionada
char CLAVE[5];                     // almacena en un array 4 digitos ingresados
char CLAVE_MAESTRA[5] = "0000";    // almacena en un array la contraseña inicial
byte INDICE = 0;                   // indice del array
LiquidCrystal_I2C lcd(0x20,16,2);  // dependiendo del fabricante del I2C el codigo 0x27 cambiar a     //para proteus es 0x20
                                   // 0x3F , 0x20 , 0x38 ,   
void setup()
{
 lcd.init();                          // inicializa el LCD
 lcd.backlight();
 servo11.attach(11,660,1400);        // Asocia el servo1 al pin 11, define el min y max del ancho del pulso 
 servo11.write(150);                // Gira el servo a 150 grados Cierra la puerta
 limpia();

 //pinMode(3, INPUT);  //linea de codigo añadida, para probaren proteus
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
  if(estado==1 && (analogRead(A3)==0)){     // si esta abierta y se pulsa boton de Nueva Clave
         nueva_clave(); 
  }
//  if(estado==1 && (digitalRead(3)==0))     // si esta abierta y se pulsa boton de Nueva Clave
//         nueva_clave(); 
}

///////////////////// Error //////////////////////////////
void error(){   
      lcd.setCursor(0,1);
      lcd.print("ERROR DE CLAVE    "); 
      limpia();
}
/////////////////////abierto o cerrado //////////////////////////////
void abierto(){  
   if(estado==0){ 
      estado=1;
      lcd.setCursor(0,1);
      lcd.print("Abierto            ");  // imprime en el LCD que esta abierta
      servo11.write(30);                 // Gira el servo a 30 grados  abre la puerta
   }
   else{
    estado=0;
    lcd.setCursor(0,1);
    lcd.print("Cerrado              ");  // imprime en el LCD que esta cerrada
    servo11.write(150);                // Gira el servo a 150 grados  cierra la puerta
   } 
 limpia();
}

/////////////////////Nueva_Clave //////////////////////////////
void nueva_clave(){  
  lcd.setCursor(0,0);
  lcd.print("NUEVA CLAVE:        ");
  lcd.setCursor(12,0);
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
  delay(2000);
  limpia();
}

///////////////////// limpia //////////////////////////////
void limpia(){  
 lcd.setCursor(0,0);
 lcd.print("CLAVE :          ");
 lcd.setCursor(9,0);
}

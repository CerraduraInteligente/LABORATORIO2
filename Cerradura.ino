//LIBRERIAS
#include <Adafruit_Fingerprint.h> 
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>            
#include <Servo.h>             
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
SoftwareSerial mySerial(2, 3);     // Crear Serial para Sensor  Rx, TX del Arduino
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);  // Crea el objeto Finger comunicacion pin 2 , 3 
int analogo5=0;                   // Variable de lectura del Analogo5 para sensor de obstaculos
                                   
void setup()
{
   lcd.init();                          // inicializa el LCD
   lcd.backlight();
   servo11.attach(11,660,1400);        // Asocia el servo1 al pin 11, define el min y max del ancho del pulso 
   servo11.write(150);                // Gira el servo a 150 grados Cierra la puerta
   limpia();
  
   //pinMode(3, INPUT);  //linea de codigo añadida, para probaren proteus
  
    pinMode(7, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(3, OUTPUT);
    Serial.begin(9600);
  
    while (!Serial);       //  Yun/Leo/Micro/Zero/...
    delay(100);
    Serial.println("Cerradura Inteligente");
  
    // set the data rate for the sensor serial port
    finger.begin(57600);  // inicializa comunicacion con sensor a 57600 Baudios
    delay(5);
    if (finger.verifyPassword()) {
      Serial.println("Detectado un sensor de huella!");
    } else {
      Serial.println("No hay comunicacion con el sensor de huella");
      Serial.println("Revise las conexiones");
      while (1) { delay(1); }
    }
  
    finger.getTemplateCount();
    Serial.print("El sensor contiene "); Serial.print(finger.templateCount); Serial.println(" plantillas");
    Serial.println("Esperando por una huella valida...");
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

    //////////////////////////////////////////////////////////
    // Si el sensor de obtaculos fue obstruido abre la puerta
//    analogo5=analogRead(A5);
//     if ( analogo5<=800) {                   
//           Serial.print("Salida Interna *** ");
//           abrirPuerta();                           
//       }
    if(estado==1 && (analogRead(A5)>0)){     // si esta abierta y el sensor fue obstruido, se cierra la puerta
           abierto(); 
    }
    getFingerprintIDez();
    delay(50);            //retardo de 50 milisegundos entre lecturas
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
   }else{
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

//////////////////////////////////////////////////////////
//////////////////// abrirPuerta() ///////////////////////
void abrirPuerta() {      //la puerta se abre unos segundos y luego se cierra
  Serial.println(" AUTORIZADA *** "); 
  digitalWrite(5, HIGH);   // Abrir la cerradura
  delay(1000);
  servo11.write(90);       // Abrir la puerta 
  delay(3000);             // Tiempo de la puerta abierta
  digitalWrite(7, HIGH);   // Suena el buzzer para indicar que se va a cerrar la puerta
  delay(500);
  servo11.write(170);       // Cierra puerta 
  delay(500);
  digitalWrite(7, LOW);     // apaga el buzzer 
  digitalWrite(5, LOW);     // cierra la cerradura 
}

//////////////////// Mal_Registro() ///////////////////////
void Mal_Registro() {      // Activa el Buzzer 2 veces por tarjeta no autorizada
  digitalWrite(7, HIGH);
  delay(200);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  delay(200);
  digitalWrite(7, LOW);
}

//////////////////// getFingerprintIDez() ///////////////////////
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) { 
    Mal_Registro();
    return -1;
  }
  // Si hay coincidencias de huella
  Serial.print("ID #"); Serial.print(finger.fingerID); 
  Serial.print(" coincidencia del "); Serial.println(finger.confidence);
   if(finger.fingerID==1){
     Serial.print("BIENVENIDO PROFE *** "); 
     abrirPuerta();
   }
   if(finger.fingerID==6){
     Serial.print("BIENVENIDO ERIC *** "); 
     abrirPuerta();
   }
  return finger.fingerID; 
}

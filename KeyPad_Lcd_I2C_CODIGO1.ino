#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x20,16,2);   ///16x2 o con un 20x4.  0x20 para proteus.  0x27 para placa i2c con arduino
 
const byte ROWS = 4; 
const byte COLS = 4;
String todo = ""; 
 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte rowPins[ROWS] = {0,1,2,3}; 
byte colPins[COLS] = {4,5,6,7};
 
int i2caddress = 0x21; // Direccion I2C teclado. 0x21 para proteus.  0x26 para placa i2c puentiada con arduino
 
Keypad_I2C kpd = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, i2caddress );
 
void setup(){
  Serial.begin(9600);
  Serial.println("\nI2C Keypad");
  kpd.begin();
  lcd.begin(16,2);// Columnas y filas de LCD    
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  lcd.setCursor(2,0);
  lcd.print("Hello World!");
  delay(2000);
  lcd.clear();
}
 
void loop(){
  char key = kpd.getKey();
  if (key){
          if (key == '#'){ // Compara char con String.
            todo = "";
            key = ' '; // Observa el espacio y comilla simple.
          }
        Serial.println("Presionado: ");
        Serial.print(todo);
        todo = todo + key;  
    lcd.clear(); // Borra pantalla.
    lcd.setCursor(0,0); // Posicion inicial del cursor.
    lcd.print(todo);
    delay (50);
  }
}  

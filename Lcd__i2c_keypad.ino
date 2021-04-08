#include <Wire.h>
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

byte pinesFilas[ROWS] = {29,28,27,26};         // pines correspondientes a las filas
byte pinesColumnas[COLS] = {25,24,23,22};  // pines correspondientes a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, ROWS, COLS);  // crea objeto teclado
 
void setup(){
  Serial.begin(9600);
  Serial.println("\nI2C Keypad");

  lcd.init();                          // inicializa el LCD
  lcd.backlight();  
    
  lcd.setCursor(2,0);
  lcd.print("Hello World!");
  delay(2000);
  lcd.clear();
}
 
void loop(){
  char key = teclado.getKey();
    if (key){
          if (key == '#'){ // Compara char con String.
          todo = "";
          key = ' '; // Observa el espacio y comilla simple.
          }
        Serial.print(todo);
        todo = todo + key;  
    lcd.clear(); // Borra pantalla.
    lcd.setCursor(0,0); // Posicion inicial del cursor.
    lcd.print(todo);
    delay (50);
  }
}  

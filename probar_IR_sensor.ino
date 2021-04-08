void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);//LED QUE INDICA EL ESTADO DEL LED
  pinMode(A8,INPUT);
  Serial.begin(9600); //VELOCIDAD COMUNICACION SERIAL
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("\n leyendo estado sensor: \n");//IMPRIME MENSAJE EN PC
  Serial.print(digitalRead(A8));//IMPRIME EL ESTADO DEL Vo DEL SENSOR MEDIANTE LA VARIABLE VALOR

  delay(100);//GENERA UN TIEMPO PARA LEER E IMPRIMIR EN PUERTO SERIE 
}

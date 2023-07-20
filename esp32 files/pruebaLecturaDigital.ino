int digitalPin = 23; // Pin GPIO a utilizar

void setup() {
  pinMode(digitalPin, INPUT); // Configurar el pin como entrada digital
  pinMode(21, OUTPUT);
  Serial.begin(115200); // Inicializar comunicación serial para imprimir resultados
}

void loop() {
  int digitalValue = digitalRead(digitalPin); // Leer el estado lógico del pin digital
  
  Serial.println(digitalValue); // Imprimir el valor leído por la comunicación serial

  if(digitalValue==1){
    digitalWrite(21, HIGH);
    }
    if(digitalValue==0){
    digitalWrite(21, LOW);
    }  
  delay(1000); // Esperar 1 segundo
}

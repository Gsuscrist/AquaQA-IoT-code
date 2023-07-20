// Define los pines a utilizar
int sensorPin = 26;   // Pin digital conectado al sensor de nivel ZP5210


void setup() {
  // Inicializa el LED como salida
  pinMode(sensorPin, INPUT);
  
  // Inicializa la comunicación serial para ver los resultados
  Serial.begin(115200);
}

void loop() {
  // Lee el valor del sensor
  float sensorValue = digitalRead(sensorPin);
  Serial.println(sensorValue);
  delay(200);
  
 }

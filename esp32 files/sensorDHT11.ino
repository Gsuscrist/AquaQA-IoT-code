#include <DHT.h>
 DHT dht(26,DHT11);
//se usa el pin 26 por que es el que convierte de digital a analogico

void setup(){
  dht.begin();
  delay(2000);
  Serial.begin(115200);
}

void loop()
{
 float temp = dht.readTemperature();
 float hum = dht.readHumidity();
  //TEMPERATURA//
  Serial.print("Temperatura = ");
  Serial.print(temp);
  Serial.println(" C");

  //HUMEDAD RELATIVA//
  Serial.print("Humedad = ");
  Serial.print(hum);
  Serial.println(" %");
  delay(2000);
}

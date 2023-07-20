/*
 * Author: Gsus
 * 
 */
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

  // level
 int levelPin = 35;
  // dht11
 DHT dht(26,DHT11);
  //ds18b20
 int oneWireBus = 25;
 OneWire oneWire(oneWireBus);
 DallasTemperature sensors(&oneWire);
  // ph
 int phPin = 36;
  //ts
 int tsPin = 39;
  //tds
#define tdsPin 323
#define VREF 5.0              // analog reference voltage(Volt) of the ADC
#define SCOUNT  30

 //tds data filter
 
int analogBuffer[SCOUNT]; 
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;

int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}





void setup() {

  //basic config
  Serial.begin(115200);

  // level
  pinMode(levelPin, INPUT);
  // dht11
  dht.begin();
  //ds18b20
  sensors.begin();
  // ph
  pinMode(phPin, INPUT);
  //ts
  pinMode(tsPin, INPUT);
  //tds
  pinMode(tdsPin, INPUT);
}

void loop() {
  //basic setup (json)
  StaticJsonDocument<300> data;
  String dataParsed;
  
  // level
  bool levelValue = (digitalRead(levelPin) == 1);
  
 
  // dht11
 float externalTemp = dht.readTemperature();
 float externalHum = dht.readHumidity();
  //ds18b20
  sensors.requestTemperatures(); 
  float internalTemp = sensors.getTempCByIndex(0);
  // ph
  float voltage=analogRead(phPin)*(5.0/4095.0);
  float phValue=(5.0*voltage);
  //ts
  int tsValue = analogRead(tsPin);  
  float tension = tsValue * (5.0 / 4095.0);
  //tds
   static unsigned long analogSampleTimepoint = millis();
  if(millis()-analogSampleTimepoint > 40U){     //read every 40 milliseconds
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(tdsPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if(analogBufferIndex == SCOUNT){ 
      analogBufferIndex = 0;
    }
  }
   
  static unsigned long printTimepoint = millis();
  if(millis()-printTimepoint > 800U){
    printTimepoint = millis();
    for(copyIndex=0; copyIndex<SCOUNT; copyIndex++){
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];      
      // analog data filtering
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 4096.0;
      //temperature compensation
      float compensationCoefficient = 1.0+0.02*(externalTemp-25.0);
      //voltage compensation
      float compensationVoltage=averageVoltage/compensationCoefficient;
      
      
      tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5;

      }
    }


 // Guardando en Json
  data["externalTemp"] = externalTemp;
  data["externalHum"] = externalHum;
  data["internalTemp"] = internalTemp;
  data["phValue"] = phValue;
  data["tsValue"] = tsValue;
  data["tensionValue"] = tension;
  data["tdsValue"] = tdsValue;
  data["level"] = levelValue;
  data["userId"]= 1;

  delay(3000);


  // Json -> String
  serializeJson(data,dataParsed);
  //printing
  Serial.println(dataParsed);


}

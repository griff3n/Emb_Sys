// Dieser Code ist vorbereitet das Praktikum am Mittwoch
#include <Wire.h>

const uint8_t ReadTemperature = 170;  //AAh
const uint8_t AccessTH = 161;         //A1h
const uint8_t AccessTL = 162;         //A2h
const uint8_t AccessConfig = 172;     //ACh
const uint8_t ReadCounter = 168;      //A8h
const uint8_t ReadSlope = 169;        //A9h
const uint8_t StartConvertT = 238;    //EEh
const uint8_t StopConvertT = 34;      //22h
const uint8_t SlaveAdress = 144;      //1001000
const uint8_t Temperatur1_2 = 128;    //0,5 Grad Celsius
const float Temperatur = 24.5;

void setTemperature(float t) {
  int t2 = t * 10;
  if(t2 % 5 != 0) {
    return;
  }
  uint8_t temperatur = t;
  bool t1_2 = false;
  if(t2 % 10 != 0) {
    t1_2 = true;
  }
  
  Wire.beginTransmission(SlaveAdress);
  Wire.write(AccessTH);
  Wire.write(temperatur);
  if(t1_2) {
    Wire.write(Temperatur1_2);
  } else {
    Wire.write(0);
  }
  Wire.endTransmission();

  Wire.beginTransmission(SlaveAdress);
  Wire.write(AccessTL);
  Wire.write(temperatur);
  if(t1_2) {
    Wire.write(Temperatur1_2);
  } else {
    Wire.write(0);
  }
  Wire.endTransmission(); 
}


void setup() {
  Serial.begin(9600);
  Wire.setModule(0);
  Wire.begin();
  Wire.beginTransmission(SlaveAdress);
  Wire.write(AccessConfig);
  Wire.write(2);
  Wire.endTransmission();
  setTemperature(Temperatur);
}

void loop() {
  float t = 0;
  Wire.beginTransmission(SlaveAdress);
  Wire.write(StartConvertT);
  Wire.endTransmission();
  Wire.beginTransmission(SlaveAdress);
  Wire.write(ReadTemperature);
  Wire.endTransmission();

  Wire.requestFrom(SlaveAdress + 1, 2);
  uint8_t val = Wire.read();
  if((val & 128) != 0) {
    t = val - 256;
  } else {
    t = val;
  }
  val = Wire.read();
  if(val != 0) {
    t += 0.5;
  }
  Serial.print("Temperatur: ");
  Serial.print(t);
  Serial.print("\n");
}

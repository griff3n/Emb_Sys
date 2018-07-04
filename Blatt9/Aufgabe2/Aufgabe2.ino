// Dieser Code ist vorbereitet das Praktikum am Mittwoch
#include <Wire.h>

const byte ReadTemperature = 0xaa;  //AAh
const byte AccessTH = 0xa1;         //A1h
const byte AccessTL = 0xa2;         //A2h
const byte AccessConfig = 0xac;     //ACh
const byte ReadCounter = 0xa8;      //A8h
const byte ReadSlope = 0xa9;        //A9h
const byte StartConvertT = 0xee;    //EEh
const byte StopConvertT = 0x22;      //22h
const byte SlaveAdress = 144 >> 1;      //1001000
const byte Temperatur1_2 = 128;    //0,5 Grad Celsius
const float Temperatur = 28.5;

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

  Wire.requestFrom(SlaveAdress, 2);
  byte val = Wire.read();
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

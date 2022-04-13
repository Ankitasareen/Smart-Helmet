
#include <MPU6050_tockn.h>
#include <SoftwareSerial.h>
#define ledPin 13
#include <Wire.h>
#include <stdio.h>
int state = 0;
SoftwareSerial EEBlue(2, 3); // RX | TX
bool currently_detecting = true;


using namespace std;

MPU6050 mpu6050(Wire);

long timer = 0;

int g = 9.8;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  EEBlue.begin(9600);  //Default Baud for comm, it may be different for your Module. 
  Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

int check_crash(double acc, double rot_speed_i, double rot_speed_f)
{
    int detect = 0;
        if(acc>=1.02*g )
        {
            if(abs(rot_speed_f-rot_speed_i) >= 45)
            {
                Serial.print("crash detected");
                detect = 1;
                return detect;
               
            }
        } 
}
void loop() {

  if(currently_detecting){
     mpu6050.update();

  if(millis() - timer > 1000){
    
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
    Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
  
    Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
  
    Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(mpu6050.getAngleX());
    Serial.print("\tangleY : ");Serial.print(mpu6050.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(mpu6050.getAngleZ());
    Serial.println("=======================================================\n");

    double x_i = mpu6050.getAngleX();
    double y_i = mpu6050.getAngleY();
    double z_i = mpu6050.getAngleZ();
    delay(1000);
    double x_f = mpu6050.getAngleX();
    double y_f = mpu6050.getAngleY();
    double z_f = mpu6050.getAngleZ();

    
    int X = check_crash(mpu6050.getAccX(),x_i,x_f );
    int Y = check_crash(mpu6050.getAccY(),y_i,y_f);
    int Z = check_crash(mpu6050.getAccZ(),z_i,z_f);

   //if(X==1 || Y==1|| Z==1){
    if(true){
      Serial.print("Detected");
      EEBlue.write("1");
      delay(1000);
      EEBlue.write("0");
      currently_detecting = false;
    }
   
    timer = millis();
    
  }
  }
  
  }

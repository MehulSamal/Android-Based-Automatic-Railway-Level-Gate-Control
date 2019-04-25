#include <EEPROM.h>
#include <Servo.h>
#include <SoftwareSerial.h>


Servo myservo1;
Servo myservo2;
int TxD = 11;
int RxD = 10;
int msg;

int sensor1 = A0;
int sensor2 = A1;
int sensor3 = A2;
int sensor4 = A3;
int pos = 90;

int GLED1 = 8;
int RLED1 = 9;

int sensor5=2;

int buzzer = 12;
SoftwareSerial bluetooth(TxD, RxD);

void setup() {
  
  
  myservo1.attach(5);
  myservo2.attach(6);
  

  pinMode(sensor1,INPUT);
  pinMode(sensor2,INPUT);
  pinMode(sensor3,INPUT);
  pinMode(sensor4,INPUT);

  pinMode(buzzer,OUTPUT);

   pinMode(sensor5,INPUT);
 
 

  EEPROM.write(0, 0);
  EEPROM.write(1, 0);
myservo1.write(90);
  myservo2.write(90);
  Serial.begin(9600);       // start serial communication at 9600bps
  bluetooth.begin(9600);

 
}

void loop() {

  
/*----------------------------------------------Signalling System 1----------------------------------*/

if(digitalRead(sensor5)==LOW)      //Check the sensor output
 {
    digitalWrite(9, HIGH);   // set the Green LED on
    digitalWrite(8,LOW);
 
  }
 if(digitalRead(sensor5)==HIGH)
  { 
    
    digitalWrite(8, HIGH);    // set the Red LED on
    digitalWrite(9,LOW);
  }
 


/*--------------------------------------------Automatic Railway Level Gate Control--------------------*/
 if (EEPROM.read(1)==0 || EEPROM.read(0)==1){

    while (analogRead(sensor1)>500){
      if (analogRead(sensor2)>500){
        if (EEPROM.read(1)!=1){ EEPROM.write(1, 1);}
        if (EEPROM.read(0)!=1){ EEPROM.write(0, 1);} 
        
       // Serial.println("X");
        delay(100);       
        }}

    while (analogRead(sensor3)>500){
      if (analogRead(sensor4)>500){
        if (EEPROM.read(1)!=1){ EEPROM.write(1, 1);}
        if (EEPROM.read(0)!=1){ EEPROM.write(0, 1);} 
       // Serial.println("Y");
        delay(100); 
        }}   
  }

  if (EEPROM.read(1)==1 || EEPROM.read(0)==1){

    while (analogRead(sensor2)>500){
      if (analogRead(sensor1)>500){
        if (EEPROM.read(1)!=0){ EEPROM.write(1, 0);}
        if (EEPROM.read(0)!=1){ EEPROM.write(0, 1);}
        
        //Serial.println("Z"); 
        delay(100);
        }}

    while (analogRead(sensor4)>500){
      if (analogRead(sensor3)>500){
        if (EEPROM.read(1)!=0){ EEPROM.write(1, 0);}
        if (EEPROM.read(0)!=1){ EEPROM.write(0, 1);}
        //Serial.println("A"); 
        delay(100);
        }}   
  }

  if (EEPROM.read(1)==1){     //Gate open
    if (pos != 90){
      bluetooth.print("Gate Open");
      for (pos = 0; pos < 90; pos += 1){
      myservo1.write(pos);
      myservo2.write(pos);
      
     // Serial.println(pos);
      digitalWrite(buzzer,HIGH);
      delay(10);
      digitalWrite(buzzer,LOW);
      delay(10);
    }}
    
    //Serial.println("Gate Opend");
    digitalWrite(buzzer,LOW);
    if (EEPROM.read(0)!=0)  {EEPROM.write(0, 0);  /*Serial.println("OK");*/} 
  }
   
  if (EEPROM.read(1)==0){     //Gate close 
    if (pos != 0){
      bluetooth.print("Gate Close");
     for (pos = 90; pos > 0; pos -= 1){
         myservo1.write(pos);
         myservo2.write(pos);
         
         digitalWrite(buzzer,HIGH);
         delay(10);
         digitalWrite(buzzer,LOW);
         delay(10);
    }}
    
    //Serial.println("Gate Closed");    
    if (EEPROM.read(0)!=0)  {EEPROM.write(0, 0); Serial.println("OK");}
  }


/*----------------------------------------------------------------------------------------------------*/

if (bluetooth.available()){
    String value = bluetooth.readString();
    msg = value.toInt();


    switch (msg) { 
      case 1:
      open();
      break;

      
      case 2:
      close();
      break;

      
      
      
      default: 

      break; 
    }
    }
}

    
 void close() { // turns gate close 
 Serial.println(msg);
    myservo1.write(0);
    myservo2.write(0);
    {EEPROM.write(1, 0); Serial.println("OK");}
    bluetooth.print("Gate Close");
}




 void open() { // turns gate open
 Serial.println(msg);
    myservo1.write(90);
    myservo2.write(90);
    {EEPROM.write(0, 0); Serial.println("OK");}
    bluetooth.print("Gate Open");
}






  

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //SIM800L Tx & Rx is connected to Arduino #3 & #2
char phone_no[]="+254759757339"; 

const int trigPin = 8;
const int echoPin =9 ;
LiquidCrystal_I2C lcd(0x27,16,2);
float duration;
float distance_cm;
//led connection
int led = 4;
//buzzer connection
int buzzerPin = 6;
//Motor A connection
int enA = 5;
int in1 = 13;
int in2 = 7;
// Motor B connections
int enB =10 ;
int in3 =11 ;
int in4 =12 ;
int sensorThres=460;
int sensorValue;
//alcohol sensor connection
int alcoholsensor = A0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);

  
  lcd.init();//initialize the lcd
  lcd.clear();//clears the lcd screen
  lcd.backlight();// initialize the backlight

   pinMode(trigPin, OUTPUT);//sets lcd trigPin as output
   pinMode(echoPin, INPUT);//sets lcd echoPin as input
   pinMode(led, OUTPUT);//sets the led as output
   pinMode(buzzerPin, OUTPUT);//sets buzzerPin as output
   pinMode(enA, OUTPUT);
   pinMode(enB, OUTPUT);
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   pinMode(in3, OUTPUT);
   pinMode(in4, OUTPUT);
  
}
void SendMessage() 
{
  Serial.println("drunk driver");
  mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  delay(1000); // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+254704678058\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("DRIVER DRUNK");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void loop() 
{
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(alcoholsensor);// reads the analog value from MQ3
  sensorValue = analogRead(A0);// sends the value to UART for debugging
  Serial.println(sensorValue);
 if ( sensorValue < 460 )
  {
    // This function lets you control spinning direction of motors
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    // Turn on motor A & B
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    digitalWrite(buzzerPin, LOW);
  } 
 
 else
  {
    Serial.println ("drunk driver");
    SendMessage();
    // Turn off motors - Initial state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
    Serial.begin(9600);
    digitalWrite(buzzerPin, HIGH); 
  }
  
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    Serial.print("duration  ");
    Serial.println(duration); 
    distance_cm =0.034/2* duration;
    Serial.print("distance_cm ");
    Serial.println(distance_cm);
 if(distance_cm <=20&&distance_cm >=3)
  {
     lcd.setCursor(0,0);
     lcd.clear();
     lcd.print("SLOW DOWN");  
     
 
  }
 if(distance_cm <=3)
  {
     lcd.setCursor(0,0);
     lcd.clear();
     lcd.print("DANGER!! ");
     digitalWrite( led, HIGH);// led lights
     digitalWrite(buzzerPin, HIGH);// buzzer rings
 for (int i = 255; i >= 0; --i)
  {
     //decelerate to zero
     analogWrite(enA, i);
     analogWrite(enB, i);
     delay(20);
  }
  }
 if(distance_cm >=20)
  {
     lcd.setCursor(0,0);//lcd cursor at 0,0
     lcd.clear();// 
     lcd.print("SAFE     ");// SAFE is displayed on the lcd
      //accelerate from zero
 for (int i = 0; i < 256; i++) 
  {
     //accelerate from zero
     analogWrite(enA, i);
     analogWrite(enB, i);
     delay(20);
     digitalWrite(led,LOW);// led doesnt light
     digitalWrite(buzzerPin,LOW);// buzzer is silent
  }
  }
 }

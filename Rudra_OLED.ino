#include <SPI.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64); 
const int analogIn = A1;
int mVperAmp = 66; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;
extern unsigned char BigNumbers[];
int analogOutPin = 5; 
float voltage = 5.0;
int sensorPin = A0;  
float sensorValue = 0.0f;  
String voltageString ="0.0";
int stringLength = 0;
String str="Normal";
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; 
float R2 = 7500.0; 
float accuvolt=0.0;
float Volt=12.0;
int y=0;
void setup()   
{                
  Serial.begin(9600);
  //Use I2c scanner to find the address
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); 
  rtc.begin();
  //setRTCTime();
  display.drawPixel(10, 10, WHITE);
  display.display();
  delay(100);
  display.clearDisplay();
  while (!Serial) {
       delay(1);
  }
}
float readVoltage()
{
     int avera = 0;
     int avera1=0;
     for (int i=0; i < 15; i++) {
     avera = avera + analogRead(sensorPin);
     }
     avera = avera/15;
     vout = (avera * 5.0) / 1023.0; 
     vin = vout / (R2/(R1+R2));
     return vin;
}
float findamps(){
   RawValue = analogRead(analogIn);
   Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
   Amps = ((Voltage - ACSoffset) / mVperAmp);
   if(Amps<0)
     Amps=0;
   return Amps;
}
void loop() 
{
    DateTime now = rtc.now(); //get the current date-time
    Serial.print("Time->"); 
    Serial.print( now.hour() ,DEC);
    Serial.print(":"); 
    Serial.println( now.minute() ,DEC); 
    delay(50);
    
    Amps=findamps();
    
    //System will decide which mode it should go
    
    if(now.hour()==18&&now.minute()==10)
    Volt=readVoltage();

    //Assign mode name to str variable
    
    if(Volt>=12.0)
    str="Normal";
    else if(10.2<=Volt<12.0)
    str="ECO";
    else if(Volt<10.2)
    str="OFF";
    
    Serial.print("Battery Voltage : ");
    Serial.println(voltage);
    Serial.print("Operation Mode : ");
    Serial.println(str);  
    Serial.print("Charging Current : ");
    Serial.println(Amps);  
    
    if(9.9<voltage && voltage<=10.2)
    {
      y=13;
    }
    else if(10.2<voltage && voltage<=10.5)
    {
      y=22;
    }
    else if(10.5<voltage && voltage<=10.8)
    {
      y=39;
    }
    else if(10.8<voltage && voltage<=11.1)
    {
      y=53;
    }
    else if(11.1<voltage && voltage<=11.4)
    {
      y=62;
    }
    else if(11.4<voltage && voltage<=11.7)
    {
      y=74;
    }
    else if(11.7<voltage && voltage<=12.0)
    {
      y=84;
    }
    else if(12.0<voltage && voltage<=12.3)
    {
      y=94;
    }
    else if(12.3<voltage && voltage<=12.6)
    {
      y=100;
    }
     else if(12.6<voltage && voltage<=12.7)
    {
      y=100;
    }
    else 
    {
      y=-1;
    }  
    
    Serial.print("Battery Percentage - ");
    Serial.println(y);
    
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(12,1);
    display.println(y);
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(100, 0);
    display.println("%");
    display.display();
    delay(2500);
    display.clearDisplay();
    DrawMode();
    delay(1000);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(12, 0);
    display.println("A ");
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(50, 0);
    display.println(Amps);
    display.display();
    delay(1500);
    display.clearDisplay();

    //Assign brightness according to the mode and time frame
    
    if(Volt>=11.8)
    {
    Serial.println("Normal MODE");
    if(18<=now.hour()&&now.minute()>=15&&now.hour()<19)
   {
    display.clearDisplay();
    analogWrite(analogOutPin,150);
   }
    else if(19<=now.hour()&&now.hour()<20){
      display.clearDisplay();
      analogWrite(analogOutPin,200);
    }
    
    else if(20<=now.hour()&&now.hour()<=23){
     display.clearDisplay();
     analogWrite(analogOutPin, 90); 
    }
      
    else if (00<=now.hour()&&now.hour()<03){
    display.clearDisplay();
    analogWrite(analogOutPin, 90);  
    }

    else if(03<=now.hour()&&now.hour()<=04){
      display.clearDisplay();
      analogWrite(analogOutPin, 80);  
    }
   
    else if(05<=now.hour()&&now.hour()<06 && now.minute()<45){
      display.clearDisplay();
      analogWrite(analogOutPin, 150);      
    }
     
    else{
      display.clearDisplay();
      analogWrite(analogOutPin, 00); 
    }
    
    }
    //ECO mode
    else if(10.2<=Volt &&  Volt<11.8)
  {
    Serial.println("ECO MODE");
    if(18<=now.hour()&&now.minute()>15&&now.hour()<19)
    analogWrite(analogOutPin, 100);
    else if(19<=now.hour()&&now.hour()<20)
    analogWrite(analogOutPin, 55);
    else if(20<=now.hour()&&now.hour()<=23)
    analogWrite(analogOutPin, 35);  
    else if (00<=now.hour()&&now.hour()<03)
    analogWrite(analogOutPin, 25);  
    else if(03<=now.hour()&&now.hour()<=04)
    analogWrite(analogOutPin, 30);  
    else if(05<=now.hour()&&now.hour()<06 && now.minute()<30)
    analogWrite(analogOutPin, 80);  
    else{
    analogWrite(analogOutPin, 00); 
    } 
  }
  //OFFLINE mode
  else if(Volt<10.2)
  {
    Serial.println("OFFLINE MODE");
     //mode[25]="Normal";
    analogWrite(analogOutPin, 0 );
  }
  
   
}

void DrawMode(void) {

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(12, 1);
  display.println(str);
  display.display(); 
}
//function to set the time to RTC
void setRTCTime()
{
  DateTime dt(2019, 12, 20, 11, 07 , 45, 2); // Year, Month, Day, Hour, Minutes, Seconds, Day of Week
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}


#include "EmonLib.h" 
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(33, 41);

EnergyMonitor emon1;
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
 
//Sct sensor pin
int pin_sct =2;
int m;// initialise variable m
float n;//initialise variable n
int Power = 0;
void setup() 
{
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);   
  //Pin, Calibration - Cur Const = Ratio / BurdenR. 1800/62 = 29. 
  emon1.current(pin_sct,32);
  //Display starting inf
  lcd.setCursor(0,0);
  lcd.print("Curr:");
  lcd.setCursor(0,1);
  lcd.print("Pow.:");
  pinMode(7, OUTPUT);
  pinMode(A1,INPUT); // set pin a0 as input pin
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() 
{ 
  //Calculate current  
  double Irms = emon1.calcIrms(1480);
  //Shows the current value
  Serial.print("A : ");
  Serial.print(Irms); // Irms
  lcd.setCursor(0,0);
  lcd.print(Irms);

  m=analogRead(A0);// read analog values from pin A0 across capacitor
  n=(m* 0.21);
  if (n<200)
  {
    n=0;
  }
   //converts analog value(x) into input ac supply value using this formula ( explained in woeking section) 
   Serial.print("   analaog input  " ) ; // specify name to the corresponding value to be printed
   Serial.print(m) ; // print input analog value on serial monitor
   Serial.print("   ac voltage  ") ; // specify name to the corresponding value to be printed
   Serial.print(n) ; // prints the ac value on Serial monitor
   Serial.println();

   Power= 0+ Irms*n;
  
  // Calculates and displays the power value
  Serial.print(" P: ");
  Serial.println(Irms*n);


//  lcd.setCursor(0, 1);
//  lcd.print("V");
//  lcd.print(n);


  lcd.setCursor(0,0);
  lcd.print(Irms);
  lcd.print("A");

  lcd.setCursor(0,1);
  lcd.print(n);
  lcd.print("V");
  delay(300);

  lcd.setCursor(10,0);
  lcd.print(Irms*n);
  lcd.print("W");

if  (Irms*n >=200)
{
  
    digitalWrite(7,LOW);
     SendMessage();
     delay (1000);
     SendMessage1();
}

  else
  digitalWrite(7,HIGH);
delay (30000);
}

void SendMessage()

{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"Authority's Contact\"\r"); // Here the contact number of authority is to be provided
  

  delay(1000);

  mySerial.println("House No... Overloaded Transformer No...");// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z

  delay(1000);

}


void SendMessage1()
{

  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode

  delay(1000);  // Delay of 1000 milli seconds or 1 second

  mySerial.println("AT+CMGS=\"Consumer's contact\"\r"); // Here the contact number of the consumer is to be given
  

  delay(1000);

  mySerial.println("Dear Customer! Your meter is triffed off due to overloading Transformer. Do not reply, Computer generated SMS");// The SMS text you want to send

  delay(100);

  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


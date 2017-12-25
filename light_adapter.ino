#include <math.h>
int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
int buzzerPin = 5;
char val;

void setup() 
{ 
  pinMode (buzzerPin, OUTPUT);
  for (int i=2;i<=4;i++)
  pinMode(i, OUTPUT); //2 for Red LED, 3 for Yellow LED, 4 for Green LED
  Serial.begin(9600); 
}

void playTone(int tone, int duration)
{
  for (long i = 0; i < duration * 1000L; i += tone * 2) 
  {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(tone);
  }
}
  
void playNote(char note, int duration) 
{
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };// play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) 
  {
    if (names[i] == note) 
    {
      playTone(tones[i], duration);
    }
  }
 }


double getTemperature(int rawADC)
{
    double temp;
    temp = log(10000.0*((1024.0/rawADC-1)));
    temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    return temp - 273.15; 
}

void loop() 
{
   int tiltState = 0;
   int temp_sensorValue;
   double temperature;
   int light_sensorValue;
   tiltState = digitalRead (A1);
   temp_sensorValue = analogRead(A0);
   temperature = getTemperature(temp_sensorValue);
   Serial.println(temperature);
   light_sensorValue = analogRead(A2);
   delay(100);
 
   if ( temperature>43 )
   { 
    digitalWrite(2,HIGH);//red LED
    delay(100);
    for (int i = 0; i < length; i++) 
    {
      if (notes[i] == ' ') 
      {
        delay(beats[i] * tempo); // rest
      } 
      else 
      {
        playNote(notes[i], beats[i] * tempo); // pause between notes
      }

     delay(tempo / 2); 
    }                                                                       
   }
  else 
   {
    digitalWrite(2, LOW); // red LED turn off
   }
 
  if ( (temperature<44 && temperature> 10) ||  tiltState == LOW)
  {
    digitalWrite(3,HIGH);// yellow LED
    
    delay(1000);
    
  }

  if( light_sensorValue<600) 
    delay(1000);
  }
  else
  {
    digitalWrite(4,LOW);
   }

   
   if (Serial.available()) 
   { // If data is available to read,
     val = Serial.read(); // read it and store it in val
   }
   if (val == '1') 
   { // If 1 was received
     digitalWrite(4, HIGH); // turn the LED on
     delay(1000);
   } 
   else {
     digitalWrite(4, LOW); // otherwise turn it off
   }
   delay(10); // Wait 10 milliseconds for next reading
 

}

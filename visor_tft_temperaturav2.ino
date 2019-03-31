/*
Arduino Smart Estufa 



*/

#include "SPI.h"
#include "MQ7.h"
#include <TFT.h>  // Arduino LCD library


// Pins SCLK and MOSI are fixed in hardware, and pin 10 (or 53)
// must be an output
//#define sclk 13 // for MEGAs use pin 52
//#define mosi 11 // for MEGAs use pin 51
#define cs 10 // for MEGAs you probably want this to be pin 53
#define dc 9
#define rst 8 // you can also connect this to the Arduino reset

// pin definition for the Leonardo
// #define cs 7
// #define dc 0
// #define rst 1

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);





#include "DHT.h" //https://learn.adafruit.com/dht/downloads



#define DHTPIN 2 // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

// Connect pin + (middle) of the sensor to +5V
// Connect pin S (on the right) of the sensor to whatever your DHTPIN is
// Connect pin - (on the left) of the sensor to GROUND
// Connect 10k resistor between S and +



// Gas sensor
const int gasAOUTpin=0;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int gasDOUTpin=7;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino



int cycleTime = 2000;

DHT dht(DHTPIN, DHTTYPE);

float h;
float t;
float v;

// char array to print to the screen
char tempPrintout[6];
char humPrintout[6];
char coPrintout[6];
char vPrintout[6];


int gasLimit;
float gasVal;

MQ7 mq7(A0, 5.0);


float leerVoltaje(int pinSonda) {

//float escala = 100; //100 para voltios, 0.1 para milivoltios
  
float escala = 0.1;


  float lectura = analogRead(pinSonda);
 
  lectura = map(lectura, 0, 1023, 0, 500);
 
  float voltaje = lectura/escala;

  return voltaje;
}
void setup() {


// Put this line at the beginning of every sketch that uses the GLCD:
TFTscreen.begin();

// clear the screen with a black background
TFTscreen.background(0, 0, 0);

// write the static text to the screen
// set the font color to white
TFTscreen.stroke(255,255,255);
// set the font size
TFTscreen.setTextSize(1);
// write the text to the top left corner of the screen
TFTscreen.text("Temp (C)",0,0);
// write the text to the top left corner of the screen
TFTscreen.text("Humidade (%)",0,45);
TFTscreen.text("CO (ppm)",0,90);
// ste the font size very large for the loop
TFTscreen.setTextSize(3);

dht.begin();


//gas sensor
pinMode(gasDOUTpin, INPUT);

}



void loop() {

// Read the value of the temp/humidity sensor on D2

// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
h = dht.readHumidity();
t = dht.readTemperature();
//t = (t*1.8)+32; //C to F conversion
v = leerVoltaje(A3);

gasVal =  mq7.getPPM();
gasLimit= digitalRead(gasDOUTpin);//reads the digital value from the CO sensor's DOUT pin

 
String tempVal = doubleToString(t, 2);
String humVal = doubleToString(h, 0);
String coStr = doubleToString(gasVal,1);
String vStr = doubleToString(v,2);




// String sensorVal = String(1.234);

// convert the reading to a char array
tempVal.toCharArray(tempPrintout, 6);
humVal.toCharArray(humPrintout, 6);
coStr.toCharArray(coPrintout, 6);
vStr.toCharArray(vPrintout, 6);

// set the font color
TFTscreen.stroke(255,255,255);
// print the sensor value
TFTscreen.text(tempPrintout, 10, 15);
TFTscreen.text(humPrintout, 10, 60);
TFTscreen.text(coPrintout, 10, 105);
TFTscreen.text(vPrintout, 90, 105);
// wait for a moment
delay(cycleTime);
// erase the text you just wrote
TFTscreen.stroke(0,0,0);
TFTscreen.text(tempPrintout, 10, 15);
TFTscreen.text(humPrintout, 10, 60);
TFTscreen.text(coPrintout, 10, 105);

TFTscreen.text(vPrintout, 90, 105);

/*
 * ALARMA
 * 
 *  valor = digitalRead(2);
   if (analogRead(0) > 250){ // Capturo analogRead (0) que es el valor analogico del sensor
      digitalWrite(13, HIGH); // Enciendo LED
      digitalWrite(zumbador,HIGH); // Hago sonar zumbador
    
      delay (150); // Pausa
    
      digitalWrite(13, LOW); // Apago LED
      digitalWrite(zumbador, LOW); // Hago silenciar zumbador
    
      delay (150); // Pausa
   }

   */

   
}

//Rounds down (via intermediary integer conversion truncation)
String doubleToString(double input,int decimalPlaces){
if(decimalPlaces!=0){
String string = String((int)(input*pow(10,decimalPlaces)));
if(abs(input)<1){
if(input>0)
string = "0"+string;
else if(input<0)
string = string.substring(0,1)+"0"+string.substring(1);
}
return string.substring(0,string.length()-decimalPlaces)+"."+string.substring(string.length()-decimalPlaces);
}
else {
return String((int)input);
}
}

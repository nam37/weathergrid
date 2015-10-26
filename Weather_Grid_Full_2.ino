

//LED INCLUDES/VARIABLES/CONSTANTS
#include "SPI.h" 
const short int numLeds = 36; //number of apa102 pixels  
const byte bytesPerLed = 4; //set this to 4 its the number of bytes each pixel requires to work  
const short int frameLength = (numLeds+2)*bytesPerLed; //length of the total apa102 data frame  
byte leds[frameLength];  
  

//NETWORK INCLUDES/VARIABLES/CONSTANTS
#include <SPI.h>
#include <WiFi.h>
int status = WL_IDLE_STATUS;
WiFiServer server(83);


//LCD INCLUDES/VARIABLES/CONSTANTS
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;


void setup()  {  

//LED SETUP
  //set up SPI  
  SPI.begin();  
  SPI.setBitOrder(MSBFIRST);  
  SPI.setClockDivider(SPI_CLOCK_DIV2);  
  
   short int cnt=0;  
  //set the start frame to 4 x 0  
  leds[0] = 0;  
  leds[1] = 0;  
  leds[2] = 0;  
  leds[3] = 0;  
  //clear each pixel block  
  for(cnt=4; cnt<frameLength-4; cnt+=4)  
  {  
    leds[cnt] = 255; //set brightness for this pixel to max  
    leds[cnt+1] = 0; //set blue to 0  
    leds[cnt+2] = 0; //set green to 0  
    leds[cnt+3] = 0; //set red to 0  
  }  
  //set up the end frame to 4 x 255  
  leds[frameLength-4] = 255;  
  leds[frameLength-3] = 255;  
  leds[frameLength-2] = 255;  
  leds[frameLength-1] = 255;  

  clearLEDs();

//LCD SETUP
  // set up the LCD's number of columns and rows:
    lcd.setRGB(255,255,255);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("  INITIALIZING  ");
  lcd.setCursor(0, 1);
  lcd.print("  PLEASE  WAIT  ");

//NETWORK SETUP
  server.begin();    // start the web server on port 83

  delay(10000);    
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("FETCHING WEATHER");

} 


//////////////////////////////////////////////////////////
//START MAIN LOOP
//////////////////////////////////////////////////////////

void loop() {
  
  //LED LOOP SECTION
  short int cnt=0, LEDCounter=0;  
  //clearLEDs();
  //AllRedLEDs();



//NETWORK LOOP SECTION
  WiFiClient client = server.available();   // listen for incoming clients
 
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
 
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:  
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
 
            // the content of the HTTP response follows the header:
            //client.print("PASS WEATHER CODE AS http://[ADDRESS]:83/W024WW<br>");
 
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;       
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }   
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
 
        // Check to see if the client request has a weather code or a temperature code:

        if (currentLine.endsWith("WW")) {
        
          String CleanWeatherCode = currentLine;
          CleanWeatherCode = CleanWeatherCode.substring(CleanWeatherCode.length() - 5, CleanWeatherCode.length() - 2);

          if (CleanWeatherCode.startsWith("0")) {
            CleanWeatherCode = CleanWeatherCode.substring(1, 3);
          }
          if (CleanWeatherCode.startsWith("0")) {
            CleanWeatherCode = CleanWeatherCode.substring(1, 3);
          }

          lcd.setCursor(0, 0);
          lcd.print(CleanWeatherCode);
          //CleanWeatherCode.toInt();
          int FinalWeatherCode = CleanWeatherCode.toInt();
          SetWeather(FinalWeatherCode);
        }          


        if (currentLine.endsWith("ZZ")) {
        
          String CleanTEMP = currentLine;
          CleanTEMP = CleanTEMP.substring(CleanTEMP.length() - 6, CleanTEMP.length() - 2);
          
          if (CleanTEMP.startsWith("0")) {
            CleanTEMP = CleanTEMP.substring(1, 4);
          }
          if (CleanTEMP.startsWith("0")) {
            CleanTEMP = CleanTEMP.substring(1, 4);
          }
          
          lcd.setCursor(0, 1);
          lcd.print("    Temp: " + CleanTEMP + "   ");

        }     


      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

//FOR TESTING
//SetWeather(random(0, 47));
//delay(500);  
}

//////////////////////////////////////////////////////////
//START FUNCTIONS
//////////////////////////////////////////////////////////


void clearLEDs() {
  short int cnt=0, LEDCounter=0;  
  
  //clear all pixels  
  for(cnt=4; cnt<frameLength-4; cnt+=4)  
  {  
      leds[cnt+1] = 0;  
      leds[cnt+2] = 0;  
      leds[cnt+3] = 0;  
  }  
  //send the data to the strip  
  for(LEDCounter=0; LEDCounter<frameLength; LEDCounter++)  
  {  
   SPI.transfer( leds[LEDCounter] );  
  }   
  //wait 1 second  
  //delay(1000);    
  //wait .1 second  
  //delay(100);   
}

void setLED (int LEDnumber, char Color) {
  short int cnt=0, LEDCounter=0;  
 //set all pixels to blue  
  //for(cnt=4; cnt<frameLength-4; cnt+=4)  
  for(cnt=LEDnumber*4; cnt<(LEDnumber*4)+4; cnt+=4)  

  {  
    if (Color == 'R') {
      leds[cnt+1] = 0;  
      leds[cnt+2] = 0;  
      leds[cnt+3] = 255;  
    }
    if (Color == 'G') {
      leds[cnt+1] = 0;  
      leds[cnt+2] = 255;  
      leds[cnt+3] = 0;  
    }
    if (Color == 'B') {
      leds[cnt+1] = 255;  
      leds[cnt+2] = 0;  
      leds[cnt+3] = 0;  
    }
    if (Color == 'Y') {
      leds[cnt+1] = 0;  
      leds[cnt+2] = 255;  
      leds[cnt+3] = 255;  
    }
    if (Color == 'W') {
      leds[cnt+1] = 255;  
      leds[cnt+2] = 255;  
      leds[cnt+3] = 255;  
    }

  }  
  //send the data to the strip  
  for(LEDCounter=0; LEDCounter<frameLength; LEDCounter++)  
  {  
   SPI.transfer( leds[LEDCounter] );  
  }   
  //wait .1 second  
  //delay(100);  
}

void SetWeather (int WeatherCode) {
 
  if (WeatherCode==32 || WeatherCode==34) {
    //SUNNY CLEAR 1,4
    clearLEDs();
    setLED (10,'Y');
    setLED (11,'Y');
    lcd.setRGB(255,255,0);
    lcd.setCursor(0, 0);
    lcd.print("  SUNNY CLEAR   ");
  }
  else if (WeatherCode==31 || WeatherCode==33) {
   //MOON CLEAR 2,4
    clearLEDs();
    setLED (8,'W');
    setLED (9,'W');
    lcd.setRGB(255,255,255);
    lcd.setCursor(0, 0);
    lcd.print("    MOON CLEAR  ");
  }
  else if (WeatherCode==9 || WeatherCode==10 || WeatherCode==12 || WeatherCode==40) {
   //RAINY 3,4
    clearLEDs();
    setLED (6,'B');
    setLED (7,'B');
    lcd.setRGB(0,0,255);
    lcd.setCursor(0, 0);
    lcd.print("     RAINY      ");
  }
  else if (WeatherCode==26) {
   //CLOUDY 4,4
    clearLEDs();
    setLED (4,'G');
    setLED (5,'G');
    lcd.setRGB(0,255,0);
    lcd.setCursor(0, 0);
    lcd.print("     CLOUDY     ");
  }
  else if (WeatherCode==23 || WeatherCode==24) {
   //WINDY 1,3
    clearLEDs();
    setLED (15,'G');
    setLED (16,'G');
    lcd.setRGB(0,255,0);
    lcd.setCursor(0, 0);
    lcd.print("      WINDY     ");
  }
  else if (WeatherCode==0 || WeatherCode==1 || WeatherCode==2 || WeatherCode==3 || WeatherCode==4 || WeatherCode==17 || WeatherCode==35 || WeatherCode==37 || WeatherCode==38 || WeatherCode==39 || WeatherCode==45 || WeatherCode==47) {
   //LIGHTNING 2,3
    clearLEDs();
    setLED (17,'Y');
    setLED (18,'Y');
    lcd.setRGB(255,255,0);
    lcd.setCursor(0, 0);
    lcd.print("   LIGHTNING    ");
  }
  else if (WeatherCode==28 || WeatherCode==30 || WeatherCode==44) {
   //CLOUDY DAY 3,3
    clearLEDs();
    setLED (19,'B');
    setLED (20,'B');
    lcd.setRGB(0,0,255);
    lcd.setCursor(0, 0);
    lcd.print("   CLOUDY DAY   ");
  }
  else if (WeatherCode==27|| WeatherCode==29) {
   //CLOUDY NIGHT 4,3
    clearLEDs();
    setLED (21,'B');
    setLED (22,'B');
    lcd.setRGB(0,0,255);
    lcd.setCursor(0, 0);
    lcd.print("  CLOUDY NIGHT  ");
  }
  else if (WeatherCode==11 || WeatherCode==13 || WeatherCode==14 || WeatherCode==15 || WeatherCode==16 || WeatherCode==46) {
   //SNOW 1,2
    clearLEDs();
    setLED (30,'W');
    setLED (31,'W');
    lcd.setRGB(255,255,255);
    lcd.setCursor(0, 0);
    lcd.print("      SNOW      ");
  }
  else if (WeatherCode==5 || WeatherCode==6 || WeatherCode==7 || WeatherCode==8 || WeatherCode==18 || WeatherCode==41 || WeatherCode==42 || WeatherCode==43) {
   //SNOW/RAIN 2,2
    clearLEDs();
    setLED (28,'W');
    setLED (29,'W');
    lcd.setRGB(255,255,255);
    lcd.setCursor(0, 0);
    lcd.print("  SNOW & RAIN   ");
  }
  else if (WeatherCode==36) {
   //COLD 3,2
    clearLEDs();
    setLED (26,'B');
    setLED (27,'B');
    lcd.setRGB(0,0,255);
    lcd.setCursor(0, 0);
    lcd.print("      COLD      ");
  }
  else if (WeatherCode==25) {
   //HOT 4,2
    clearLEDs();
    setLED (24,'R');
    setLED (25,'R');
    lcd.setRGB(255,0,0);
    lcd.setCursor(0, 0);
    lcd.print("       HOT      ");
  }
  else if (WeatherCode==19 || WeatherCode==20 || WeatherCode==21 || WeatherCode==22) {
   //FOGGY 1,1
    clearLEDs();
    setLED (35,'W');
    setLED (36,'W');
    lcd.setRGB(255,255,255);
    lcd.setCursor(0, 0);
    lcd.print("      FOGGY     ");
  }
  
  else {
   //UNKNOWN WEATHER CODE
    clearLEDs();
    lcd.setRGB(255,0,0);
    lcd.setCursor(0, 0);
    lcd.print("    UNKNOWN     ");
  }
   
  
}

void AllRedLEDs () {
  //FUNCTION USED FOR TESTING
  short int cnt=0, LEDCounter=0;  
  //set all pixels to red  
  for(cnt=4; cnt<frameLength-4; cnt+=4)  
  {  
      leds[cnt+1] = 0;  
      leds[cnt+2] = 0;  
      leds[cnt+3] = 255;  
  }  
  //send the data to the strip  
  for(LEDCounter=0; LEDCounter<frameLength; LEDCounter++)  
  {  
   SPI.transfer( leds[LEDCounter] );  
  }   
  //wait 1 second  
  delay(1000);  
  
}

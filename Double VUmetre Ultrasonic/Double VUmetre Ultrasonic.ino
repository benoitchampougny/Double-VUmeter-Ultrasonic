
/* This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include "FastLED.h"
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 60
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
int pin1  = 6;
int pin2 = 5;
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, pin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, pin2, NEO_GRB + NEO_KHZ800);

// définition des broche  s utilisées
int trig = 12;
int echo = 11;
long lecture_echo;
long cm;
long cmControl;
int distanceMini = 30;
int distanceMaxi = 60;
int distanceSelect = 20;



int microPin = 0;

int valMicro;

int positionRightOld;
int positionLeftOld;
int positionSwitchOld;

int pointMiddleMicro = 345;
int percentMicro;
int countMode = 9;
int countLuminosity = 100;
int countVolume = 100;
int countPartSecondMini = 1;
int countPartSecondMaxi = 200;
int deltaPartSecond;
long speedMusic;
int sparking;
int cooling;

int arrayTram[NUM_LEDS];
int counterColorRed = 255;
int counterColorGreen = 255;
int counterColorBlue = 255;
byte byteCounterColorRed;
byte byteCounterColorGreen;
byte byteCounterColorBlue ;
bool colorRed = true;
bool colorGreen = false;
bool colorBlue = false;
bool colorWhite = false;
bool selectStrip1 = true;
bool selectStrip2 = true;
bool selectInverterStrip = false;
int orderStrip = 1;
int cursorSize = ((float)NUM_LEDS/100) * 10; 
bool selectUltrasonic;
bool selectUltrasonicOld;
bool selectUltrasonicSecond;
int selectUltrasonicPercent;
int counterVolume = 20;
int counterLuminosity = 20;
int counterMode = 3;
String menuSelect = "mode";
int menuSelectInt = 0;

long tempTimeTram;
long tempTimeFire;
long tempTimeMode9;
long tempTimeMode7;
long tempTimeRainbow;
long tempTimeSelect;
long tempTimeStrobe1;
long tempTimeStrobe2;
long tempTimeUltrasonic;
long tempTimeSparkle;
long tempTimeOrder;
long tempTimeStrip;

void setup() {
      Serial.begin(9600);
      pinMode(trig, OUTPUT);
      digitalWrite(trig, LOW);
      pinMode(echo, INPUT); 

      deltaPartSecond = countPartSecondMaxi - countPartSecondMini;
      strip1.begin();
      strip1.show();

      strip2.begin();
      strip2.show();
      
      long tempTimeTram = millis();
      long tempTimeFire = millis();
      long tempTimeMode9 = millis();
      long tempTimeMode7 = millis();
      long tempTimeRainbow = millis();
      long tempTimeStrobe1 = millis();
      long tempTimeStrobe2 = millis();
      long tempTimeSparkle = millis();
      long tempTimeOrder = millis();
      long tempTimeStrip = millis();
}

void loop(){
  
  
    
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    tempTimeUltrasonic = micros();
    digitalWrite(trig, LOW);
    lecture_echo = pulseIn(echo, HIGH);
    cmControl = lecture_echo / 58;
    Serial.print(lecture_echo);
    if (!(cmControl <= 4)){
      cm = cmControl;
    }
   
  valMicro = analogRead(microPin);

// Select Ultrasonic
  if (cm < distanceSelect){
    if ((millis() - tempTimeSelect) >= 50){
    selectUltrasonic = true;
    }
  }
  if (cm >= distanceSelect){
    tempTimeSelect = millis();
    selectUltrasonic = false;
  }

  selectUltrasonicSecond = false;
  if ((cm >= distanceMini) && (cm <= distanceMaxi)) {
    selectUltrasonicSecond = true;
    selectUltrasonicPercent = ((cm - distanceMini) * 100) / (distanceMaxi - distanceMini);
  }
  if (cm > distanceMaxi){
    selectUltrasonicPercent = 100;
  }



  
// Percent micro
  if (valMicro < pointMiddleMicro){
    valMicro = pointMiddleMicro - valMicro;
  }
   if (valMicro >= pointMiddleMicro){
    valMicro = valMicro - pointMiddleMicro;
  }
  
  percentMicro = (unsigned int)(valMicro*100) / (((float)counterVolume / 100) * pointMiddleMicro) ;
  if (percentMicro <= 0){
    percentMicro = 1;
  }
  if (percentMicro > 100){
    percentMicro = 100;
  }

// speed music
    speedMusic = 1000 / (countPartSecondMini + (deltaPartSecond * percentMicro) / 100);
// color convert

byteCounterColorRed = byte(counterColorRed);
byteCounterColorGreen = byte(counterColorGreen);
byteCounterColorBlue = byte(counterColorBlue);

// check color

if (selectStrip1) strip1.setBrightness((float)((255 * counterLuminosity) / 100));
if (selectStrip2) strip2.setBrightness((float)((255 * counterLuminosity) / 100));

  if (counterColorRed == 0){
    colorRed = false;
    }
  if (counterColorRed == 255){
    colorRed = true;
    }
  if (counterColorGreen == 0){
    colorGreen = false;
    }
  if (counterColorGreen == 255){
    colorGreen = true;
    }
  if (counterColorBlue == 0){
    colorBlue = false;
    }
  if (counterColorBlue == 255){
    colorBlue = true;
    }
  if ((counterColorRed == 255) && (counterColorBlue == 255) && (counterColorGreen == 255)){
    colorWhite = true;
    }
  if ((counterColorRed == 0)||(counterColorBlue == 0)||(counterColorGreen == 0)){
    colorWhite = false;
    }    


    if (((menuSelect == "color") && (selectUltrasonicSecond))||((counterMode == 7) && (delayDontStop(tempTimeMode7,20)))){
      if ((colorRed)&&(!colorGreen)&&(!colorBlue)) {
        counterColorBlue = counterColorBlue+5;
        }

      if ((colorRed)&&(colorBlue)&&(!colorGreen)) {
        counterColorRed = counterColorRed-5;
        }

      if ((colorBlue)&&(!colorRed)&&(!colorGreen)) {
        counterColorGreen = counterColorGreen+5;
        }

      if ((colorGreen)&&(colorBlue)&&(!colorRed)) {
        counterColorBlue = counterColorBlue-5;
        }

      if ((colorGreen)&&(!colorBlue)&&(!colorRed)) {
        counterColorRed = counterColorRed+5;
        }
      if ((colorGreen)&&(colorRed)&&(!colorBlue)&&(!colorWhite)) {
        counterColorBlue = counterColorBlue+5;
        }
      if ((colorGreen)&&(colorRed)&&(colorWhite)&&(colorBlue)) {
        counterColorGreen = counterColorGreen-5;
        }
      if ((colorGreen)&&(colorRed)&&(colorBlue)) {
        counterColorBlue = counterColorBlue-5;
        }
      }
      
    // Control luminosity
    if ((menuSelect == "luminosity") && (selectUltrasonicSecond)) {
        counterLuminosity = selectUltrasonicPercent;
    }
    if (counterLuminosity <=0) {
      counterLuminosity=5;
    }
    // Control mode
    if ((menuSelect == "mode") && (selectUltrasonicSecond)) {
        delay(1000);
        counterMode ++;
        setAll(0,0,0);
        showStrip(counterColorRed, counterColorGreen, counterColorBlue);
        if (counterMode > 13){
          counterMode = 0;
        }
      }
    // Control volume
    if ((menuSelect == "volume") && (selectUltrasonicSecond)) {
        counterVolume = selectUltrasonicPercent;
       }
      
    // Select strip
    if ((menuSelect == "select strip") && (selectUltrasonicSecond)){
      if (millis() - tempTimeStrip >= 1000){
           setAll(0,0,0);
           showStrip(counterColorRed, counterColorGreen, counterColorBlue);
           tempTimeStrip = millis();
           if ((selectStrip1) && (selectStrip2)) {
            selectStrip1 = false;
           }
           else if ((!selectStrip1) && (selectStrip2)) {
              selectStrip1 = true;
              selectStrip2 = false;
           }
           else if ((selectStrip1) && (!selectStrip2)) {
              selectStrip1 = true;
              selectStrip2 = true;
           }
       }
    }
    Serial.println(selectStrip1);
    Serial.println(selectStrip2);
    // Select order strip
    if ((menuSelect == "order strip") && (selectUltrasonicSecond) && (selectStrip1) && (selectStrip2)){
      if (millis() - tempTimeOrder >= 1000){
           setAll(0,0,0);
           showStrip(counterColorRed, counterColorGreen, counterColorBlue);
           if (orderStrip) orderStrip = 0;
           else orderStrip = 1;
           tempTimeOrder = millis();
      }
    }

    // select menu
    
    if (!selectUltrasonic) selectUltrasonicOld = false;
    Serial.println(menuSelect);

    if (selectUltrasonic) {

        if (selectUltrasonic != selectUltrasonicOld) {
          selectUltrasonicOld = selectUltrasonic;
          menuSelectInt++;
          if (menuSelectInt > 6){
            menuSelectInt = 0;
          }
          if (menuSelectInt == 0){
            menuSelect = "color";
          }
          if (menuSelectInt == 1) {
            menuSelect = "luminosity";
          }
          if (menuSelectInt == 2) {
            menuSelect = "mode";
          }
          if (menuSelectInt == 3) {
            menuSelect = "volume";
          }
          if (menuSelectInt == 4) {
            menuSelect = "off";
          }
          if (menuSelectInt == 5){
            menuSelect = "select strip";
          }
          if (((!selectStrip1) || (!selectStrip2)) && (menuSelectInt == 6)) {
            menuSelectInt = 0;
            menuSelect = "color";
          }
          if ((menuSelectInt == 6) && (selectStrip1) && (selectStrip2)){
            menuSelect = "order strip";
          }
          setAll(0,0,0);
          showStrip(counterColorRed, counterColorGreen, counterColorBlue);
          }
        }
         
// Affichage

if (menuSelect == "volume"){
  equalizer(counterVolume, 0, 255, 0);
}

if (menuSelect == "luminosity") {
          setAll(255, 255, 255);
          showStrip(counterColorRed, counterColorGreen, counterColorBlue);
}

if (menuSelect == "color") {
          setAll(byteCounterColorRed, byteCounterColorGreen, byteCounterColorBlue);
          showStrip(counterColorRed, counterColorGreen, counterColorBlue);
} 

if (menuSelect == "off") {
          setAll(0,0,0);
          showStrip(counterColorRed, counterColorGreen, counterColorBlue);
} 
             
if (!(menuSelect == "mode")){
  counterMode = 1;
  selectInverterStrip = false;
}
if (menuSelect == "mode"){
  if (counterMode == 0) {
    selectInverterStrip = false;
    rainbowCycle();
    }
  if (counterMode == 1) {
    Sparkle(byteCounterColorRed, byteCounterColorGreen, byteCounterColorBlue, speedMusic);
    }
  if (counterMode == 2) {
     if (percentMicro > 30){
     Strobe(byteCounterColorRed, byteCounterColorGreen, byteCounterColorBlue,5, 7);
     }
   }
  if (counterMode == 3) {
    sparking = 50 + ((15 * percentMicro) / 100 );
    cooling = 20 + ((8 * percentMicro) / 100 );
    Fire(cooling, sparking, 1) ;
    }
  if (counterMode == 4) {
    equalizer(percentMicro, counterColorRed, counterColorGreen, counterColorBlue);
    }
  if (counterMode == 5) {
    mirrorEqualizer(percentMicro, counterColorRed, counterColorGreen, counterColorBlue);
    }
  if (counterMode == 6) {
      tram( counterColorRed, counterColorGreen, counterColorBlue, percentMicro, 20);
    }
  if (counterMode == 7) {
      setAll(counterColorRed,counterColorGreen,counterColorBlue);
      showStrip(counterColorRed, counterColorGreen, counterColorBlue);
    }
  if (counterMode == 8) {
      for(int i = 0; i < NUM_LEDS; i++ ) {
            setPixel(i,random((((float)255/100) * percentMicro) , 255),random((((float)255/100) * percentMicro) , 255),random((((float)255/100) * percentMicro) , 255));
          } 
      showStrip(counterColorRed, counterColorGreen, counterColorBlue);  
   }
  if (counterMode == 9) {
      setAll(counterColorRed,counterColorGreen,counterColorBlue);
      showStrip(counterColorRed, counterColorGreen, counterColorBlue);
    }
  if (counterMode == 10) {
       if (selectStrip1) strip1.setBrightness((float)(255 * percentMicro) / 100);
       if (selectStrip2) strip2.setBrightness((float)(255 * percentMicro) / 100);
       
       setAll(counterColorRed,counterColorGreen,counterColorBlue);
       showStrip(counterColorRed, counterColorGreen, counterColorBlue);
      }
  if ((selectStrip1) && (selectStrip2)) {
    if (counterMode == 11) {
        selectInverterStrip = true;
        equalizer(percentMicro, counterColorRed, counterColorGreen, counterColorBlue);
      }
    if (counterMode == 12) {
        mirrorEqualizer(percentMicro, counterColorRed, counterColorGreen, counterColorBlue);
     }
    if (counterMode == 13) {
      tram( counterColorRed, counterColorGreen, counterColorBlue, percentMicro, 20);
    }
  }
  else counterMode = 0;
 }


// select strip

  if (menuSelect == "select strip") {
    setAll(0,0,0);
    if (selectStrip1){
      for(int i = 0; i < NUM_LEDS; i++ ) {
            strip1.setPixelColor(i, strip1.Color(0,0,random(255)));
          }      
    }
    if (selectStrip2){
      for(int i = 0; i < NUM_LEDS; i++ ) {
            strip2.setPixelColor(i, strip2.Color(0,0,random(255)));
          }      
    }
    showStrip(counterColorRed, counterColorGreen, counterColorBlue);
  }
   
  if (menuSelect == "order strip"){
    if (orderStrip) {
          for(int i = cursorSize; i < (cursorSize + cursorSize); i++ ) {
            strip2.setPixelColor(i, strip2.Color(0,0,255));
          }
          for(int i = (NUM_LEDS - cursorSize); i < (NUM_LEDS - (cursorSize +cursorSize)); i++ ) {
            strip2.setPixelColor(i, strip2.Color(0,0,255));
          }
          for(int i = ((NUM_LEDS/2) - (cursorSize/2)); i < ((NUM_LEDS/2) + (cursorSize/2)); i++ ) {
            strip1.setPixelColor(i, strip1.Color(0,0,255));
          }
    }
    if (!orderStrip){
          for(int i = cursorSize; i < (cursorSize + cursorSize); i++ ) {
            strip1.setPixelColor(i, strip1.Color(0,0,255));
          }
          for(int i = (NUM_LEDS - cursorSize); i < (NUM_LEDS - (cursorSize +cursorSize)); i++ ) {
            strip1.setPixelColor(i, strip1.Color(0,0,255));
          }
          for(int i = ((NUM_LEDS/2) - (cursorSize/2)); i < ((NUM_LEDS/2) + (cursorSize/2)); i++ ) {
            strip2.setPixelColor(i, strip2.Color(0,0,255));
          }
    }
    showStrip(counterColorRed, counterColorGreen, counterColorBlue);
  }

}
// Function mode

void rainbowCycle() {
  byte *c;
  uint16_t i, j;
  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
     for(i=0; i< NUM_LEDS; i++) {
        c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
        setPixel(i, *c, *(c+1), *(c+2));
      }
      showStrip(counterColorRed, counterColorGreen, counterColorBlue);
    }
 }

byte * Wheel(byte WheelPos) {
    static byte c[3];
    if(WheelPos < 85) {
      c[0]=WheelPos * 3;
      c[1]=255 - WheelPos * 3;
      c[2]=0;
    }
    else if(WheelPos < 170) {
      WheelPos -= 85;
      c[0]=255 - WheelPos * 3;
      c[1]=0; c[2]=WheelPos * 3;
    }
     else {
       WheelPos -= 170;
       c[0]=0;
       c[1]=WheelPos * 3;
       c[2]=255 - WheelPos * 3;
     }
  return c;
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  if (millis() - tempTimeSparkle >= SpeedDelay){
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip(counterColorRed, counterColorGreen, counterColorBlue); 
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
  }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay){
   for(int j = 0; j < StrobeCount; j++) {
     if (millis() - tempTimeStrobe1 >= FlashDelay){
      setAll(red,green,blue);
      showStrip(counterColorRed, counterColorGreen, counterColorBlue);
      if (millis() - tempTimeStrobe2 >= FlashDelay){
        setAll(0,0,0);
        showStrip(counterColorRed, counterColorGreen, counterColorBlue);
        }
      }
   }
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
  if (delayDontStop(tempTimeFire,SpeedDelay)){
  
  // Step 1. Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    if(cooldown>heat[i]) {
      heat[i]=0;
    }
    else {
      heat[i]=heat[i]-cooldown;
    }
  }

  // Step 2. Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3. Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
               }
    // Step 4. Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  showStrip(counterColorRed, counterColorGreen, counterColorBlue);
  }
}

void setPixelHeatColor(int Pixel, byte temperature) { // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191); // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {         // hottest
    setPixel(Pixel, 255, 255, heatramp);
  }
  else if( t192 > 0x40 ) {  // middle
    setPixel(Pixel, 255, heatramp, 0);
  }
  else {                   // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void equalizer(int percent, int counterColorRed, int counterColorGreen, int counterColorBlue) {
  int firstPixel;
  int i;
  setAll(0,0,0);
  firstPixel = (NUM_LEDS * percent) / 100;
  for(i=0; i< firstPixel; i++) {
    setPixel(i, counterColorRed, counterColorGreen, counterColorBlue);
  }
  showStrip(counterColorRed, counterColorGreen, counterColorBlue);
}

void mirrorEqualizer(int percent, int counterColorRed, int counterColorGreen, int counterColorBlue) {
  int firstPixel;
  int center = NUM_LEDS / 2;
  int Pixel = (center * percent) / 100;
  int i;
  setAll(0,0,0);
  for(i=0; i< Pixel; i++) {
    setPixel((center + i), counterColorRed, counterColorGreen, counterColorBlue);
    setPixel((center - i), counterColorRed, counterColorGreen, counterColorBlue);
  }
  showStrip(counterColorRed, counterColorGreen, counterColorBlue);
}

int* tram( int counterColorRed, int counterColorGreen, int counterColorBlueint,int percent, int percentParameter) {
  int arrayTramResult[NUM_LEDS];
  int i;
  for(i=0; i< NUM_LEDS; i++) {
        arrayTramResult[i] = arrayTram[(i - 1)];
        }
  if (percent >= percentParameter){
    arrayTramResult[0] = 1;
  }
    if (percent < percentParameter){
      arrayTramResult[0] = 0;
  }
  
  for(i=0; i< NUM_LEDS; i++) {
    arrayTram[i] = arrayTramResult[i];
    
    if (arrayTram[i] == 1) {
   
      setPixel(i, counterColorRed, counterColorGreen, counterColorBlue);
      }
    else {
      setPixel(i, 0, 0, 0);
      }
  }
  showStrip(counterColorRed, counterColorGreen, counterColorBlue);
}

// ---> here we define the effect function <---
// *** REPLACE TO HERE ***

void setPixel(int Pixel, byte red, byte green, byte blue) {
  if (!selectInverterStrip) {
    
 #ifdef ADAFRUIT_NEOPIXEL_H

   // NeoPixel

   if (selectStrip1) strip1.setPixelColor(Pixel, strip1.Color(red, green, blue));
   if (selectStrip2) strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));


 #endif
    
  }
  if (selectInverterStrip) {
    
 #ifdef ADAFRUIT_NEOPIXEL_H

   // NeoPixel
   if ((orderStrip) && (selectStrip1) && (selectStrip2)){
      strip1.setPixelColor(Pixel, strip1.Color(red, green, blue));
      strip2.setPixelColor(Pixel, strip2.Color(0, 0, 0));    
   }
   if (!(orderStrip) && (selectStrip1) && (selectStrip2)){
      strip1.setPixelColor(Pixel, strip1.Color(0, 0, 0));
      strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));    
   }


 #endif

 }

 #ifndef ADAFRUIT_NEOPIXEL_H

   // FastLED

   leds[Pixel].r = red;

   leds[Pixel].g = green;

   leds[Pixel].b = blue;

 #endif

}

void showStrip(byte red, byte green, byte blue) {

 #ifdef ADAFRUIT_NEOPIXEL_H

   // NeoPixel

   if (selectStrip1) strip1.show();
   if (selectStrip2) strip2.show();

 #endif

 #ifndef ADAFRUIT_NEOPIXEL_H

   // FastLED

   FastLED.show();

 #endif

 if ((orderStrip) && (selectInverterStrip)) { {
    for(int i = 0; i < NUM_LEDS; i++ ) {

    strip2.setPixelColor(i, strip2.Color(red, green, blue));

  }
 }

 if ((!orderStrip) && (selectInverterStrip)) {
    for(int i = 0; i < NUM_LEDS; i++ ) {

    strip1.setPixelColor(i, strip1.Color(red, green, blue));

  }
 }
 }
}

void setAll(byte red, byte green, byte blue) {

  for(int i = 0; i < NUM_LEDS; i++ ) {

    setPixel(i, red, green, blue);

  }

  showStrip(counterColorRed, counterColorGreen, counterColorBlue);

}

int delayDontStop(long memoryTime, int delayMicro){
  long nowTime = millis();
  long tempTime = nowTime - memoryTime;
  if (tempTime >= delayMicro){
    memoryTime = millis();
    return 1;
  }
  if (tempTime < delayMicro){
    return 0;
  }
  if (tempTime < 0){
    memoryTime = millis();
    return 0;
  }
}



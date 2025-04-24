#include "FastLED.h" // подключаем библиотеку фастлед

byte bright = 10; // яркость LED светодиодов
byte baza = 0;     // изменение оттенка LED
byte speed = 30;
uint16_t ibyte = 0;

#define NUM_LEDS 22
#define PIN 3
//CRGB leds[NUM_LEDS];
CRGB leds[366];

void setup() {
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(bright);
  Serial.begin(19200);
}

uint32_t base64(char x)
{
  if(x >= 0x30 && x <= 0x39){ //21042521
    return x + 0x04;
  } else if(x >= 0x41 && x <= 0x5a){
    return x - 0x41;
  } else if(x >= 0x61 && x <= 0x7a){
    return x - 0x47;
  } else if(x == 0x2b){
    return 62;
  } else if(x == 0x2f){
    return 63;
  }
  return 0;
}

void getByte(char a) {
  if(ibyte == 0){
    speed = a;
  }
  ibyte++;
  Serial.println((uint8_t)a);
}
void uart() {
  char s = ' ';
  uint8_t n = 0;
  uint32_t three_byte = 0;
  while(1) {
    if(Serial.available()) {
      s = Serial.read();
      if(s == '='){
        switch(n){
          case 1:
            getByte((three_byte>>16)%256); //third
            break;
          case 2:
            getByte((three_byte>>16)%256); //third
            getByte((three_byte>>8)%256); //second
            break;
          case 3:
            getByte((three_byte>>16)%256); //third
            getByte((three_byte>>8)%256); //second
            getByte(three_byte%256); //first
            break;
        }
        ibyte = 0;
        break; //выход из цикла
      }
      //Serial.print(s);
      
      //Serial.println(three_byte);
      //Serial.println(three_byte + (three_byte<<6));
      n++;
      switch(n){
        case 1:
          three_byte += base64(s)<<18;
          break;
        case 2:
          three_byte += base64(s)<<12;
          break;
        case 3:
          three_byte += base64(s)<<6;
          break;
        case 4:
          three_byte += base64(s);
        default:
          n = 0;
          //Serial.println(three_byte);
          getByte((three_byte>>16)%256); //first
          getByte((three_byte>>8)%256); //second
          getByte(three_byte%256); //third
          three_byte = 0;
      }
    }
  }
}

void loop() {
  if(Serial.available()){
    uart();
  }
  fadeToBlackBy(leds, NUM_LEDS, 1);
  int pos = beatsin16(speed, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(baza++, 255, 130);
  FastLED.setBrightness(bright);
  FastLED.show();
}
/*
void loop() {
    //fadeToBlackBy(leds, NUM_LEDS, 1);
    for(int i = 0; i < 16; i++)
      leds[i] = CRGB(255, 255, 200);
    FastLED.setBrightness(bright);
    FastLED.show();
}
*/
/*
void loop() {
    fadeToBlackBy(leds, NUM_LEDS, 2);
    int posR = random16(NUM_LEDS);
    int posG = random16(NUM_LEDS);
    int posB = random16(NUM_LEDS);
    int pos2 = random16(NUM_LEDS);
    leds[posR] = CRGB(255, 0, 0);
    leds[posG] = CRGB(0, 255, 0);
    leds[posB] = CRGB(0, 0, 255);
    leds[pos2] = CRGB(0, 0, 0);
    FastLED.setBrightness(bright);
    //delay(100);
    FastLED.show();
}
*/
/*
void loop() {
    fadeToBlackBy(leds, NUM_LEDS, 2);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(baza++, 255, 192);
    //int pos2 = beatsin16(13, 0, NUM_LEDS - 1);
    //leds[pos2] += CHSV(baza++, 255, 192);
    FastLED.setBrightness(bright);
    FastLED.show();
    //Serial.println(99);
    //delay(90);
}
*/

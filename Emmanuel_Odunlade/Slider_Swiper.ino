 /*
  * Author: Emmanuel Odunlade 
  * Complete Project Details http://randomnerdtutorials.com
  */
 
#include <HID.h>// include the library 

int swipe = 0;//slide
boolean left = false;
boolean right = false;
int maxD = 5;//(in cm) maximum distance from ultrasonic at which the obstruction will be considered a gesture
long int lastTouch = -1;
int resetAfter = 1000;//ms
int afterslideDelay = 500;//
int slideleft_Begin = -1;
int slideNone = 0;
int slideright_Begin =1;

//Declaring the connected pins
int lLed = 7;
int rLed = 6;
const int lEcho = 2;
const int lTrig = 3;
const int rEcho = 4;
const int rTrig = 5;

void setup(){
  pinMode(lLed,OUTPUT);
  pinMode(rLed,OUTPUT);
  pinMode(rEcho,INPUT);  
  pinMode(rTrig,OUTPUT);
  pinMode(lEcho,INPUT);
  pinMode(lTrig,OUTPUT);
  
  Serial.begin(9600);
    // Sends a clean report to the host. This is important because
  // the 16u2 of the Uno/Mega is not turned off while programming
  // so you want to start with a clean report to avoid strange bugs after reset.
  pressRawKeyboard(0, 0);
}

//get distance
//echo is input, trigger is output
unsigned long measureD(int input, int output){
  digitalWrite(output, HIGH);
  delayMicroseconds(10); 
  digitalWrite(output, LOW);
  long range= pulseIn(input, HIGH);
  int distance= range / 29 / 2;// to get distance in cm
  return distance;
}

//
boolean act (int input, int output, int led){
  int d = measureD(input,output);
  boolean pinActivated = false;
  if (d < maxD){
    digitalWrite(led,HIGH);
    pinActivated = true;
  }
  else{
    digitalWrite(led,LOW);
    pinActivated = false;
  }
  return pinActivated;
}

void slideNow(char directn){
  if ('R' == directn)
  {
    // press the right rrow key
    //Serial.println("F");
    pressRawKeyboard(0,RAW_KEYBOARD_RIGHT_ARROW); //modifiers + key
    pressRawKeyboard(0, 0); // release! Important
  }
  if ('L' == directn)
  {
    //press the left arrow key
    //Serial.println("B"); for debug
    pressRawKeyboard(0,RAW_KEYBOARD_LEFT_ARROW); // //modifiers + key
    pressRawKeyboard(0, 0); // release! Important
  }
  delay(afterslideDelay);
  swipe = slideNone;
}

void pressRawKeyboard(uint8_t modifiers, uint8_t key){
  uint8_t keys[8] = { 
    modifiers, 0, key, 0, 0, 0, 0, 0    }; //modifiers, reserved, key[0]
  HID_SendReport(HID_REPORTID_KeyboardReport, keys, sizeof(keys));
}


void loop(){
    left = act(lEcho,lTrig,lLed);
    right = act(rEcho,rTrig,rLed);
    if (left || right){
        lastTouch = millis();
    }
    if (millis() - lastTouch > resetAfter){
      swipe = 0;
      //serial.println(@reset slide& timer);
    }
    if (swipe >= slideNone){
      if ((left)&&(!right)){
        swipe = slideright_Begin;
      }
      if ((right)&&(swipe == slideright_Begin)){
        slideNow('R');
      }
    }
    if (swipe <= slideNone ){
      if ((right)&&(!left)){
        swipe = slideleft_Begin;
      }
      if ((left) && (swipe == slideleft_Begin)){
        slideNow('L');
      }
    }
    delay(50);
 }
 

#include <IRremote.h>
int RECV_PIN = 2; // Receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

int IR_LED = 3;
int INDICATOR_LED = 13;


int BUTTON_POWER = 1711591394, 
BUTTON_OPEN_CLOSE = 1711591372,
BUTTON_DISC_MENU = 1711591370,
BUTTON_REPEAT =1711591406,
BUTTON_REPEAT_2 = 1711591365,
BUTTON_USB = 1711591423,
BUTTON_RWD = 1711591374,
BUTTON_BOOKMARK = 1711591397,
BUTTON_VIDEO = 1711591377,
BUTTON_FWD = 1711591390,
BUTTON_FIRST = 1711591422,
BUTTON_STOP = 1711591377,
BUTTON_PLAY = 1711591361,
BUTTON_LAST = 1711591393,
BUTTON_MENU = 1711591412,
BUTTON_RETURN = 1711591402,
BUTTON_UP = 1711591418,
BUTTON_LEFT = 1711591382,
BUTTON_ENTER = 1711591414,
BUTTON_RIGHT = 1711591398,
BUTTON_DWN = 1711591366;


void setup(){
  Serial.begin(9600);
  pinMode(A5,INPUT);
  pinMode(IR_LED,OUTPUT);
  pinMode(INDICATOR_LED,OUTPUT); 
  irrecv.enableIRIn(); // Start the receiver
}

void loop(){

  if (irrecv.decode(&results)) {
    int val = results.value;
    Serial.println(val);
        Serial.println(results.decode_type);
        Serial.println(results.bits);
    if (val == BUTTON_ENTER){
      Serial.println("SHOOT BUTTON pressed");     
      for (int i=1;i<=50;i++){
        int freq = analogRead(A5);
        freq=constrain(freq,0,992);
        freq=map(freq,0,992,1,65);
        Serial.println(freq);   
        shutterNow(IR_LED, freq);
      }
    }
    irrecv.resume();
  }
}

void shutterNow(int _pin, int _freq){
  bool _seq[] = {
    0,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1        };
  high(8972,_freq,_pin);
  wait(4384);
  high(624,_freq,_pin);
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      wait(488);
      high(600,_freq,_pin);
    }
    else{
      wait(1600);
      high(600,_freq,_pin);
    }
  };
}

void wait(unsigned int time){
  unsigned long start = micros();
  while(micros()-start<=time){
  }
}

void high(unsigned int time, int freq, int pinLED){
  int pause = (1000/freq/2)-4;
  unsigned long start = micros();
  while(micros()-start<=time){
    digitalWrite(pinLED,HIGH);
    delayMicroseconds(pause);
    digitalWrite(pinLED,LOW);
    delayMicroseconds(pause);
  }
}






#include <multiCameraIrControl.h>

int IR_LED = 9;
int INDICATOR_LED = 13;
Olympus D5(IR_LED);

void setup(){
  pinMode(IR_LED,OUTPUT);
  pinMode(INDICATOR_LED,OUTPUT); 
}


void loop(){  
  digitalWrite(13,HIGH);
  for (unsigned int i=0;i<=20;i++){
    D5.shutterNow();
  }
  digitalWrite(13,LOW);
  delay(5000);  
}






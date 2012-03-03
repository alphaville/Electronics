#include <IRremote.h>
#include <multiCameraIrControl.h>




int IR_LED = 9;
int INDICATOR_LED = 13;
Olympus D5(IR_LED);

int RECV_PIN = 2; // Receiver
IRrecv irrecv(RECV_PIN);

void setup(){
  pinMode(IR_LED,OUTPUT);
  pinMode(INDICATOR_LED,OUTPUT); 
    irrecv.enableIRIn(); // Start the receiver
}


void loop(){  
  digitalWrite(13,HIGH);
  for (unsigned int i=0;i<=20;i++){
    D5.shutterNow();
  }
  digitalWrite(13,LOW);
  delay(5000);  
}






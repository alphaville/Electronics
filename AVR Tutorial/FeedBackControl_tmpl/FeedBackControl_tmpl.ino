#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define _FREE_RUNNING_MODE 100
#define _SIGNLE_CONVERSION_MODE 101
#define mode _FREE_RUNNING_MODE
#define PRESCALER 128
#define DEBUG_MODE false

volatile uint16_t myMeasurement = 0;

long intervalMicros = 600;
long previousMicros = 0;

/*
 * Set up the prescaler
 * Parse the parameter PRESCALER defined above
 */
void setupPrescaler(){
  switch (PRESCALER){
  case 64:
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1;
    break;
  case 32:
    ADCSRA |= 1<<ADPS2 | 1<<ADPS0;
    break;
  case 16:
    ADCSRA |= 1<<ADPS2;
    break;
  case 8:
    ADCSRA |= 1<<ADPS1 | 1<<ADPS0;
    break;
  case 4:
    ADCSRA |= 1<<ADPS1; 
    break;
  case 128:
  default:
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;
  }
}

// Main Method
void setup(void)
{
  DDRC &= ~(1<<DDC0); // Pin A0 as Input
  DDRD |=  (1<<PIND6) | (1<<PIND7);//Pins 6, 7 on arduino as output
  setupPrescaler();
  ADMUX |= (1<<REFS0) | (1<<REFS1); // Internal 1.1V reference
  if (mode==_FREE_RUNNING_MODE){
    ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));//ADC in free-running mode
    ADCSRA |= (1<<ADATE);//Signal source, in this case is the free-running
  }
  ADCSRA |= 1<<ADIE; // Enable the interrupt
  ADCSRA |= 1<<ADEN;// Enable the ADR
  sei();// Enable Interrupts (Global)

  ADCSRA |= 1<<ADSC;//Start first conversion (necessary always)  
  if (DEBUG_MODE){
    Serial.begin(9600);
  }
  //wait for configurations to take action... 
  for (unsigned int i=0;i<12;i++) {
    PORTD ^= 1<<PIND7;
    delay(80);
  }
}

void loop(){
  while(1){   
    unsigned long currentMillis = micros();
    noInterrupts();
        //Note: If the ADC interrupt writes to myMeasurement while we're reading, 
        //      we might read gibberish. e.g. if myMeasurement changes from 1234 to
        //      6789 we may come up with 6734 or 1289! This is why we need to protect
        //      this variable in a noInterrupts-interrupts-block.
        uint16_t currentState = myMeasurement;//  fix the variable - it changes all the time!
    interrupts();
    uint16_t controllerAction = calculateControllerAction(currentState);
    if (micros()-currentMillis>intervalMicros){//Time Exceeded already!!!
      intervalMicros = micros()-currentMillis;//update the intervalMicros...
    }
    while(currentMillis - previousMicros < intervalMicros) {//wait for the time to come...
      currentMillis = micros();
    }
    previousMicros = currentMillis;
    //analogWrite(6,map(controllerAction,0,2046,0,255));      //apply the controller's action
    PORTD ^= 1<<PIND6;//flip pin 6 on arduino
  }
}

// Method to calculate the controller's action
uint16_t calculateControllerAction(uint16_t currentState){
  uint8_t gain = 2;  
  return currentState*gain;
}

// ADC Interrupt method
// Note: The variables that are accessed from within an interrupt
//       should be declared as volatile!
ISR(ADC_vect)
{
  myMeasurement = ADC; //or what is the same: myMeasurement = ADCH<<8 | lowPart;
  if (mode==_SIGNLE_CONVERSION_MODE) { 
    ADCSRA |= 1<<ADSC; 
  }
}





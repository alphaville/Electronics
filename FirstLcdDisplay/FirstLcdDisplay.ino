/*
About:
  Mini Voltmeter. Using a moving average smoothing algorithm, output
  a voltage measurement to an LCD screen. At the same time, an interrupt
  caters for when a button is pressed. In that case, a message appears on
  the screen.

Circuit:
  Connect an LCD screen (2 lines x 16 characters) to pins 8-13.
  Connect the output of an inverted schmitt trigger to pin 2.
  Connect the output of an analog potentiometer to the analog pin A0.
  
Author:
  Pantelis Sopasakis
*/
#include <LiquidCrystal.h>
#include <RunningAverage.h>

// Constants
#define __interval  80
#define __interval_swap  80
#define __analogPin A0
#define __switchPin 2
#define __ma 50 // Moving average length
#define __headerText  "Mini Voltmeter  "

// Define the LCD screen object:
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Volatiles
volatile boolean flag = false;
volatile int samples = 0;

// Other Definitions
long previousMillis = 0, previousMillis_swap = 0;
unsigned long currentMillis = 0, currentMillis_swap = 0;
RunningAverage myRA(20); 


void setup() {
  Serial.begin(9600);// optional - for debugging only
  pinMode(__switchPin,INPUT);
  lcd.begin(16, 2);
  lcd.print(__headerText); 
  myRA.clr();
  attachInterrupt(0,swap,RISING);
}


void updateMeasurement(){
  int measurement = analogRead(__analogPin);
  myRA.add(measurement);
  samples++;
  float measurementAverage = myRA.avg();
  lcd.print("    ");
  lcd.setCursor(0,1);
  lcd.print(measurementAverage);
  lcd.print(" - ");

  lcd.print((double)map(constrain(measurementAverage,83,789),
  83,789,370,3530)/1000);
  lcd.print("V  ");
  if (samples == __ma)
  {
    samples = 0;
    myRA.clr();
  }
}

void swap(){  
  flag = !flag;
}

void loop() {
  currentMillis = millis();
  if(currentMillis - previousMillis > __interval) {
    previousMillis = currentMillis; 
    updateMeasurement();    
  }  
  currentMillis_swap = millis();
  if(currentMillis_swap - previousMillis_swap > __interval_swap) {
    lcd.setCursor(0,0);
    if (flag){
      lcd.print("P. Sopasakis");
    }
    else{
      lcd.print(__headerText);
    }
  }
}// END of file

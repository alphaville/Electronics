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
 
Dependencies:
 This project depends on the following libraries:
  * LiquidCrystal
  * RunningAverage
  * IRremote
Author:
 Pantelis Sopasakis
 */
#include <LiquidCrystal.h>
#include <RunningAverage.h>
#include <IRremote.h>

// Constants
#define __interval  120
#define __interval_swap  80
#define __analogPin A0
#define __switchPin 2
#define __switchPin2 3
#define __ma 250 // Moving average length
#define __headerText  "Mini Voltmeter  "

int RECV_PIN = 4; // Receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

// Codes
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
  pinMode(RECV_PIN,INPUT);
  pinMode(__switchPin,INPUT);
  pinMode(__switchPin2,INPUT);
  lcd.begin(16, 2);
  lcd.print(__headerText); 
  myRA.clr();
  attachInterrupt(0,swap,RISING);
  attachInterrupt(1,swap2,RISING);
  irrecv.enableIRIn(); // Start the receiver
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

/*
 Sends the current measurement through the serial port 
 back to the computer.
*/
void swap2(){  
  Serial.println(analogRead(A0));
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
  ir();

}// END of file

void ir(){
  if (irrecv.decode(&results)) {
    Serial.print("IR Value :");
    int val = results.value;
    Serial.println(val);           
    if (val == BUTTON_ENTER){
      swap();
    }
    irrecv.resume(); // Receive the next value
  }
}

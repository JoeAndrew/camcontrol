//SKETCH FOR MULTI-CAMERA REMOTE CONTROLLER
//Operates up to three cameras - cam1, cam2, cam3
//Delay between cameras can be left at 0, or adjusted in milliseconds

//Edited 20120609 to make initial delay set in 10-sec instead of 1-sec increments

//set up the OLED
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13

#include <SSD1306.h>

SSD1306 oled(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//INTERRUPT HANDLER
volatile int state = LOW;  //changed by interrupt

//PIN ASSIGNMENTS

//aux outputs for flash triggering etc.
int aux1pin = 0;
int aux2pin = 1;

// triggering switch connects to pin 2; interrupt 0 is on this pin

//ENCODER SETUP
//pins for encoder

int dpInEncoderA=3;
int dpInEncoderB=4;
int dpInEncoderPress=5;

int pulsesPerClick = 1;  //how many pulses do we get each time we click the knob
long encoderValue = 0;   //where we will store the value the encoder adjusts
int tempPulse = 0;  //variable for storing pulses, so we can account for multiple pulses
int currentValue = 0;  //current value we read from the encoder

//PINS TO WHICH CAMERA JACKS ARE CONNECTED
int cam1pin = 6;
int cam2pin = 7;
int cam3pin = 8;

//for string conversion
int runCount = 1;
char modeValString[]="000";
char runCountString[]="000";
char runMaxString[]="000";

// MENU SETUP

//mode names for menus
char* modeName[]={
  "# of trips: ", "Wait before (sec): ",
  "Wait between (sec): ", "Delay 1 (ms): ", "Delay 2 (ms): ",
  "Delay 3 (ms): "};

int modeVal[6]={
  1,0,0,0,0,0};                         //array in which we'll store the values

int mode = 0;                                       //initialize mode value; changed by changeMode fn
int maxModes = 5;   //get count of array elements
int modeChange = 1;                                //initialize change value; changed by encoder


void setup()
{

  // configure the pins
  _ResetPins();                                  //on ReadEncoder tab

  // init serial communication
  //Serial.begin(9600); 
  //Serial.println("Ready to begin");
  
  //attach the interrupt for triggering
  attachInterrupt(0, trigger, RISING);
  
  //test the OLED
    oled.ssd1306_init(SSD1306_SWITCHCAPVCC);

  // init done
  
  oled.display(); // show splashscreen
  delay(2000);
  oled.clear();   // clears the screen and buffer

  // Fill screen
  oled.fillrect(0, 0, SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, WHITE);
  oled.display();
  delay(2000);
  oled.clear();   // clears the screen and buffer
}

void loop()
{

  if (state == HIGH)                            //state goes HIGH when triggered by interrupt
  {
    //Serial.println("TRIPPED!");
    oled.clear();
    oled.drawstring(0,0, "TRIPPED!");
    oled.display();
    
    //wait for the initial delay
        int runMax = modeVal[0];
        int theSecs = modeVal[1];
        int runCount = 1;
    while(theSecs > 0) {
      oled.clear();
      itoa (runCount,runCountString, 10);
      itoa (runMax, runMaxString, 10);
      oled.drawstring(0,0, "Run ");
      oled.drawstring(25,0, runCountString);
      oled.drawstring(50,0, "of ");
      oled.drawstring(70,0, runMaxString);
      oled.drawstring(0,2, "starts in ");
      char theSecsString[]="000";
      itoa (theSecs, theSecsString, 10);
      oled.drawstring(65,2, theSecsString);
      oled.display();
      theSecs--;
      delay(1000);  //may need tweaking based on display refresh
    }
    runTriggers();
    
    //now complete the rest of the run
    //only if run count was not set to 1
    if (!(modeVal[0]==1)){
    runSequence(modeVal[0]);
    }
    state = LOW;
    oled.clear();
    oled.drawstring(0,0, "Done");
    oled.display();
    //Serial.println("Done");
    //set the state back to LOW when finished
  }
  else {                                        //this is what we do while waiting for trigger

    oled.clear();
    oled.drawstring(0,0, modeName[mode]);
    //convert number to string
    itoa (modeVal[mode], modeValString, 10);
    oled.drawstring(0,2, modeValString);
    oled.display();
    ReadEncoder();
  }
}

//THE INTERRUPT HANDLER
void trigger()
{
  state = HIGH;
}






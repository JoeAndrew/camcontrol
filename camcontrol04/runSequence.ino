//////////////////////////////////////////
//  FUNCTIONS FOR RUNNING THE SEQUENCE  //
//////////////////////////////////////////

void runSequence(int runMax) {
  int runCount = 2;
  do {
    int theSecs = modeVal[2];
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
    runCount++;
  }
  while (!(runCount == (runMax+1)));  //needed to make last run start
}

//triggers the cams
//change variable names as needed
void runTriggers() {
  delay(modeVal[3]);
  digitalWrite(cam1pin,HIGH);
  delay(modeVal[4]);
  digitalWrite(cam2pin,HIGH);
  delay(modeVal[5]);
  digitalWrite(cam3pin,HIGH);
  //reset the pins
  delay(100);                    //arbitrary value to give the camera time to respond
  digitalWrite(cam1pin,LOW);
  digitalWrite(cam2pin,LOW);
  digitalWrite(cam3pin,LOW);
}







//Routine Driven by Interrupt, trap 1 second interrupts, and output every minute
/*
ISR(TIMER1_COMPA_vect) {
  seconds++;
  mseconds++;
  if (seconds == 60) { //make 60 for each output
    seconds = 0;
    Serial.println("5 second ...");
    Serial.println(mseconds);
   // usbData();//comment this out to temporarily disable data every minute for debug
    WebServer();
  }
  if (mseconds == 250) { //make 60 for each output
    mseconds = 0;
    Serial.print("5 Min ...");
    LogSD();
  }


  
} //end of interrupt routine
*/

// Main routines, find header, then sync in with it, get a packet, and decode data in it, plus report any errors.
void loop() {

 EthernetClient client = server.available();
  tempBit = polarity ^ 1;
  noErrors = true;
  firstZero = false;
  headerHits = 0;
  nosBits = 0;
  maxBytes = 15; //too big for any known OS signal
  nosBytes = 0;
  discNos = discards;
  manchester[0] = 0;
  digitalWrite(ledPin, 0);
  while (noErrors && (nosBytes < maxBytes)) {
    while (digitalRead(RxPin) != tempBit) {
    }
    delayMicroseconds(sDelay);
    if (digitalRead(RxPin) != tempBit) {
      noErrors = false;
    }
    else {
      byte bitState = tempBit ^ polarity;
      delayMicroseconds(lDelay);
      if (digitalRead(RxPin) == tempBit) {
        tempBit = tempBit ^ 1;
      }
      if (bitState == 1) {
        if (!firstZero) {
          headerHits++;
          if (headerHits == headerBits) {
            //digitalWrite(ledPin,1);
          }
        }
        else {
          add(bitState);
        }
      }
      else {
        if (headerHits < headerBits) {
          noErrors = false;
        }
        else {
          if ((!firstZero) && (headerHits >= headerBits)) {
            firstZero = true;
            digitalWrite(ledPin, 1);

          }
          add(bitState);
        }
      }
    }
  }
  digitalWrite(ledPin, 0);


  if (timer0_overflow_count > 9000){ //check every 9 seconds or so
    WebServer();
    LogSD();
    timer0_overflow_count = 0;
  }
 



}

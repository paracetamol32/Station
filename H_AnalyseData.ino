void analyseData() {
  tmElements_t tm;
RTC.read(tm);

  if (manchester[0] == 0xaf) { //detected the Thermometer and Hygrometer (every 53seconds)
    scan = scan | 1;
    logTemp = 0; //reset missing reads to zero
    thermom();
    dumpThermom();
    digitalWrite(RedPin, 1);     //Temperature is Red
    digitalWrite(GrePin, 0);
    digitalWrite(BluPin, 0);

  }
  if (manchester[0] == 0xa1) { //detected the Anemometer and Wind Direction (every 14seconds)
    scan = scan | 2;
    logWind = 0;//reset missing reads to zero
    anemom();
    dumpAnemom();
    digitalWrite(RedPin, 0);
    digitalWrite(GrePin, 1);    //Wind is Green
    digitalWrite(BluPin, 0);
  }
  if (manchester[0] == 0xa2) { //detected the Rain Gauge (every 47seconds)
    scan = scan | 4;
    logRain = 0;//reset missing reads to zero
    rain();
    dumpRain();
    digitalWrite(RedPin, 0);
    digitalWrite(GrePin, 0);
    digitalWrite(BluPin, 1);   //Rain is Blue
  }
  if (manchester[0] == 0xad) { //detected the UV Light Sensor (every 73seconds)
    scan = scan | 8; //not checked at the moment
    logUV = 0;//reset missing reads to zero
    UV();
    //dumpUV();
    digitalWrite(RedPin, 1);
    digitalWrite(GrePin, 1);
    digitalWrite(BluPin, 0);   //UV is Yellow
  }
  if (manchester[0] == 0xa3) { //detected an original Sensor designed by Us every 45 seconds!!!
    //totally experimental
    scan = scan | 8;  //not checked at the moment
    logExp = 0; //reset missing reads to zero
   // totExp();
    //This code is not used by the three sensors in the WMR86 product. It may clash with other
    //other OS Sensors and a different value chosen in the Tx and then Rx
    digitalWrite(RedPin, 1);
    digitalWrite(GrePin, 0);
    digitalWrite(BluPin, 1); //Experimental is Purple!!! or Magenta???

  }
  //Serial.println(scan,DEC);
  eraseManchester();
    
}

void add(byte bitData) {
  if (discNos > 0) {
    discNos--;//discard bits before real data
  }
  else {
    //the incoming bitstream has bytes placed in reversed nibble order on the fly, then the CS is done.
    if (bitData) {
      //if it is a '1' OR it in, others leave at a '0'
      manchester[nosBytes] |= oregon[nosBits];//places the reversed low nibble, with hi nibble, on the fly!!!
    }
    //Oregon Scientific sensors have specific packet lengths
    //Maximum bytes for each sensor must set once the sensor has been detected.
    if (manchester[0] == 0xA2) {
      maxBytes = 11; //rain
      csIndex = 19;
    }
    if (manchester[0] == 0xA1) {
      maxBytes = 10; //wind
      csIndex = 18;
    }
    if (manchester[0] == 0xAF) {
      maxBytes = 9; //temp
      csIndex = 16;
    }
    if (manchester[0] == 0xAD) {
      maxBytes = 8; //UV Light Detector
      csIndex = 14; //CS byte begins at 14 nibble
    }
    if (manchester[0] == 0xA3) {
      maxBytes = 10; //experimental, Solar,Strikes,UV
      csIndex = 18; //CS byte begins at 18 nibble
    }
    nosBits++;
    //Pack the bits into 8bit bytes
    if (nosBits == 8) {
      nosBits = 0;
      nosBytes++;
      manchester[nosBytes] = 0; //next byte to 0 to accumulate data
    }
    //Check the bytes for a valid packet once maxBytes received
    if (nosBytes == maxBytes) {
      //hexBinDump();
      digitalWrite(ledPin, 1);
      //Check Checksum first
      if (ValidCS(csIndex)) {
        //Process the byte array into Human readable numbers
        analyseData();
      }
      noErrors = false; //make it begin again from the start
    }
  }
}



void setup() {
  Serial.begin(115200);
  Serial.println("Start Eth");
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println ("Go!");
  pinMode(RxPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(GrePin, OUTPUT);
  pinMode(BluPin, OUTPUT);
  pinMode(Ground, OUTPUT);

  pinMode(Gnd3, OUTPUT);
  digitalWrite(Ground, 0);
  digitalWrite(Gnd3, 0);
  pinMode(VccPin, OUTPUT);
  //  pinMode(Ground, OUTPUT);
  pinMode(Gnd2, OUTPUT);
  pinMode(Gnd, OUTPUT);
  digitalWrite(Gnd2, 0);
  digitalWrite(Gnd, 0);
  digitalWrite(VccPin, 1);


  //  if (!sensor7021.begin()) {
  //    Serial.println("Did not find Si7021 sensor!");
  //    while (true);
  //  }



  //Start SD card
  pinMode(53, OUTPUT);         //53 for Mega
  pinMode(SS, OUTPUT);
  // digitalWrite(53, HIGH);
  root.close();
  file.close();
  if (!card.init(SPI_HALF_SPEED, chipSelect)) ;

  // if (!root.openRoot(&volume)) digitalWrite(LedSD, HIGH);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }

  //Setup Arrays for 24/28 hour trends
  for (int z = 0; z < 2 ; z++) {
    for (int x = 0; x < 24; x++) {
      for (int y = 0 ; y < 14; y++) {
        Trends [y] [x] [z] = -999;
      }
    }
  }

  //Get Yearly Extremes
  EEPROM_readAnything(0, YearData);


  Wire.begin();
  setSyncProvider(RTC.get);
  tmElements_t tm;
  RTC.read(tm);
  PreviousTime = now();
  timeClient.begin();


  // make the RGB LED flash a signature heartbeat on start up
  digitalWrite(RedPin, 0);
  digitalWrite(GrePin, 1);
  digitalWrite(BluPin, 1);  //Flash Cyan
  delay(1000);
  digitalWrite(RedPin, 1);
  digitalWrite(GrePin, 1);  //Flash Yellow
  digitalWrite(BluPin, 0);
  delay(1000);
  digitalWrite(RedPin, 1);
  digitalWrite(GrePin, 0);
  digitalWrite(BluPin, 1);  //Flash Purple
  delay(1000);
  digitalWrite(BluPin, 0);  //all off
  digitalWrite(RedPin, 0);
  delay(1000);




  /*
     //  Enable these if attempting to debug the program or the circuit
     Serial.println("Debug Manchester Version 18");
     Serial.print("Using a delay of 1/4 bitWaveform ");
     Serial.print(sDelay,DEC);
     Serial.print(" uSecs 1/2 bitWaveform ");
     Serial.print(lDelay,DEC);
     Serial.println(" uSecs ");
     if (polarity){
     Serial.println("Negative Polarity hi->lo=1");
     }
     else{
     Serial.println("Positive Polarity lo->hi=1");
     }
     Serial.print(headerBits,DEC);
     Serial.println(" bits expected for a valid header");
     if (discards){
     Serial.print(discards,DEC);
     Serial.println(" leading bits discarded from Packet");
     }
     else{
     Serial.println("All bits inside the Packet");
     }
     Serial.println("D 00 00001111 01 22223333 02 44445555 03 66667777 04 88889999 05 AAAABBBB 06 CCCCDDDD 07 EEEEFFFF 08 00001111 09 22223333 0A 44445555");


    //Tutorial on using the BMP05 Press/Temp transducer https://www.sparkfun.com/tutorials/253
    //  bmp.begin();    //start the barometer and temp packages
  */
  /*
    // Initialize Timer1 for a 1 second interrupt
    // Thanks to http://www.engblaze.com/ for this section, see their Interrupt Tutorial
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    // set compare match register to desired timer count:
    OCR1A = 15624;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();
  */

}

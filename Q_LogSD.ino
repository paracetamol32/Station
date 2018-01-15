void LogSD() {
   WinNewFlag = true;
  formatTimeDigits(LogMonth, month());
  formatTimeDigits(LogDay, day());
  formatTimeDigits(LogHour, hour());
  formatTimeDigits(LogMinute, minute());

  //  formatTimeDigits(LogMonth, month(now));
  //  formatTimeDigits(LogDay, day(now));
  //  formatTimeDigits(LogHour, hour(now));
  //  formatTimeDigits(LogMinute, minute(now));

//  Serial.println("LogSD Log..............");
//  float minnow = minute();
//  int minint = minnow / 5;
//  float mindec = (minnow / 5);
//  Serial.println(minnow);


// Serial.println(mindec);
  //if ((mindec - minint != 0)  && year() != 2000) {
     if (minute() <= 5  && year() != 2000) {
    if (WriteFlag == true) {
    //  Serial.println("Writing Log..............");
      WriteFlag = false;
      char filename[] = "L"; //file prefix
      monat = month(); // convert byte to int
      yeaat = year();
      itoa(monat, charbuffermonat, 10);
      itoa(yeaat, charbufferyeaat, 10);
      strcat(filename, charbufferyeaat); //append the month to the prefix
      strcat(filename, charbuffermonat); //append the month to the prefix
      strcat(filename, ".csv");   //append the file ending
      File logfile = SD.open (filename, FILE_WRITE);
      if (! logfile) {
        Serial.println("error opening datalog.txt");
        // Wait forever since we cant write data
        while (1) ;
      }
      // logfile.open(&root, filename, O_CREAT | O_APPEND | O_WRITE);
      logfile.print(LogDay);
      logfile.print("/");
      logfile.print(LogMonth);
      logfile.print("/");
      logfile.print(year());
      logfile.print(" ");
      logfile.print(LogHour);
      logfile.print(":");
      logfile.print(LogMinute);
      logfile.print(", ");
      logfile.print(outTemp);
      logfile.print(", ");
      logfile.print(intHum);
      logfile.print(", ");
      logfile.print(intTemp);
      logfile.print(", ");
      logfile.print(avWindspeed);  //m/s
      logfile.print(", ");
      logfile.print(quadrant);  //m/s
      logfile.print(", ");
      logfile.print(gustWindspeed);
      logfile.print(", ");
      logfile.print(MaxGust24);
      logfile.print(", ");
      logfile.print(rainTotal);
      logfile.print(", ");
      logfile.print(rainRate);
      logfile.print(", ");

      logfile.print(TotalRainFrom0000);
      logfile.print(", ");
      logfile.println(TotalRainHour);
      logfile.flush();
      logfile.close();

      //update trends on the hour
      if (hour() != 0) {
        Trends [0] [hour()] [0] = OutTempNow;
        Trends [1] [hour()] [0] = OutHumNow;
        Trends [2] [hour()] [0] = UVNow;
        Trends [3] [hour()] [0] = GustNow;
        Trends [4] [hour()] [0] = AverageNow;
        Trends [5] [hour()] [0] = DirectionNow;
        Trends [6] [hour()] [0] = BarrometerNow;
        Trends [7] [hour()] [0] = InTempNow;

        Trends [11] [hour()] [0] = TotalRainHour;
        Trends [12] [hour()] [0] = PowerNow;

      }

      if (hour() == 0) {
        //If midnight, copy data from this 24hrs to previous and clear data
        for (int x = 0; x < 24; x++) {
          for (int y = 0 ; y < 14; y++) {
            //move all data from this 24hrs to 'yesterday'
            Trends [y] [x] [1] = Trends [y] [x] [0];
            Trends [y] [x] [0] = -999;
          }
        }

        //add in midnight
        Trends [0] [0] [0] = OutTempNow;
        Trends [1] [0] [0] = OutHumNow;
        Trends [2] [0] [0] = UVNow;
        Trends [3] [0] [0] = GustNow;
        Trends [4] [0] [0] = AverageNow;
        Trends [5] [0] [0] = DirectionNow;
        Trends [6] [0] [0] = BarrometerNow;
        Trends [7] [0] [0] = InTempNow;

        Trends [11] [0] [0] = TotalRainHour;


        //UpdateYearlyExtremes
        EEPROM_writeAnything(0, YearData);

        //Reset 24hr Extrememes
        MaxTemp24 = OutTempNow;
        MinTemp24 = OutTempNow;
        MaxGust24 = GustNow;
        Direction24 = DirectionNow;
        MaxPower24 = PowerNow;
        MaxUV24 = UVNow;
        TotalRainFrom0000 = 0;
        PreviousTime = now();

        if (day() == 1 && month() == 1) { //1st January reset years data
          YearData.MaxTempY = OutTempNow;
          YearData.MaxTempYD = now();
          YearData.MinTempY = OutTempNow;
          YearData.MinTempYD = now();
          YearData.MaxGustY = GustNow;
          YearData.DirectionY = DirectionNow;
          YearData.WindYD = now();
          YearData.TotalPowerY = 0.0;
          YearData.TotalRainY = 0;
          EEPROM_writeAnything(0, YearData);
        }
      }
      //reset hours totals

      TotalRainHour = 0;
    }
  }
  else
  {
    //reset flag to allow write to SD at next sync interval
    WriteFlag = true;
  }


}


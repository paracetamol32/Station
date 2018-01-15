/*   PCR800 Rain Gauge  Sample Data:
  //  0        1        2        3        4        5        6        7        8        9        A
  //  A2       91       40       50       93       39       33       31       10       08       02
  //  0   1    2   3    4   5    6   7    8   9    10  11   12  13   14  15   16  17   18  19   20  21
  //  10100010 10010001 01000000 00100100 00000000 00000000 00000010 00100001 00010100 00000000 00100001
  //  -------- -------  bbbb---  RRRRRRRR 88889999 AAAABBBB CCCCDDDD EEEEFFFF 00001111 2222CCCC cccc

  // byte(0)_byte(1) = Sensor ID?????
  // bbbb = Battery indicator??? (7)  My investigations on the anemometer would disagree here. (However check above for Xander's contribution).
  // After exhaustive low battery tests these bbbb bits did not change
  // RRRRRRRR = Rolling Code Byte
  // 222211110000.FFFFEEEEDDDD = Total Rain Fall (inches)
  // CCCCBBBB.AAAA99998888 = Current Rain Rate (inches per hour)
  // ccccCCCC = 1 byte Checksum cf. sum of nybbles
  // Message length is 20 nybbles so working in inches
  Three tips caused the following
  1 tip=0.04 inches or 1.1mm (observed off the LCD)
  My experiment
  Personally I don't like this value. I think mult by 25 and divide by 1000 IS closer to return mm directly.
  0.127mm per tip??? It looks close the above. Again can't vouch 100% for this, any rigorous assistance would be appreciated.
  Over the last 6 months this has proved to be Ok
*/
void rain() {
  rainTotal = float(((nyb(18) * 1000000) + (nyb(17) * 100000) + (nyb(16) * 10000) + (nyb(15) * 1000) + (nyb(14) * 100) + (nyb(13)*10)) * 25.4 / 40000);
//  Serial.print (" Rain: ");
//  Serial.println (float(((nyb(18) * 1000000) + (nyb(17) * 100000) + (nyb(16) * 10000) + (nyb(15) * 1000) + (nyb(14) * 100) + (nyb(13)*10)) * 25.4 / 40000));
//  Serial.print (" Rain2: ");
//  Serial.println (float( ((nyb(18) * 1000000) + (nyb(17) * 100000) + (nyb(16) * 10000) + (nyb(15) * 1000) + (nyb(14) * 100) + (nyb(13)*10)))* 25.4/10000);
  rainRate = float(((nyb(8) * 10000) + (nyb(9) * 1000) + (nyb(10) * 100) + (nyb(11) * 10) + nyb(12)) * 25.4 / 4000.0);
  //Serial.println((nyb(8)*10000)+(nyb(9)*1000)+(nyb(10)*100)+(nyb(11)*10)+nyb(12),DEC);
  RainTime = now();
    //hexBinDump();

      Serial.print(" Total Rain ");
//    Serial.print(rainTotal);
  //   Serial.print(" OldTotal Rain ");
  //  Serial.print(OldRainTotal);
  if (rainTotal != OldRainTotal) {
    if (RainNewFlag == false) { //Stops 1st reading going through and giving additonal values
      TotalRainFrom0000 += 0.254;
      TotalRainHour += 0.254;
      YearData.TotalRainY += 0.254;

    }
    OldRainTotal = rainTotal;
    RainNewFlag = false;
  }
}


void dumpRain() {


  //  Serial.print(" mm, ");
  //  Serial.print("Rain Rate ");
  //  Serial.print(rainRate);
  //  Serial.print(" TotalRainFrom0000 ");
  //  Serial.print(TotalRainFrom0000);
  //  Serial.print(" TotalRainHour   ");
  //  Serial.print(TotalRainHour);
  //  Serial.println(" mm/hr ");
}

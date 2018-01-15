
// WGR800 Wind speed sensor
// Sample Data:
// 0        1        2        3        4        5        6        7        8        9
// A1       98       40       8E       00       0C       70       04       00       34
// 0   1    2   3    4   5    6   7    8   9    10  11   12  13   14  15   16  17   2   3
// 10100001 10011000 01000000 10001110 00000000 00001100 01110000 00000100 00000000 00110100
// -------- -------- bbbb---- NRRRRRRR xxxx9999 xxxxxxxx CCCCDDDD xxxxFFFF 0000---- CCCCcccc
// Av Speed 0.4000000000m/s Gusts 0.7000000000m/s  Direction: N

// byte(0)_byte(1) = Sensor ID?????
// bbbb = Battery indicator??? (7)  My investigations would disagree here.  After exhaustive low battery tests these bits did not change
// NRRRRRRR = Rolling Code Byte, the N bit is set to 1 for 64 cycles to indicate it is reset or new to the Rx box
// 9999 = Direction
// DDDD.CCCC = Gust Speed (m per sec)
// 0000.FFFF = Avg Speed(m per sec)
// multiply by 3600/1000 for km/hr
// ccccCCCC = 1 byte checksum cf. sum of nybbles
// packet length is 20 nybbles

void anemom() {
  //D A1 98 40 8E 08 0C 60 04 00 A4
  avWindspeed = (double)( ((nyb(17) * 100) + ((nyb(16) * 10) + nyb(15))) * 3.6 ) / 10;
  wspeed =      (double)((((nyb(13) * 10) + nyb(12) + (nyb(14) * 100))) * 3.6) / 10;


  // after every minute, reset gustWindspeed to avWindspeed and then take the highest gust after that (approx4 readings a minute)



  WindTime = now();
  if (wspeed > MaxGust24 && wspeed != -999) {
    MaxGust24 = wspeed;
    Direction24 = DirectionNow;
    //Serial.print("MaxGust today ");
    //Serial.println(MaxGust24);
  }
  if (MaxGust24 > YearData.MaxGustY && wspeed != -999) {
    YearData.MaxGustY = MaxGust24;
    YearData.DirectionY = Direction24;
    YearData.WindYD = now();
    //Serial.print("MaxGust this Year ");
    //Serial.println(YearData.MaxGustY);
  }

  if (WinNewFlag == false) { //Stops 1st reading going through and giving additonal values

    if (avWindspeed < OldavWindspeed) {
      avWindspeed = OldavWindspeed;

    }
    if ( wspeed < Oldwspeed) {
      wspeed = Oldwspeed;

    }

  }

  if (avWindspeed > wspeed) {
    wspeed = avWindspeed;

  }
  OldavWindspeed = avWindspeed;
  Oldwspeed = wspeed;

  quadrant = nyb(9) & 0xF;
}

void dumpAnemom() {
  // Serial.print (now());
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print(" Av Speed ");
  Serial.print(avWindspeed);
  Serial.print(" km/h, Gusts ");
  Serial.print (wspeed);
  Serial.print(" km/h, Av Speed ");
  Serial.print(double(avWindspeed ) / 3.6);
  Serial.print(" m/s, Gusts ");
  Serial.print (double(wspeed ) / 3.6);
  Serial.print(" m/s, Direction: ");
  Serial.print(quadrant);
  Serial.print(" -> ");
  Serial.println(windDir[quadrant]);

}

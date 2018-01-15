// Formating routine for interface to host computer, output once a minute once all three sesnors have been detected
void usbData() {

  // Stn Id, Packet Type, Wind Quadrant, Wind Speed, Rain Tips, Ext temp, Int Temp, Int Pressure, Int Humidity
 // Serial.print ("send usb data");
//  intHumi = ((double)sensor7021.readHumidity());      //DHT22 readings %Humidity
//  intTemp = ((double)sensor7021.readTemperature());
  Serial.print("Humidity:    "); Serial.print(intHum);
  Serial.print("\tTemperature: "); Serial.println(temperature);
  
  //internal temperature
//  intTemp = 0;
//  intHumi = 0;
  //intPres = (double)bmp.readPressure() / 100.0; //Pa reduced to mBar
  //leave this check at 7 until the other readings are stabilized
  if ((scan & 7) == 7) {           //scan==15 means all 4 readings now have a valid value, ready for output on Serial
    //Battery/Signal status, OR in the the four status values for the signal connections.
    logTemp++;
    if (logTemp > 40) {
      batStat = batStat | 1;
    }
    logWind++;
    if (logWind > 40) {
      batStat = batStat | 2;
    }
    logRain++;
    if (logRain > 40) {
      batStat = batStat | 4;
    }
    logUV++;
    if (logUV > 40) {
      batStat = batStat | 8;
    }
    logExp++;
    if (logExp > 40) {
      batStat = batStat | 16;
    }
    //reset the batStat to 0 every 24hours
    aday++;
    if (aday > 1440) {
      batStat = 0;
      aday = 0;
    }

    // Order: Battery Status, Quadrant, Wind Gust, Rainfall, Temperature, InternalTemp, Internal Pressure, Int Humidity, Solar Power, Lightning, UV Radiation
    Serial.print(batStat, DEC);      //Send out the number to indicate if a sensor is not transmitting for maore than 20 mins. Low Battery or other damage.
    Serial.print(",");
    Serial.print(quadrant);          //0-15 in 22.5 degrees steps clockwise
    Serial.print(",");
    Serial.print(gustWindspeed, 1);  //Gust windspeed km/hr, not average windspeed (graphing over 10 samples gives Average)
    Serial.print(",");
    gustWindspeed = avWindspeed;     //reset gust to average, then take the larger next reading
    Serial.print(rainTotal, 1);      //currently considered to checked for a good calibration to mm
    Serial.print(",");
    Serial.print(temperature, 2);    // OS Temperature Centigrade
    Serial.print(",");
    Serial.print(intTemp, 2);        //BMP085 temperature (used for compensation reading) Centigrade
    Serial.print(",");
    Serial.print(intPres, 2);        //BMP085 pressure reading milli-bars
    Serial.print(",");
    Serial.print(intHum);           //Digital DHT22 seems better than the OS in Temp/Hum sensor % relative
    Serial.print(",");
    Serial.print(intSolar, DEC);      //A reading from experimental Solar Power sensor
    Serial.print(",");
    Serial.print(intLightning, DEC);      //A reading from the nos of Lightning Strikes
    Serial.print(",");
    Serial.print(intUV, DEC);      //UV Sensor
    Serial.println();

    //Indicate the transmission of this one second data with a white light set up R+B+G!!
    digitalWrite(RedPin, 1);         //White, all colours on, data has been logged to the Server
    digitalWrite(GrePin, 1);
    digitalWrite(BluPin, 1);
  }
}

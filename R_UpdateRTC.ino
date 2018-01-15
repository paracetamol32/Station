void UpdateRTC(){
  NTPClient timeClient(ntpUDP, "78.46.189.152", 3600);
timeClient.update();
  delay(1000);
  timeClient.update();
 
    Serial.print(year(timeClient.getEpochTime()));
    Serial.print(" ");
    Serial.print(month(timeClient.getEpochTime()));
    Serial.print(" ");
    Serial.print(day(timeClient.getEpochTime()));
    Serial.print(" ");
    Serial.print(hour(timeClient.getEpochTime()));
    Serial.print(" ");
    Serial.print(minute(timeClient.getEpochTime()));
    Serial.println(" ");

setTime(hour(timeClient.getEpochTime()),minute(timeClient.getEpochTime()),0,day(timeClient.getEpochTime()),month(timeClient.getEpochTime()),year(timeClient.getEpochTime()));
RTC.set(now());

 


}

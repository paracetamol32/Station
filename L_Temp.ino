
// THGN800 Temperature and Humidity Sensor
// 0        1        2        3        4        5        6        7        8        9          Bytes
// 0   1    2   3    4   5    6   7    8   9    A   B    C   D    E   F    0   1    2   3      nybbles
// 01011111 00010100 01000001 01000000 10001100 10000000 00001100 10100000 10110100 01111001   Bits
// -------- -------- bbbbcccc RRRRRRRR 88889999 AAAABBBB SSSSDDDD EEEE---- CCCCcccc --------   Explanation
// byte(0)_byte(1) = Sensor ID?????
// bbbb = Battery indicator??? (7), My investigations on the anemometer would disagree here.  After exhaustive low battery tests these bits did not change
// RRRRRRRR = Rolling code byte
// nybble(5) is channel selector c (Switch on the sensor to allocate it a number)
// BBBBAAAA.99998888 Temperature in BCD
// SSSS sign for negative (- is !=0)
// EEEEDDDD Humidity in BCD
// ccccCCCC 1 byte checksum cf. sum of nybbles
// Packet length is 18 nybbles and indeterminate after that
// H 00 01 02 03 04 05 06 07 08 09    Byte Sequence
// D AF 82 41 CB 89 42 00 48 85 55    Real example
// Temperature 24.9799995422 degC Humidity 40.0000000000 % rel
void thermom() {
  temperature = (double)((nyb(11) * 100) + (nyb(10) * 10) + nyb(9)) / 10; //accuracy to 0.1 degree seems unlikely
  channel = nyb(5);
  if (nyb(12) == 1) { //  Trigger a negative temperature
    temperature = -1.0 * temperature;

  }
  humidity = (nyb(14) * 10) + nyb(13);
if (channel == 1){
  outTemp = temperature;
  outHum = humidity;
}
if (channel == 5){
 Temp5 = temperature;
 Hum5 = humidity;
}

if (channel == 6){
 Temp6 = temperature;
 Hum6 = humidity;
}
}
void dumpThermom() {
  Serial.print("Temperature ");
  Serial.print(temperature);
  Serial.print(" degC, Humidity ");
  Serial.print(humidity);
  Serial.print("% Rel ");
  Serial.print("Ch:");
  Serial.println(channel);
}

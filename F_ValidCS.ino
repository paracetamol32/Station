bool ValidCS(int CSPos) {
  boolean ok = false;
  byte cs = 0;
  for (int x = 1; x < CSPos; x++) {
    byte test = nyb(x);
    cs += test;
  }
  //do it by nybbles as some CS's cross the byte boundaries eg rainfall
  byte check1 = nyb(CSPos);
  byte check2 = nyb(CSPos + 1);
  byte check = (check2 << 4) + check1;
//  /*
    if (manchester[0]==0xA2){
//    Serial.print(check1,HEX);  //dump out the LSNybble Checksum
//    Serial.print("(LSB), ");
//    Serial.print(check2,HEX);  //dump out the MSNybble Checksum
//    Serial.print("(MSB), ");
//    Serial.print(check,HEX);   //dump out the Rx'ed predicted byte Checksum
//    Serial.print("(combined), calculated = ");
//    Serial.println(cs,HEX);    //dump out the calculated byte Checksum
    //Serial.print("   ");     //Space it out for the next printout
    }
 // */
  if (cs == check) {
    ok = true;
  }
  return ok;
}

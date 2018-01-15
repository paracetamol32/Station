
//UVN800 UV Light Sensor
// D AD 10101101 87 10000111 41 01000001 AB 10101011 00 00000000 00 00000000 70 01110000 D3 11010011
// D AD 10101101 87 10000111 41 01000001 AB 10101011 00 00000000 0A 00001010 70 01110000 74 01110100

void UV() {
  //maximum readings appear ot be about 130-140 units (not sure how they rate them)
  intUV = int(((nyb(12) - 7) * 16) + nyb(11));
}
void dumpUV() {
  Serial.print("UV Level ");
  Serial.println(intUV);
}



void hexBinDump() {
  //Serial.println("T A3 10100011 07 00000111 02 00000010 AA 10101010 F0 11110000 06 00000110 FF 11111111 07 00000111 33 00110011 60 01100000");
  Serial.print("D ");
  for ( int i = 0; i < maxBytes; i++) {
    byte mask = B10000000;
    if (manchester[i] < 16) {
      Serial.print("0");
    }
    Serial.print(manchester[i], HEX);
    Serial.print(" ");
    for (int k = 0; k < 8; k++) {
      if (manchester[i] & mask) {
        Serial.print("1");
      }
      else {
        Serial.print("0");
      }
      mask = mask >> 1;
    }
    Serial.print(" ");
  }
  Serial.println();
}

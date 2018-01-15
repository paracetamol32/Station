byte nyb(int nybble) {
  int bite = nybble / 2;       //DIV 2, find the byte
  int nybb  = nybble % 2;      //MOD 2  0=MSB 1=LSB
  byte b = manchester[bite];
  if (nybb == 0) {
    b = (byte)((byte)(b) >> 4);
  }
  else {
    b = (byte)((byte)(b) & (byte)(0xf));
  }
  return b;
}


void getBitField(long long *wordp, long long *bitmaskp, long long *bitshiftp, long long *bitfieldp) {
  *bitfieldp = ((unsigned long long)(*wordp & *bitmaskp)) >> *bitshiftp;
}

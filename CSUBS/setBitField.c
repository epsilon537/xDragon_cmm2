
void setBitField(long long *wordp, long long *bitmaskp, long long *bitshiftp, long long *bitfieldp) {
  *wordp &= ~(*bitmaskp);
  *wordp |= (*bitfieldp << *bitshiftp) & *bitmaskp;
}

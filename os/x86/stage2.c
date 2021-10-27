void _boot() {
  int *p= (int *)0x10000;
  p[0] = 1;
  return;
}
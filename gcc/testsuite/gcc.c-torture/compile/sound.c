/* { dg-require-stack-size "8192+4" } */

main ()
{
  char audio[8192];
  int i;

  for (i = 0;  i < 4095;  i += 1)
    audio[i] = i / 8,
    audio[8191 - i] = i / 8;

  for (;;)
    write (1, audio, 8192);
}

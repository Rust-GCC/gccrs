/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */
/* { dg-additional-options "-O3 -fno-ipa-cp-clone" } */

void intf_pcmPlayEffect(int *src, int *dst, int size) {
 int i;
 for (i = 0; i < size; i++)
  *dst++ = *src & 0x80 ? (*src++ & 0x7f) : -*src++;
}


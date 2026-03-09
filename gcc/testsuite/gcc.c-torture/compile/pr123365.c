/* PR c/123365 */

void
foo ()
{
  __asm__ volatile ("" : "+r" ((long long[]) { 0 }));
}

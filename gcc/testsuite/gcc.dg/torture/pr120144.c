/* PR target/120144 */
/* { dg-do compile } */
/* { dg-require-effective-target int128 } */

int f(unsigned __int128 t)
{
  return __builtin_clzg(t);
}

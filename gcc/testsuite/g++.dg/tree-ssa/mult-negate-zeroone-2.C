// Verify { 0, 1 } * -{ 0, 1 } == -({ 0, 1 } & { 0, 1 }) at runtime, including
// the commutative (negated operand first) form.
// { dg-do run }
// { dg-options "-O2" }

int __attribute__((noipa)) f (int x, int y) { return (x > 5) * -(y > 5); }
int __attribute__((noipa)) g (int x, int y) { return -(y > 5) * (x > 5); }

int main () {
  for (int x = -3; x < 12; x++)
    for (int y = -3; y < 12; y++)
      {
	if (f (x, y) != -((x > 5) & (y > 5))) __builtin_abort ();
	if (g (x, y) != -((x > 5) & (y > 5))) __builtin_abort ();
      }
  return 0;
}

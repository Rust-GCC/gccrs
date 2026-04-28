/* PR tree-optimization/125039 */

void
foo ()
{
  double b = b, c = c;
  if (c)
    c = -c;
  double d = b == c ? b : c;
  if (c != d)
    foo ();
}

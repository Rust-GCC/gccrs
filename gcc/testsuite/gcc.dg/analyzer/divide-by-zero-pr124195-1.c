short s;

int
foo()
{
  s %= (0, 0); /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" } */
  return s > 0;
}

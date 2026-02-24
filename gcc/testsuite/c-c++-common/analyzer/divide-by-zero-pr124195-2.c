short s;

int
foo()
{
  s %= 0; /* { dg-warning "division by zero \\\[-Wdiv-by-zero\\\]" } */
  /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" "" { target *-*-* } .-1 } */
  return s > 0;
}

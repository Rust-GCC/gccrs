unsigned m;
int c;

void
foo()
{
  do c %= (5ull << 40) & m; /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" } */
  while (c);
}

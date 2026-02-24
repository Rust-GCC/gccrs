float
test_1 ()
{
  return 42.f / 0.f; /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" } */
}

static float __attribute__((noinline))
get_zero ()
{
  return 0.f;
}

float
test_2 ()
{
  return 42.f / get_zero (); /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" } */
}

float
test_3 (float x)
{
  return x / get_zero (); /* { dg-warning "division by zero \\\[-Wanalyzer-div-by-zero\\\]" } */
}

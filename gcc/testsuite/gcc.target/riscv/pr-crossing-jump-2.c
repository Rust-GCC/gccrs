/* Test that multiple crossing jumps all use the long form.
   This is a more comprehensive test for CROSSING_JUMP_P handling.  */

/* { dg-do compile } */
/* { dg-require-effective-target freorder } */
/* { dg-options "-O2 -freorder-blocks-and-partition" } */

extern void abort (void);

static void __attribute__((cold, noinline))
cold_path_1 (void)
{
  abort ();
}

static void __attribute__((cold, noinline))
cold_path_2 (void)
{
  abort ();
}

/* Function with multiple cold paths.  */
int
validate (int a, int b)
{
  if (__builtin_expect (a > 0, 1))
    {
      if (__builtin_expect (b > 0, 1))
	{
	  /* Hot path  */
	  return a + b;
	}
      /* Cold path 1  */
      cold_path_1 ();
    }
  /* Cold path 2  */
  cold_path_2 ();
  return -1;
}

/* All jumps to cold sections should use "jump" (AUIPC+JALR).
   We expect at least 2 crossing jumps.  */

/* { dg-final { scan-assembler-times "jump\\t\\.L\[0-9\]+,ra" 2 } } */


/* Test that jumps crossing section boundaries use the long form (AUIPC+JALR).
   When -freorder-blocks-and-partition is used, cold code is placed in
   .text.unlikely section.  Jumps from hot code (.text) to cold code
   (.text.unlikely) must use the long form because the linker may place
   these sections more than 1MB apart, exceeding the JAL instruction's range.

   This test verifies that CROSSING_JUMP_P is handled correctly in the
   RISC-V backend.  */

/* { dg-do compile } */
/* { dg-require-effective-target freorder } */
/* { dg-options "-O2 -freorder-blocks-and-partition" } */

extern void abort (void);

/* Force the error path to be cold.  */
static void __attribute__((cold, noinline))
handle_error (void)
{
  abort ();
}

/* Function with hot and cold paths.  */
int
check_positive (int x)
{
  if (__builtin_expect (x > 0, 1))
    {
      /* Hot path - stays in .text  */
      return x;
    }
  /* Cold path - moved to .text.unlikely  */
  handle_error ();
  return -1;
}

/* The jump to the cold section should use "jump" (AUIPC+JALR) not "j" (JAL).
   We check for "jump" instruction which is the long form.  */

/* { dg-final { scan-assembler "jump\\t\\.L\[0-9\]+,ra" } } */


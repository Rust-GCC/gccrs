/* { dg-do compile } */
/* { dg-skip-if "" { ! { arm_thumb1_ok || arm_thumb2_ok } } } */
/* { dg-options "-mthumb -Os" }  */
/* { dg-final { scan-assembler "push\t\{r3" { target { ! arm*-*-uclinuxfdpiceabi } } } } */
/* { dg-final { scan-assembler-not "\[^\-\]r8" { target { ! arm*-*-uclinuxfdpiceabi } } } } */

extern int hist_verify;
extern int a1;
extern char *pre_process_line (char*);
extern char* str_cpy (char*, char*);
extern int str_len (char*);
extern char* x_malloc (int);
#define savestring(x) (char *)str_cpy (x_malloc (1 + str_len (x)), (x))

char *
history_expand_line_internal (char* line)
{
  char *new_line;
  int old_verify;
  int a = a1;
  old_verify = hist_verify;
  hist_verify = 0;
  new_line = pre_process_line (line);
  hist_verify = old_verify + a;
  return (new_line == line) ? savestring (line) : new_line;
}

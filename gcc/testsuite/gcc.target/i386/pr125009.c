/* { dg-do compile { target fpic } } */
/* { dg-options "-mtune=generic -O2 -fPIC" } */
/* { dg-additional-options "-march=pentiumpro" { target ia32 } } */

struct target_var_desc {
  bool always_copy_from;
  bool is_attach;
};
struct target_mem_desc {
  struct target_var_desc list[];
};
void
gomp_map_vars_internal (int n, int l)
{
  struct target_mem_desc *tgt;
  int j;
  for (j = 0; j < l; j++)
    tgt->list[j].always_copy_from = tgt->list[j].is_attach = false;
  for (;j < n; j++)
    tgt->list[j].always_copy_from = tgt->list[j].is_attach = false;
}

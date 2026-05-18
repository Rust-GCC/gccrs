/* { dg-do compile { target rv64 } } */
/* { dg-options "-O2 -fdump-rtl-ext_dce" } */
typedef long unsigned int size_t;
struct function
{
  unsigned int curr_properties;
  unsigned int last_verified;
};
extern struct function *cfun;

void
execute_function_todo (void *data)
{
  unsigned int flags = (size_t) data;
  if (flags & (1 << 5))
    flags |= (1 << 15);
  (cfun + 0)->last_verified = flags & ((1 << 2) | (1 << 3) | (1 << 4));
}

/* { dg-final { scan-rtl-dump "Successfully transformed to:" "ext_dce" } } */


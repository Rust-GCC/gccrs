/* Test "originally defined" message for struct redefinition for C23.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */

struct s { int i; }; /* { dg-message "originally defined" } */
struct s { int j; }; /* { dg-error "redefinition" } */

void
f (int x)
{
  struct t { int (*a)[x]; }; /* { dg-message "originally defined" } */
  struct t { int (*a)[x]; }; /* { dg-error "redefinition" } */
}

/* PR c/48552 */
/* { dg-do compile } */
/* { dg-options "" } */

struct S;

void
f1 (void *x)
{
  __asm volatile ("" : : "r" (*x));	/* { dg-warning "dereferencing" "deref" } */
}					/* { dg-error "invalid use of void expression" "void expr" { target *-*-* } .-1 } */

void
f2 (void *x)
{
  __asm volatile ("" : "=r" (*x));	/* { dg-warning "dereferencing" "deref" } */
}					/* { dg-error "invalid use of void expression" "void expr" { target *-*-* } .-1 } */
					/* { dg-error "invalid lvalue in 'asm' output 0" "invalid lvalue" { target *-*-* } .-2 } */
void
f3 (void *x)
{
  __asm volatile ("" : : "m" (*x));	/* { dg-warning "dereferencing" } */
}

void
f4 (void *x)
{
  __asm volatile ("" : "=m" (*x));	/* { dg-warning "dereferencing" } */
}

void
f5 (void *x)
{
  __asm volatile ("" : : "g" (*x));	/* { dg-warning "dereferencing" "deref" } */
}					/* { dg-error "invalid use of void expression" "void expr" { target *-*-* } .-1 } */

void
f6 (void *x)
{
  __asm volatile ("" : "=g" (*x));	/* { dg-warning "dereferencing" "deref" } */
}					/* { dg-error "invalid use of void expression" "void expr" { target *-*-* } .-1 } */
					/* { dg-error "invalid lvalue in 'asm' output 0" "invalid lvalue" { target *-*-* } .-2 } */
void
f7 (struct S *x)
{
  __asm volatile ("" : : "r" (*x));	/* { dg-error "dereferencing pointer to incomplete type" } */
}

void
f8 (struct S *x)
{
  __asm volatile ("" : "=r" (*x));	/* { dg-error "invalid lvalue in 'asm' output 0" } */
}

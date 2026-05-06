/* PR middle-end/124827 */
/* Test __atomic routines for invalid memory model errors. This only needs
   to be tested on a single size.  */
/* { dg-do compile } */
/* { dg-require-effective-target sync_int_long } */
/* { dg-options "-fsanitize=thread -Winvalid-memory-model" } */

int i, e, b;
bool x;

[[gnu::always_inline]] static inline void
foo (int relaxed, int seq_cst, int acquire, int release, int acq_rel, int consume)
{
  __atomic_compare_exchange_n (&i, &e, 1, 0, relaxed, seq_cst); /* { dg-warning "failure memory model 'memory_order_seq_cst' cannot be stronger" } */
  __atomic_compare_exchange_n (&i, &e, 1, 0, seq_cst, release); /* { dg-warning "invalid failure memory" } */
  __atomic_compare_exchange_n (&i, &e, 1, 1, seq_cst, acq_rel); /* { dg-warning "invalid failure memory" } */

  __atomic_load_n (&i, release); /* { dg-warning "invalid memory model" } */
  __atomic_load_n (&i, acq_rel); /* { dg-warning "invalid memory model" } */

  __atomic_store_n (&i, 1, acquire); /* { dg-warning "invalid memory model" } */
  __atomic_store_n (&i, 1, consume); /* { dg-warning "invalid memory model" } */
  __atomic_store_n (&i, 1, acq_rel); /* { dg-warning "invalid memory model" } */

  __atomic_load_n (&i, 44); /* { dg-warning "invalid memory model" } */

  __atomic_clear (&x, consume); /* { dg-warning "invalid memory model" } */
  __atomic_clear (&x, acquire); /* { dg-warning "invalid memory model" } */

  __atomic_clear (&x, acq_rel); /* { dg-warning "invalid memory model" } */

}

int
main ()
{
  foo (__ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE, __ATOMIC_RELEASE,
       __ATOMIC_ACQ_REL, __ATOMIC_CONSUME);
}

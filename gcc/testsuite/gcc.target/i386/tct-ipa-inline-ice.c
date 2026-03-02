/* Test function multiversioning and inlining of a function with a
   target_clones attribute.  Note, we need to allow inlining of
   target_clones functions, see
   "Do not inline functions with multiple clone targets." in c-attribs.cc,
   otherwise the test would not trigger the ICE.  */
/* { dg-do assemble } */
/* { dg-require-ifunc "" } */
/* { dg-options "-O3" } */

struct range_state
{
  unsigned int n;
  unsigned int s[58];
  unsigned int *q[58];
};

struct coder_state
{
  struct range_state r;
  unsigned int t;
  unsigned int x[12];
};

inline void
bar(struct range_state *r, unsigned int *ptr, unsigned int value)
{
  r->s[r->n] = value;
  r->q[r->n] = ptr;
  ++r->n;
}

__attribute__((target_clones("default", "arch=x86-64-v2")))
inline void
foo(struct coder_state *c, unsigned int back)
{
  if (back == ~0u) {
    bar(&c->r, &c->x[c->t], 0);
  }
}

__attribute__((target_clones("default", "arch=x86-64-v2")))
int
ice_trigger(struct coder_state *c, unsigned int back)
{
  while (1) {
    foo(c, back);
  }
}

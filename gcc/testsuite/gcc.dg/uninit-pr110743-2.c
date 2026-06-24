/* { dg-do compile } */
/* { dg-options "-O2 -ftrivial-auto-var-init=pattern -Wuninitialized" } */
/* { dg-additional-options "-mgeneral-regs-only" { target { aarch64-*-* x86_64-*-* i?86-*-* } } } */

struct layer_mask {
    unsigned int x : 17;
    unsigned int y : 1;
} __attribute__((__packed__)) __attribute__((__aligned__(sizeof (unsigned int))));
struct layer_masks {
  struct layer_mask layers[16];
};

void bar (struct layer_masks *);

void foo ()
{
  struct layer_masks layer_masks; /* { dg-bogus "uninitialized" } */
  bar (&layer_masks);
}

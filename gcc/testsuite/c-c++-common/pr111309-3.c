/* PR c/111309 */
/* { dg-do compile { target { c || c++11 } } } */
/* { dg-additional-options "-std=c23" { target c } } */

void
foo (void)
{
  enum E : int { E0 = 0 } e;
  __builtin_clzg (E0);		/* { dg-error "has enumerated type" } */
  __builtin_clzg (e);		/* { dg-error "has enumerated type" } */
  __builtin_clzg ((enum E) 0);	/* { dg-error "has enumerated type" } */
  __builtin_clzg (E0, 0);	/* { dg-error "has enumerated type" } */
  __builtin_clzg (e, 0);	/* { dg-error "has enumerated type" } */
  __builtin_clzg ((enum E) 0, 0);/* { dg-error "has enumerated type" } */
  __builtin_ctzg (E0);		/* { dg-error "has enumerated type" } */
  __builtin_ctzg (e);		/* { dg-error "has enumerated type" } */
  __builtin_ctzg ((enum E) 0);	/* { dg-error "has enumerated type" } */
  __builtin_ctzg (E0, 0);	/* { dg-error "has enumerated type" } */
  __builtin_ctzg (e, 0);	/* { dg-error "has enumerated type" } */
  __builtin_ctzg ((enum E) 0, 0);/* { dg-error "has enumerated type" } */
  __builtin_clrsbg (E0);	/* { dg-error "has enumerated type" } */
  __builtin_clrsbg (e);		/* { dg-error "has enumerated type" } */
  __builtin_clrsbg ((enum E) 0);/* { dg-error "has enumerated type" } */
  __builtin_ffsg (E0);		/* { dg-error "has enumerated type" } */
  __builtin_ffsg (e);		/* { dg-error "has enumerated type" } */
  __builtin_ffsg ((enum E) 0);	/* { dg-error "has enumerated type" } */
  __builtin_parityg (E0);	/* { dg-error "has enumerated type" } */
  __builtin_parityg (e);	/* { dg-error "has enumerated type" } */
  __builtin_parityg ((enum E) 0);/* { dg-error "has enumerated type" } */
  __builtin_popcountg (E0);	/* { dg-error "has enumerated type" } */
  __builtin_popcountg (e);	/* { dg-error "has enumerated type" } */
  __builtin_popcountg ((enum E) 0);/* { dg-error "has enumerated type" } */
}

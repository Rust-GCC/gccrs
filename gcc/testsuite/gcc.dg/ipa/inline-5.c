/* Check statements that are eliminated by inlining.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu17 -O2 -fdump-ipa-fnsummary-details -fno-early-inlining -fno-partial-inlining -fno-ipa-cp"  } */

struct a {int a,b,c,d,e;};
void t(int);
void t2();

void
accessfield (struct a a)
{
  t(a.a);
    /* Should compile as:
       tmp = a.a
	 Will be eliminated by inlining
       t (tmp);  */
  t2(&a);
  t(a.a);
  return;
    /* Will be eliminated by inlining */
}
void
accessreference (struct a *a)
{
  t(a->a);
    /* Should compile as:
       a.0_1 = a;
	 Will be eliminated by inlining
       tmp = a.0_1->a;
	 50% will be eliminated by inlining 
       t (tmp)  */
  t2(&a);
  return;
    /* Will be eliminated by inlining */
}

/* { dg-final { scan-ipa-dump-times "Will be eliminated" 4 "fnsummary" } } */
/* { dg-final { scan-ipa-dump-times "50. will be eliminated" 1 "fnsummary"  } } */

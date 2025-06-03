/* { dg-do compile } */
/* { dg-options "-std=gnu17 -fgnu-tm -fdump-tree-tmedge" } */

int i;
extern void virgin () __attribute__((transaction_pure));

void
foo()
{
	__transaction_atomic {
	    virgin(i);
	}
}

/* { dg-final { scan-tree-dump-times "readOnly" 1 "tmedge" } } */

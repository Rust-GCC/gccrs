/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -fdump-tree-forwprop1" } */

typedef __attribute__((__vector_size__(4))) char V4;
typedef __attribute__((__vector_size__(8))) char V8;

_BitInt(17) foo(V8 *xx)
{
	V8 x = *xx;
	V4 v = *(V4 *)&x;
	return *(_BitInt(17) *)&v;
}

/* { dg-final { scan-tree-dump "VIEW_CONVERT_EXPR" "forwprop1" } } */

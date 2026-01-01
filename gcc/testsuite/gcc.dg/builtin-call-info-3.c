/* { dg-do compile } */
/* { dg-options "-O2 -Wtrampolines -Wreturn-local-addr" } */


void * foo(int n)
{
	int g(int x)
	{
		return n + x;
	}

	return __builtin_call_code_address(g);	// ok
}

void * bar(int n)
{
	int g(int x)
	{
		return n + x;
	}

	return __builtin_call_static_chain(g);	/* { dg-warning "returns address of local variable" } */
}


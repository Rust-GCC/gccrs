/* { dg-do run } */
/* { dg-options "-Wtrampolines" } */

/* Check that we get the expected pointers in
   different context.  */

int f(int x)
{

	if (f != __builtin_call_code_address(f))
		__builtin_abort();

	if ((void*)0 != __builtin_call_static_chain(f))
		__builtin_abort();

	static int (*code)(int);
	static void *chain;

	int g(int y)
	{
		if (code != __builtin_call_code_address(g))
			__builtin_abort();
		
		if (chain != __builtin_call_static_chain(g))
			__builtin_abort();
	
		return x + y; 
	}

	code = __builtin_call_code_address(g);
	chain = __builtin_call_static_chain(g);

	return g(x);
}

int main()
{
	if (f != __builtin_call_code_address(f))
		__builtin_abort();

	if ((void*)0 != __builtin_call_static_chain(f))
		__builtin_abort();

	if (6 != f(3))
		__builtin_abort();
}


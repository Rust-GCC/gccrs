/* { dg-do run } */
/* { dg-options "-Wtrampolines" } */

typedef struct {
	int (*code)(int);
	void *chain;
} call_info_t;

int apply(call_info_t c, int value)
{
	return __builtin_call_with_static_chain(c.code(value), c.chain);
}

int foo(int x)
{
	int add(int y)
	{
		return x + y;
	}

	call_info_t info = {
		__builtin_call_code_address(add),
		__builtin_call_static_chain(add)
	};

	return apply(info, x);
}

int main()
{
	if (4 != foo(2))
		__builtin_abort();
}


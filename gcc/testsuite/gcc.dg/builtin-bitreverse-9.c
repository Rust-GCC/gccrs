/* PR middle-end/125517 */
/* { dg-do compile { target int128 } } */
/* { dg-options "-O2" } */

#if __SIZEOF_INT128__ == 16
static_assert (__builtin_bitreverse128 (((unsigned __int128)
					 0x6132fc885cb8cf0cULL << 64)
					| 0xeaa7649b043ae77bULL)
	       == (((unsigned __int128) 0xdee75c20d926e557ULL << 64)
		   | 0x30f31d3a113f4c86ULL), "bitreverse128 1");
static_assert (__builtin_bitreverse128 (0xeaa7649b043ae77bULL)
	       == ((unsigned __int128) 0xdee75c20d926e557ULL << 64),
	       "bitreverse128 2");
static_assert (__builtin_bitreverse128 (0xaa7649b043ae77bULL)
	       == ((unsigned __int128) 0xdee75c20d926e550ULL << 64),
	       "bitreverse128 3");
#endif

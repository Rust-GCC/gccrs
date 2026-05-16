/* { dg-do compile } */
/* { dg-options "-std=gnu11" } */

#if __CHAR_BIT__ == 8
_Static_assert (__builtin_bitreverse8 (0x01u) == 0x80u, "bitreverse8");
#if __SIZEOF_SHORT__ == 2
_Static_assert (__builtin_bitreverse16 (0x0001u) == 0x8000u, "bitreverse16");
#endif
#if __SIZEOF_INT__ == 4
_Static_assert (__builtin_bitreverse32 (0x12345678u) == 0x1e6a2c48u,
		"bitreverse32");
#endif
#if __SIZEOF_LONG_LONG__ == 8
_Static_assert (__builtin_bitreverse64 (0x0123456789abcdefull)
		== 0xf7b3d591e6a2c480ull, "bitreverse64");
#endif
#if __SIZEOF_INT128__ == 16
_Static_assert (__builtin_bitreverse128 (((unsigned __int128)
					  0x0123456789abcdefull << 64)
					 | 0x2468ace013579bdfull)
		== (((unsigned __int128) 0xfbd9eac807351624ull << 64)
		    | 0xf7b3d591e6a2c480ull), "bitreverse128");
#endif
#endif

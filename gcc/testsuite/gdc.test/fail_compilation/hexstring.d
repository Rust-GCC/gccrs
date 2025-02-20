/**
TEST_OUTPUT:
---
fail_compilation/hexstring.d(29): Error: cannot implicitly convert expression `"123F"` of type `string` to `immutable(ubyte[])`
fail_compilation/hexstring.d(33): Error: hex string length 1 must be a multiple of 2 to cast to `immutable(ushort[])`
fail_compilation/hexstring.d(34): Error: hex string length 3 must be a multiple of 4 to cast to `immutable(uint[])`
fail_compilation/hexstring.d(35): Error: hex string length 5 must be a multiple of 8 to cast to `immutable(ulong[])`
fail_compilation/hexstring.d(36): Error: array cast from `wstring` to `immutable(ulong[])` is not supported at compile time
fail_compilation/hexstring.d(36):        perhaps remove postfix `w` from hex string
fail_compilation/hexstring.d(37): Error: array cast from `string` to `immutable(uint[])` is not supported at compile time
fail_compilation/hexstring.d(38): Error: array cast from `string` to `immutable(ushort[])` is not supported at compile time
fail_compilation/hexstring.d(39): Error: array cast from `string` to `immutable(uint[])` is not supported at compile time
fail_compilation/hexstring.d(39):        perhaps remove postfix `c` from hex string
fail_compilation/hexstring.d(40): Error: hex string with `dstring` type needs to be multiple of 4 bytes, not 5
fail_compilation/hexstring.d(41): Error: cannot implicitly convert expression `x"11223344"d` of type `dstring` to `immutable(float[])`
fail_compilation/hexstring.d(42): Error: cannot implicitly convert expression `x"1122"w` of type `wstring` to `immutable(ubyte[])`
fail_compilation/hexstring.d(28): Error: cannot implicitly convert expression `x"123F"` of type `string` to `ubyte[]`
---
*/
immutable ubyte[] s0 = x"123F";
static assert(s0[0] == 0x12);
static assert(s0[1] == 0x3F);
immutable byte[] s1 = x"123F";

enum E(X) = cast(X[]) x"AABBCCDD";
static assert(E!int[0] == 0xAABBCCDD);

ubyte[] f1 = x"123F";
immutable ubyte[] f2 = "123F";
immutable ubyte[] f3 = x"123F"c;
immutable ubyte[] f4 = cast(string) x"123F";

immutable ushort[] f5 = cast(immutable ushort[]) x"11";
immutable uint[] f6 = cast(immutable uint[]) x"112233";
immutable ulong[] f7 = cast(immutable ulong[]) x"1122334455";
immutable ulong[] f8 = cast(immutable ulong[]) x"11223344"w;
immutable uint[] f9 = cast(immutable uint[]) "ABCD";
immutable ushort[] f10 = cast(immutable ushort[]) (x"1122" ~ "");
immutable uint[] f11 = cast(immutable uint[]) x"AABBCCDD"c;
immutable uint[] f12 = x"1122334455"d;
immutable float[] f13 = x"11223344"d;
immutable ubyte[] f14 = x"1122"w;

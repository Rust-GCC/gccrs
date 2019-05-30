/* { dg-do compile } */
/* { dg-require-effective-target wchar } */
/* { dg-options "-std=gnu99" } */

#include <wchar.h>

typedef __CHAR16_TYPE__	char16_t;
typedef __CHAR32_TYPE__ char32_t;

const char	s0[]	= u8"ab";
const char16_t	s1[]	= u8"ab";	/* { dg-error "from a string literal with type array of .char." } */
const char32_t  s2[]    = u8"ab";	/* { dg-error "from a string literal with type array of .char." } */
const wchar_t   s3[]    = u8"ab";	/* { dg-error "from a string literal with type array of .char." } */

const char      t0[0]   = u8"ab";	/* { dg-warning " is too long" } */
const char      t1[1]   = u8"ab";	/* { dg-warning " is too long" } */
const char      t2[2]   = u8"ab";
const char      t3[3]   = u8"ab";
const char      t4[4]   = u8"ab";

const char      u0[0]   = u8"\u2160.";	/* { dg-warning " is too long" } */
const char      u1[1]   = u8"\u2160.";	/* { dg-warning " is too long" } */
const char      u2[2]   = u8"\u2160.";	/* { dg-warning " is too long" } */
const char      u3[3]   = u8"\u2160.";	/* { dg-warning " is too long" } */
const char      u4[4]   = u8"\u2160.";
const char      u5[5]   = u8"\u2160.";
const char      u6[6]   = u8"\u2160.";

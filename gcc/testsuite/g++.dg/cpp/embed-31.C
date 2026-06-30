// { dg-do preprocess }
// { dg-options "-Wno-c++29-extensions" }

#embed __FILE__ offset(1) offset(1) // { dg-error "duplicate embed parameter 'offset'" }
#embed __FILE__ offset prefix() suffix() // { dg-error "expected '\\\('" }
#embed __FILE__ offset (1 / 0) // { dg-error "division by zero in #embed" }
#embed __FILE__ __offset__ (+ + +) // { dg-error "operator '\\\+' has no right operand" }
#define FOO 1
#embed __FILE__ offset(0 + defined(FOO)) // { dg-error "'defined' in '#embed' parameter" }
#embed __FILE__ offset (-1) // { dg-error "negative embed parameter operand" }
#embed __FILE__ offset (-42) // { dg-error "negative embed parameter operand" }
#embed __FILE__ offset (-9223372036854775807 - 1) // { dg-error "negative embed parameter operand" }
#embed __FILE__ offset (18446744073709551615ULL) // { dg-error "too large 'offset' argument" }
#embed __FILE__ offset(0) gnu::offset(0) // { dg-error "'gnu::offset' parameter conflicts with 'offset' parameter" }
#embed __FILE__ gnu::offset(0) offset(0) // { dg-error "'offset' parameter conflicts with 'gnu::offset' parameter" }
#embed __FILE__ __offset__(0) __gnu__::__offset__(0) // { dg-error "'gnu::offset' parameter conflicts with 'offset' parameter" }
#embed __FILE__ __gnu__::__offset__(0) __offset__(0) // { dg-error "'offset' parameter conflicts with 'gnu::offset' parameter" }
#if 1 + __has_embed (__FILE__ offset(1) __offset__(1)) // { dg-error "duplicate embed parameter 'offset'" }
#endif
#if 1 + __has_embed (__FILE__ __offset__ prefix() suffix()) // { dg-error "expected '\\\('" }
#endif
#if 1 + __has_embed (__FILE__ offset(1/0)) // { dg-error "division by zero in #embed" }
#endif
#if 1 + __has_embed (__FILE__ offset(+ + +)) // { dg-error "operator '\\\+' has no right operand" }
#endif
#if 1 + __has_embed (__FILE__ offset(0 + defined(FOO))) // { dg-error "'defined' in '#embed' parameter" }
#endif
#if 1 + __has_embed (__FILE__ offset (-1)) // { dg-error "negative embed parameter operand" }
#endif
#if 1 + __has_embed (__FILE__ offset (-42)) // { dg-error "negative embed parameter operand" }
#endif
#if 1 + __has_embed (__FILE__ offset (-9223372036854775807 - 1)) // { dg-error "negative embed parameter operand" }
#endif
#if 1 + __has_embed (__FILE__ offset (18446744073709551615ULL)) // { dg-error "too large 'offset' argument" }
#endif
#if 1 + __has_embed (__FILE__ offset (0) gnu::offset (0)) // { dg-error "'gnu::offset' parameter conflicts with 'offset' parameter" }
#endif
#if 1 + __has_embed (__FILE__ gnu::offset (0) offset (0)) // { dg-error "'offset' parameter conflicts with 'gnu::offset' parameter" }
#endif
#if 1 + __has_embed (__FILE__ __offset__ (0) __gnu__::__offset__ (0)) // { dg-error "'gnu::offset' parameter conflicts with 'offset' parameter" }
#endif
#if 1 + __has_embed (__FILE__ __gnu__::__offset__ (0) __offset__ (0)) // { dg-error "'offset' parameter conflicts with 'gnu::offset' parameter" }
#endif

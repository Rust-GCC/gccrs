/* Test #elifdef and #elifndef in GNU11: erroneous usages.  */
/* { dg-do preprocess } */
/* { dg-options "-std=gnu11" } */

#define A
#undef B

#elifdef A /* { dg-error "'#elifdef' without '#if'" } */
#elifdef B /* { dg-error "'#elifdef' without '#if'" } */
#elifndef A /* { dg-error "'#elifndef' without '#if'" } */
#elifndef B /* { dg-error "'#elifndef' without '#if'" } */

#if 1 /* { dg-error "-:began here" } */
#else
#elifdef A /* { dg-error "'#elifdef' after '#else'" } */
#endif

#if 1 /* { dg-error "-:began here" } */
#else
#elifdef B /* { dg-error "'#elifdef' after '#else'" } */
#endif

#if 1 /* { dg-error "-:began here" } */
#else
#elifndef A /* { dg-error "'#elifndef' after '#else'" } */
#endif

#if 1 /* { dg-error "-:began here" } */
#else
#elifndef B /* { dg-error "'#elifndef' after '#else'" } */
#endif

#if 0
#elifdef A = /* { dg-warning "extra tokens at end of '#elifdef' directive" } */
#endif

#if 0
#elifdef B = /* { dg-warning "extra tokens at end of '#elifdef' directive" } */
#endif

#if 0
#elifndef A = /* { dg-warning "extra tokens at end of '#elifndef' directive" } */
#endif

#if 0
#elifndef B = /* { dg-warning "extra tokens at end of '#elifndef' directive" } */
#endif

#if 0
#elifdef /* { dg-error "no macro name given in '#elifdef' directive" } */
#endif

#if 0
#elifndef /* { dg-error "no macro name given in '#elifndef' directive" } */
#endif

#if 0
#elifdef , /* { dg-error "macro names must be identifiers" } */
#endif

#if 0
#elifndef , /* { dg-error "macro names must be identifiers" } */
#endif

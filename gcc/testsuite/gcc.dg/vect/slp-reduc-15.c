/* { dg-do compile } */
/* { dg-require-effective-target vect_float } */
/* { dg-additional-options "-fgimple -march=x86-64-v3" { target x86_64-*-* } } */

/* Test that SLP reduction vectorization handles commutative operand swap
   for .COND_ADD in multi-lane SLP where the reduction operand appears
   at arg1 in some lanes and arg2 in others.  If-conversion can generate
   COND_ADD with mixed reduction operand positions.

   Without the IFN commutative swap fix, the multi-lane SLP group
   would be rejected with "different reduc_idx N instead of M".  */

float cond_arr[1024];
float data_a[1024];
float data_b[1024];
float data_c[1024];
float data_d[1024];

void __GIMPLE (ssa, guessed_local(16535624), startwith("loop"))
foo (float * restrict p0, float * restrict p1,
     float * restrict p2, float * restrict p3)
{
  float s0;
  float s1;
  float s2;
  float s3;
  int i;
  float _a;
  float _b;
  float _c;
  float _d;
  float _5;
  _Bool _cond;
  unsigned int _30;
  unsigned int _31;

  __BB(2, guessed_local(16535624)):
  goto __BB3(precise(134217728));

  __BB(3, loop_header(1), guessed_local(1057206200)):
  s0_22 = __PHI (__BB5: s0_10, __BB2: 0.0f);
  s1_24 = __PHI (__BB5: s1_11, __BB2: 0.0f);
  s2_26 = __PHI (__BB5: s2_12, __BB2: 0.0f);
  s3_28 = __PHI (__BB5: s3_13, __BB2: 0.0f);
  i_27 = __PHI (__BB5: i_19, __BB2: 0);
  _31 = __PHI (__BB5: _30, __BB2: 1024u);
  _5 = cond_arr[i_27];
  _cond = _5 > 0.0f;
  _a = data_a[i_27];
  _b = data_b[i_27];
  _c = data_c[i_27];
  _d = data_d[i_27];
  /* s0, s1: reduction operand at position 1 (arg1) → reduc_idx=1 */
  s0_10 = .COND_ADD (_cond, s0_22, _a, s0_22);
  s1_11 = .COND_ADD (_cond, s1_24, _b, s1_24);
  /* s2, s3: reduction operand at position 2 (arg2) → reduc_idx=2 */
  s2_12 = .COND_ADD (_cond, _c, s2_26, s2_26);
  s3_13 = .COND_ADD (_cond, _d, s3_28, s3_28);
  i_19 = i_27 + 1;
  _30 = _31 - 1u;
  if (_30 != 0u)
    goto __BB5(guessed(132118446));
  else
    goto __BB4(guessed(2099282));

  __BB(5, guessed_local(1040670576)):
  goto __BB3(precise(134217728));

  __BB(4, guessed_local(16535624)):
  *p0_40(D) = s0_10;
  *p1_41(D) = s1_11;
  *p2_42(D) = s2_12;
  *p3_43(D) = s3_13;
  return;
}

/* With the IFN commutative swap fix, these 4 reductions should be
   vectorized using SLP despite different reduc_idx values (1 vs 2).  */
/* { dg-final { scan-tree-dump "swapped operands to match def types in" "vect" { target x86_64-*-* } } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 4 "vect" { target x86_64-*-* } } } */


use crate::{
    core_arch::{simd::*, simd_llvm::*, x86::*},
    mem::{self, transmute},
    ptr,
};

#[cfg(test)]
use stdarch_test::assert_instr;

/// Computes the absolute values of packed 32-bit integers in `a`.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990,33&text=_mm512_abs_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsd))]
pub unsafe fn _mm512_abs_epi32(a: __m512i) -> __m512i {
    let a = a.as_i32x16();
    // all-0 is a properly initialized i32x16
    let zero: i32x16 = mem::zeroed();
    let sub = simd_sub(zero, a);
    let cmp: i32x16 = simd_gt(a, zero);
    transmute(simd_select(cmp, a, sub))
}

/// Computes the absolute value of packed 32-bit integers in `a`, and store the
/// unsigned results in `dst` using writemask `k` (elements are copied from
/// `src` when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990,33&text=_mm512_abs_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsd))]
pub unsafe fn _mm512_mask_abs_epi32(src: __m512i, k: __mmask16, a: __m512i) -> __m512i {
    let abs = _mm512_abs_epi32(a).as_i32x16();
    transmute(simd_select_bitmask(k, abs, src.as_i32x16()))
}

/// Computes the absolute value of packed 32-bit integers in `a`, and store the
/// unsigned results in `dst` using zeromask `k` (elements are zeroed out when
/// the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990,33,34,35,35&text=_mm512_maskz_abs_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsd))]
pub unsafe fn _mm512_maskz_abs_epi32(k: __mmask16, a: __m512i) -> __m512i {
    let abs = _mm512_abs_epi32(a).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, abs, zero))
}

/// Compute the absolute value of packed signed 64-bit integers in a, and store the unsigned results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_abs_epi64&expand=48)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsq))]
pub unsafe fn _mm512_abs_epi64(a: __m512i) -> __m512i {
    let a = a.as_i64x8();
    // all-0 is a properly initialized i64x8
    let zero: i64x8 = mem::zeroed();
    let sub = simd_sub(zero, a);
    let cmp: i64x8 = simd_gt(a, zero);
    transmute(simd_select(cmp, a, sub))
}

/// Compute the absolute value of packed signed 64-bit integers in a, and store the unsigned results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_abs_epi64&expand=49)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsq))]
pub unsafe fn _mm512_mask_abs_epi64(src: __m512i, k: __mmask8, a: __m512i) -> __m512i {
    let abs = _mm512_abs_epi64(a).as_i64x8();
    transmute(simd_select_bitmask(k, abs, src.as_i64x8()))
}

/// Compute the absolute value of packed signed 64-bit integers in a, and store the unsigned results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_abs_epi64&expand=50)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpabsq))]
pub unsafe fn _mm512_maskz_abs_epi64(k: __mmask8, a: __m512i) -> __m512i {
    let abs = _mm512_abs_epi64(a).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, abs, zero))
}

/// Finds the absolute value of each packed single-precision (32-bit) floating-point element in v2, storing the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_abs_ps&expand=65)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_abs_ps(v2: __m512) -> __m512 {
    let a = _mm512_set1_epi32(0x7FFFFFFF); // from LLVM code
    let b = transmute::<f32x16, __m512i>(v2.as_f32x16());
    let abs = _mm512_and_epi32(a, b);
    transmute(abs)
}

/// Finds the absolute value of each packed single-precision (32-bit) floating-point element in v2, storing the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_abs_ps&expand=66)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandd))]
pub unsafe fn _mm512_mask_abs_ps(src: __m512, k: __mmask16, v2: __m512) -> __m512 {
    let abs = _mm512_abs_ps(v2).as_f32x16();
    transmute(simd_select_bitmask(k, abs, src.as_f32x16()))
}

/// Finds the absolute value of each packed double-precision (64-bit) floating-point element in v2, storing the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_abs_pd&expand=60)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_abs_pd(v2: __m512d) -> __m512d {
    let a = _mm512_set1_epi64(0x7FFFFFFFFFFFFFFF); // from LLVM code
    let b = transmute::<f64x8, __m512i>(v2.as_f64x8());
    let abs = _mm512_and_epi64(a, b);
    transmute(abs)
}

/// Finds the absolute value of each packed double-precision (64-bit) floating-point element in v2, storing the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_abs_pd&expand=61)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_mask_abs_pd(src: __m512d, k: __mmask8, v2: __m512d) -> __m512d {
    let abs = _mm512_abs_pd(v2).as_f64x8();
    transmute(simd_select_bitmask(k, abs, src.as_f64x8()))
}

/// Add packed 32-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_epi32&expand=100)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddd))]
pub unsafe fn _mm512_add_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_add(a.as_i32x16(), b.as_i32x16()))
}

/// Add packed 32-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_epi32&expand=101)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddd))]
pub unsafe fn _mm512_mask_add_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let add = _mm512_add_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, add, src.as_i32x16()))
}

/// Add packed 32-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_epi32&expand=102)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddd))]
pub unsafe fn _mm512_maskz_add_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let add = _mm512_add_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, add, zero))
}

/// Add packed 64-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_epi64&expand=109)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddq))]
pub unsafe fn _mm512_add_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_add(a.as_i64x8(), b.as_i64x8()))
}

/// Add packed 64-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_epi64&expand=110)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddq))]
pub unsafe fn _mm512_mask_add_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let add = _mm512_add_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, add, src.as_i64x8()))
}

/// Add packed 64-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_epi64&expand=111)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpaddq))]
pub unsafe fn _mm512_maskz_add_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let add = _mm512_add_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, add, zero))
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_ps&expand=139)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps))]
pub unsafe fn _mm512_add_ps(a: __m512, b: __m512) -> __m512 {
    transmute(simd_add(a.as_f32x16(), b.as_f32x16()))
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_ps&expand=140)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps))]
pub unsafe fn _mm512_mask_add_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let add = _mm512_add_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, add, src.as_f32x16()))
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_ps&expand=141)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps))]
pub unsafe fn _mm512_maskz_add_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let add = _mm512_add_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, add, zero))
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_pd&expand=127)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd))]
pub unsafe fn _mm512_add_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(simd_add(a.as_f64x8(), b.as_f64x8()))
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_pd&expand=128)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd))]
pub unsafe fn _mm512_mask_add_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let add = _mm512_add_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, add, src.as_f64x8()))
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_pd&expand=129)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd))]
pub unsafe fn _mm512_maskz_add_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let add = _mm512_add_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, add, zero))
}

/// Subtract packed 32-bit integers in b from packed 32-bit integers in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_epi32&expand=5694)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubd))]
pub unsafe fn _mm512_sub_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_sub(a.as_i32x16(), b.as_i32x16()))
}

/// Subtract packed 32-bit integers in b from packed 32-bit integers in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_epi32&expand=5692)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubd))]
pub unsafe fn _mm512_mask_sub_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let sub = _mm512_sub_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, sub, src.as_i32x16()))
}

/// Subtract packed 32-bit integers in b from packed 32-bit integers in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_epi32&expand=5693)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubd))]
pub unsafe fn _mm512_maskz_sub_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let sub = _mm512_sub_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, sub, zero))
}

/// Subtract packed 64-bit integers in b from packed 64-bit integers in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_epi64&expand=5703)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubq))]
pub unsafe fn _mm512_sub_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_sub(a.as_i64x8(), b.as_i64x8()))
}

/// Subtract packed 64-bit integers in b from packed 64-bit integers in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_epi64&expand=5701)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubq))]
pub unsafe fn _mm512_mask_sub_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let sub = _mm512_sub_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, sub, src.as_i64x8()))
}

/// Subtract packed 64-bit integers in b from packed 64-bit integers in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_epi64&expand=5702)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsubq))]
pub unsafe fn _mm512_maskz_sub_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let add = _mm512_sub_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, add, zero))
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_ps&expand=5733)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps))]
pub unsafe fn _mm512_sub_ps(a: __m512, b: __m512) -> __m512 {
    transmute(simd_sub(a.as_f32x16(), b.as_f32x16()))
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_ps&expand=5731)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps))]
pub unsafe fn _mm512_mask_sub_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let sub = _mm512_sub_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, sub, src.as_f32x16()))
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_ps&expand=5732)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps))]
pub unsafe fn _mm512_maskz_sub_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let sub = _mm512_sub_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, sub, zero))
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_pd&expand=5721)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd))]
pub unsafe fn _mm512_sub_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(simd_sub(a.as_f64x8(), b.as_f64x8()))
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_pd&expand=5719)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd))]
pub unsafe fn _mm512_mask_sub_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let sub = _mm512_sub_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, sub, src.as_f64x8()))
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_pd&expand=5720)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd))]
pub unsafe fn _mm512_maskz_sub_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let sub = _mm512_sub_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, sub, zero))
}

/// Multiply the low signed 32-bit integers from each packed 64-bit element in a and b, and store the signed 64-bit results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mul_epi32&expand=3907)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuldq))]
pub unsafe fn _mm512_mul_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmuldq(a.as_i32x16(), b.as_i32x16()))
}

/// Multiply the low signed 32-bit integers from each packed 64-bit element in a and b, and store the signed 64-bit results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_epi32&expand=3905)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuldq))]
pub unsafe fn _mm512_mask_mul_epi32(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let mul = _mm512_mul_epi32(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, mul, src.as_i64x8()))
}

/// Multiply the low signed 32-bit integers from each packed 64-bit element in a and b, and store the signed 64-bit results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_epi32&expand=3906)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuldq))]
pub unsafe fn _mm512_maskz_mul_epi32(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let mul = _mm512_mul_epi32(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, mul, zero))
}

/// Multiply the packed 32-bit integers in a and b, producing intermediate 64-bit integers, and store the low 32 bits of the intermediate integers in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mullo_epi&expand=4005)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmulld))]
pub unsafe fn _mm512_mullo_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_mul(a.as_i32x16(), b.as_i32x16()))
}

/// Multiply the packed 32-bit integers in a and b, producing intermediate 64-bit integers, and store the low 32 bits of the intermediate integers in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mullo_epi32&expand=4003)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmulld))]
pub unsafe fn _mm512_mask_mullo_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let mul = _mm512_mullo_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, mul, src.as_i32x16()))
}

/// Multiply the packed 32-bit integers in a and b, producing intermediate 64-bit integers, and store the low 32 bits of the intermediate integers in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mullo_epi32&expand=4004)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmulld))]
pub unsafe fn _mm512_maskz_mullo_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let mul = _mm512_mullo_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, mul, zero))
}

/// Multiplies elements in packed 64-bit integer vectors a and b together, storing the lower 64 bits of the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mullox_epi64&expand=4017)
///
/// This intrinsic generates a sequence of instructions, which may perform worse than a native instruction. Consider the performance impact of this intrinsic.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_mullox_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_mul(a.as_i64x8(), b.as_i64x8()))
}

/// Multiplies elements in packed 64-bit integer vectors a and b together, storing the lower 64 bits of the result in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mullox&expand=4016)
///
/// This intrinsic generates a sequence of instructions, which may perform worse than a native instruction. Consider the performance impact of this intrinsic.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_mask_mullox_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let mul = _mm512_mullox_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, mul, src.as_i64x8()))
}

/// Multiply the low unsigned 32-bit integers from each packed 64-bit element in a and b, and store the unsigned 64-bit results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mul_epu32&expand=3916)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuludq))]
pub unsafe fn _mm512_mul_epu32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmuludq(a.as_u32x16(), b.as_u32x16()))
}

/// Multiply the low unsigned 32-bit integers from each packed 64-bit element in a and b, and store the unsigned 64-bit results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_epu32&expand=3914)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuludq))]
pub unsafe fn _mm512_mask_mul_epu32(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let mul = _mm512_mul_epu32(a, b).as_u64x8();
    transmute(simd_select_bitmask(k, mul, src.as_u64x8()))
}

/// Multiply the low unsigned 32-bit integers from each packed 64-bit element in a and b, and store the unsigned 64-bit results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_epu32&expand=3915)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmuludq))]
pub unsafe fn _mm512_maskz_mul_epu32(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let mul = _mm512_mul_epu32(a, b).as_u64x8();
    let zero = _mm512_setzero_si512().as_u64x8();
    transmute(simd_select_bitmask(k, mul, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=mm512_mul_ps&expand=3934)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps))]
pub unsafe fn _mm512_mul_ps(a: __m512, b: __m512) -> __m512 {
    transmute(simd_mul(a.as_f32x16(), b.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). RM.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_ps&expand=3932)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps))]
pub unsafe fn _mm512_mask_mul_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let mul = _mm512_mul_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, mul, src.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_ps&expand=3933)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps))]
pub unsafe fn _mm512_maskz_mul_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let mul = _mm512_mul_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, mul, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mul_pd&expand=3925)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd))]
pub unsafe fn _mm512_mul_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(simd_mul(a.as_f64x8(), b.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). RM.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_pd&expand=3923)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd))]
pub unsafe fn _mm512_mask_mul_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let mul = _mm512_mul_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, mul, src.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_pd&expand=3924)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd))]
pub unsafe fn _mm512_maskz_mul_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let mul = _mm512_mul_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, mul, zero))
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_div_ps&expand=2162)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps))]
pub unsafe fn _mm512_div_ps(a: __m512, b: __m512) -> __m512 {
    transmute(simd_div(a.as_f32x16(), b.as_f32x16()))
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_div_ps&expand=2163)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps))]
pub unsafe fn _mm512_mask_div_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let div = _mm512_div_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, div, src.as_f32x16()))
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_div_ps&expand=2164)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps))]
pub unsafe fn _mm512_maskz_div_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let div = _mm512_div_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, div, zero))
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_div_pd&expand=2153)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd))]
pub unsafe fn _mm512_div_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(simd_div(a.as_f64x8(), b.as_f64x8()))
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_div_pd&expand=2154)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd))]
pub unsafe fn _mm512_mask_div_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let div = _mm512_div_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, div, src.as_f64x8()))
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_div_pd&expand=2155)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd))]
pub unsafe fn _mm512_maskz_div_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let div = _mm512_div_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, div, zero))
}

/// Compare packed signed 32-bit integers in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_epi32&expand=3582)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsd))]
pub unsafe fn _mm512_max_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmaxsd(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_epi32&expand=3580)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsd))]
pub unsafe fn _mm512_mask_max_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, max, src.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_epi32&expand=3581)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsd))]
pub unsafe fn _mm512_maskz_max_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed signed 64-bit integers in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_epi64&expand=3591)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsq))]
pub unsafe fn _mm512_max_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmaxsq(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_epi64&expand=3589)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsq))]
pub unsafe fn _mm512_mask_max_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, max, src.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_epi64&expand=3590)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxsq))]
pub unsafe fn _mm512_maskz_max_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_ps&expand=3655)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps))]
pub unsafe fn _mm512_max_ps(a: __m512, b: __m512) -> __m512 {
    transmute(vmaxps(
        a.as_f32x16(),
        b.as_f32x16(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_ps&expand=3653)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps))]
pub unsafe fn _mm512_mask_max_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let max = _mm512_max_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, max, src.as_f32x16()))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_ps&expand=3654)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps))]
pub unsafe fn _mm512_maskz_max_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let max = _mm512_max_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_pd&expand=3645)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd))]
pub unsafe fn _mm512_max_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(vmaxpd(a.as_f64x8(), b.as_f64x8(), _MM_FROUND_CUR_DIRECTION))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_pd&expand=3643)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd))]
pub unsafe fn _mm512_mask_max_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let max = _mm512_max_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, max, src.as_f64x8()))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_pd&expand=3644)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd))]
pub unsafe fn _mm512_maskz_max_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let max = _mm512_max_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_epu32&expand=3618)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxud))]
pub unsafe fn _mm512_max_epu32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmaxud(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_epu32&expand=3616)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxud))]
pub unsafe fn _mm512_mask_max_epu32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epu32(a, b).as_u32x16();
    transmute(simd_select_bitmask(k, max, src.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_epu32&expand=3617)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxud))]
pub unsafe fn _mm512_maskz_max_epu32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epu32(a, b).as_u32x16();
    let zero = _mm512_setzero_si512().as_u32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed maximum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=max_epu64&expand=3627)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxuq))]
pub unsafe fn _mm512_max_epu64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpmaxuq(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_epu64&expand=3625)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxuq))]
pub unsafe fn _mm512_mask_max_epu64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epu64(a, b).as_u64x8();
    transmute(simd_select_bitmask(k, max, src.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_epu&expand=3626)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpmaxuq))]
pub unsafe fn _mm512_maskz_max_epu64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_max_epu64(a, b).as_u64x8();
    let zero = _mm512_setzero_si512().as_u64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed signed 32-bit integers in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_epi32&expand=3696)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsd))]
pub unsafe fn _mm512_min_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpminsd(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_epi32&expand=3694)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsd))]
pub unsafe fn _mm512_mask_min_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, max, src.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_epi32&expand=3695)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsd))]
pub unsafe fn _mm512_maskz_min_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed signed 64-bit integers in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_epi64&expand=3705)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsq))]
pub unsafe fn _mm512_min_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpminsq(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_epi64&expand=3703)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsq))]
pub unsafe fn _mm512_mask_min_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, max, src.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_epi64&expand=3704)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminsq))]
pub unsafe fn _mm512_maskz_min_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_ps&expand=3769)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps))]
pub unsafe fn _mm512_min_ps(a: __m512, b: __m512) -> __m512 {
    transmute(vminps(
        a.as_f32x16(),
        b.as_f32x16(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_ps&expand=3767)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps))]
pub unsafe fn _mm512_mask_min_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let max = _mm512_min_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, max, src.as_f32x16()))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_ps&expand=3768)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps))]
pub unsafe fn _mm512_maskz_min_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let max = _mm512_min_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_pd&expand=3759)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd))]
pub unsafe fn _mm512_min_pd(a: __m512d, b: __m512d) -> __m512d {
    transmute(vminpd(a.as_f64x8(), b.as_f64x8(), _MM_FROUND_CUR_DIRECTION))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_pd&expand=3757)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd))]
pub unsafe fn _mm512_mask_min_pd(src: __m512d, k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let max = _mm512_min_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, max, src.as_f64x8()))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_pd&expand=3758)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd))]
pub unsafe fn _mm512_maskz_min_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let max = _mm512_min_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_epu32&expand=3732)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminud))]
pub unsafe fn _mm512_min_epu32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpminud(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_epu32&expand=3730)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminud))]
pub unsafe fn _mm512_mask_min_epu32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epu32(a, b).as_u32x16();
    transmute(simd_select_bitmask(k, max, src.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_epu32&expand=3731)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminud))]
pub unsafe fn _mm512_maskz_min_epu32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epu32(a, b).as_u32x16();
    let zero = _mm512_setzero_si512().as_u32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed minimum values in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_epu64&expand=3741)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminuq))]
pub unsafe fn _mm512_min_epu64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vpminuq(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_epu64&expand=3739)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminuq))]
pub unsafe fn _mm512_mask_min_epu64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epu64(a, b).as_u64x8();
    transmute(simd_select_bitmask(k, max, src.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_epu64&expand=3740)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpminuq))]
pub unsafe fn _mm512_maskz_min_epu64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let max = _mm512_min_epu64(a, b).as_u64x8();
    let zero = _mm512_setzero_si512().as_u64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sqrt_ps&expand=5371)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps))]
pub unsafe fn _mm512_sqrt_ps(a: __m512) -> __m512 {
    transmute(vsqrtps(a.as_f32x16(), _MM_FROUND_CUR_DIRECTION))
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sqrt_ps&expand=5369)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps))]
pub unsafe fn _mm512_mask_sqrt_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    let sqrt = _mm512_sqrt_ps(a).as_f32x16();
    transmute(simd_select_bitmask(k, sqrt, src.as_f32x16()))
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sqrt_ps&expand=5370)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps))]
pub unsafe fn _mm512_maskz_sqrt_ps(k: __mmask16, a: __m512) -> __m512 {
    let sqrt = _mm512_sqrt_ps(a).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, sqrt, zero))
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sqrt_pd&expand=5362)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd))]
pub unsafe fn _mm512_sqrt_pd(a: __m512d) -> __m512d {
    transmute(vsqrtpd(a.as_f64x8(), _MM_FROUND_CUR_DIRECTION))
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sqrt_pd&expand=5360)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd))]
pub unsafe fn _mm512_mask_sqrt_pd(src: __m512d, k: __mmask8, a: __m512d) -> __m512d {
    let sqrt = _mm512_sqrt_pd(a).as_f64x8();
    transmute(simd_select_bitmask(k, sqrt, src.as_f64x8()))
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sqrt_pd&expand=5361)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd))]
pub unsafe fn _mm512_maskz_sqrt_pd(k: __mmask8, a: __m512d) -> __m512d {
    let sqrt = _mm512_sqrt_pd(a).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, sqrt, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=fmadd_ps&expand=2557)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
pub unsafe fn _mm512_fmadd_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    transmute(vfmadd132ps(
        a.as_f32x16(),
        b.as_f32x16(),
        c.as_f32x16(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmadd_ps&expand=2558)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
pub unsafe fn _mm512_mask_fmadd_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fmadd = _mm512_fmadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmadd_ps&expand=2560)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
pub unsafe fn _mm512_maskz_fmadd_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fmadd = _mm512_fmadd_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmadd_ps&expand=2559)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
pub unsafe fn _mm512_mask3_fmadd_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fmadd = _mm512_fmadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmadd_pd&expand=2545)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
pub unsafe fn _mm512_fmadd_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    transmute(vfmadd132pd(
        a.as_f64x8(),
        b.as_f64x8(),
        c.as_f64x8(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmadd_pd&expand=2546)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
pub unsafe fn _mm512_mask_fmadd_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fmadd = _mm512_fmadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmadd_pd&expand=2548)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
pub unsafe fn _mm512_maskz_fmadd_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fmadd = _mm512_fmadd_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmadd_pd&expand=2547)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
pub unsafe fn _mm512_mask3_fmadd_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fmadd = _mm512_fmadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsub_ps&expand=2643)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generate vfmadd, gcc generate vfmsub
pub unsafe fn _mm512_fmsub_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    transmute(vfmadd132ps(
        a.as_f32x16(),
        b.as_f32x16(),
        sub,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsub_ps&expand=2644)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generate vfmadd, gcc generate vfmsub
pub unsafe fn _mm512_mask_fmsub_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fmsub = _mm512_fmsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsub_ps&expand=2646)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generate vfmadd, gcc generate vfmsub
pub unsafe fn _mm512_maskz_fmsub_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fmsub = _mm512_fmsub_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsub_ps&expand=2645)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generate vfmadd, gcc generate vfmsub
pub unsafe fn _mm512_mask3_fmsub_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fmsub = _mm512_fmsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsub_pd&expand=2631)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang fmadd, gcc fmsub
pub unsafe fn _mm512_fmsub_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    transmute(vfmadd132pd(
        a.as_f64x8(),
        b.as_f64x8(),
        sub,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsub_pd&expand=2632)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang fmadd, gcc fmsub
pub unsafe fn _mm512_mask_fmsub_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fmsub = _mm512_fmsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsub_pd&expand=2634)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang fmadd, gcc fmsub
pub unsafe fn _mm512_maskz_fmsub_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fmsub = _mm512_fmsub_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmsub, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsub_pd&expand=2633)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang fmadd, gcc fmsub
pub unsafe fn _mm512_mask3_fmsub_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fmsub = _mm512_fmsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmsub, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmaddsub_ps&expand=2611)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
pub unsafe fn _mm512_fmaddsub_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    transmute(vfmaddsub213ps(
        a.as_f32x16(),
        b.as_f32x16(),
        c.as_f32x16(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmaddsub_ps&expand=2612)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
pub unsafe fn _mm512_mask_fmaddsub_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fmaddsub = _mm512_fmaddsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmaddsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmaddsub_ps&expand=2614)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
pub unsafe fn _mm512_maskz_fmaddsub_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fmaddsub = _mm512_fmaddsub_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmaddsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmaddsub_ps&expand=2613)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
pub unsafe fn _mm512_mask3_fmaddsub_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fmaddsub = _mm512_fmaddsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmaddsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmaddsub_pd&expand=2599)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
pub unsafe fn _mm512_fmaddsub_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    transmute(vfmaddsub213pd(
        a.as_f64x8(),
        b.as_f64x8(),
        c.as_f64x8(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmaddsub_pd&expand=2600)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
pub unsafe fn _mm512_mask_fmaddsub_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fmaddsub = _mm512_fmaddsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmaddsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmaddsub_pd&expand=2602)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
pub unsafe fn _mm512_maskz_fmaddsub_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fmaddsub = _mm512_fmaddsub_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmaddsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmaddsub_ps&expand=2613)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
pub unsafe fn _mm512_mask3_fmaddsub_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fmaddsub = _mm512_fmaddsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmaddsub, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsubadd_ps&expand=2691)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
pub unsafe fn _mm512_fmsubadd_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    transmute(vfmaddsub213ps(
        a.as_f32x16(),
        b.as_f32x16(),
        sub,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsubadd_ps&expand=2692)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
pub unsafe fn _mm512_mask_fmsubadd_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fmsubadd = _mm512_fmsubadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmsubadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsubadd_ps&expand=2694)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
pub unsafe fn _mm512_maskz_fmsubadd_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fmsubadd = _mm512_fmsubadd_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmsubadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsubadd_ps&expand=2693)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
pub unsafe fn _mm512_mask3_fmsubadd_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fmsubadd = _mm512_fmsubadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fmsubadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsubadd_pd&expand=2679)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
pub unsafe fn _mm512_fmsubadd_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    transmute(vfmaddsub213pd(
        a.as_f64x8(),
        b.as_f64x8(),
        sub,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsubadd_pd&expand=2680)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
pub unsafe fn _mm512_mask_fmsubadd_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fmsubadd = _mm512_fmsubadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmsubadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsubadd_pd&expand=2682)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
pub unsafe fn _mm512_maskz_fmsubadd_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fmsubadd = _mm512_fmsubadd_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmsubadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsubadd_pd&expand=2681)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
pub unsafe fn _mm512_mask3_fmsubadd_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fmsubadd = _mm512_fmsubadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fmsubadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmadd_ps&expand=2723)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
pub unsafe fn _mm512_fnmadd_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f32x16());
    transmute(vfmadd132ps(
        sub,
        b.as_f32x16(),
        c.as_f32x16(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmadd_ps&expand=2724)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
pub unsafe fn _mm512_mask_fnmadd_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fnmadd = _mm512_fnmadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fnmadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmadd_ps&expand=2726)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
pub unsafe fn _mm512_maskz_fnmadd_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fnmadd = _mm512_fnmadd_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fnmadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmadd_ps&expand=2725)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
pub unsafe fn _mm512_mask3_fnmadd_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fnmadd = _mm512_fnmadd_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fnmadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmadd_pd&expand=2711)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
pub unsafe fn _mm512_fnmadd_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f64x8());
    transmute(vfmadd132pd(
        sub,
        b.as_f64x8(),
        c.as_f64x8(),
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmadd_pd&expand=2712)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
pub unsafe fn _mm512_mask_fnmadd_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fnmadd = _mm512_fnmadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fnmadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmadd_pd&expand=2714)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
pub unsafe fn _mm512_maskz_fnmadd_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fnmadd = _mm512_fnmadd_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fnmadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmadd_pd&expand=2713)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
pub unsafe fn _mm512_mask3_fnmadd_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fnmadd = _mm512_fnmadd_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fnmadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmsub_ps&expand=2771)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
pub unsafe fn _mm512_fnmsub_ps(a: __m512, b: __m512, c: __m512) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f32x16());
    let subc = simd_sub(zero, c.as_f32x16());
    transmute(vfmadd132ps(
        suba,
        b.as_f32x16(),
        subc,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmsub_ps&expand=2772)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
pub unsafe fn _mm512_mask_fnmsub_ps(a: __m512, k: __mmask16, b: __m512, c: __m512) -> __m512 {
    let fnmsub = _mm512_fnmsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fnmsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmsub_ps&expand=2774)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
pub unsafe fn _mm512_maskz_fnmsub_ps(k: __mmask16, a: __m512, b: __m512, c: __m512) -> __m512 {
    let fnmsub = _mm512_fnmsub_ps(a, b, c).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fnmsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmsub_ps&expand=2773)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
pub unsafe fn _mm512_mask3_fnmsub_ps(a: __m512, b: __m512, c: __m512, k: __mmask16) -> __m512 {
    let fnmsub = _mm512_fnmsub_ps(a, b, c).as_f32x16();
    transmute(simd_select_bitmask(k, fnmsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmsub_pd&expand=2759)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
pub unsafe fn _mm512_fnmsub_pd(a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f64x8());
    let subc = simd_sub(zero, c.as_f64x8());
    transmute(vfmadd132pd(
        suba,
        b.as_f64x8(),
        subc,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmsub_pd&expand=2760)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
pub unsafe fn _mm512_mask_fnmsub_pd(a: __m512d, k: __mmask8, b: __m512d, c: __m512d) -> __m512d {
    let fnmsub = _mm512_fnmsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fnmsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmsub_pd&expand=2762)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
pub unsafe fn _mm512_maskz_fnmsub_pd(k: __mmask8, a: __m512d, b: __m512d, c: __m512d) -> __m512d {
    let fnmsub = _mm512_fnmsub_pd(a, b, c).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fnmsub, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmsub_pd&expand=2761)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
pub unsafe fn _mm512_mask3_fnmsub_pd(a: __m512d, b: __m512d, c: __m512d, k: __mmask8) -> __m512d {
    let fnmsub = _mm512_fnmsub_pd(a, b, c).as_f64x8();
    transmute(simd_select_bitmask(k, fnmsub, c.as_f64x8()))
}

/// Compute the approximate reciprocal of packed single-precision (32-bit) floating-point elements in a, and store the results in dst. The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rcp14_ps&expand=4502)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14ps))]
pub unsafe fn _mm512_rcp14_ps(a: __m512) -> __m512 {
    transmute(vrcp14ps(
        a.as_f32x16(),
        _mm512_setzero_ps().as_f32x16(),
        0b11111111_11111111,
    ))
}

/// Compute the approximate reciprocal of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rcp14_ps&expand=4500)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14ps))]
pub unsafe fn _mm512_mask_rcp14_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    transmute(vrcp14ps(a.as_f32x16(), src.as_f32x16(), k))
}

/// Compute the approximate reciprocal of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rcp14_ps&expand=4501)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14ps))]
pub unsafe fn _mm512_maskz_rcp14_ps(k: __mmask16, a: __m512) -> __m512 {
    transmute(vrcp14ps(a.as_f32x16(), _mm512_setzero_ps().as_f32x16(), k))
}

/// Compute the approximate reciprocal of packed double-precision (64-bit) floating-point elements in a, and store the results in dst. The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rcp14_pd&expand=4493)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14pd))]
pub unsafe fn _mm512_rcp14_pd(a: __m512d) -> __m512d {
    transmute(vrcp14pd(
        a.as_f64x8(),
        _mm512_setzero_pd().as_f64x8(),
        0b11111111,
    ))
}

/// Compute the approximate reciprocal of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rcp14_pd&expand=4491)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14pd))]
pub unsafe fn _mm512_mask_rcp14_pd(src: __m512d, k: __mmask8, a: __m512d) -> __m512d {
    transmute(vrcp14pd(a.as_f64x8(), src.as_f64x8(), k))
}

/// Compute the approximate reciprocal of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rcp14_pd&expand=4492)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrcp14pd))]
pub unsafe fn _mm512_maskz_rcp14_pd(k: __mmask8, a: __m512d) -> __m512d {
    transmute(vrcp14pd(a.as_f64x8(), _mm512_setzero_pd().as_f64x8(), k))
}

/// Compute the approximate reciprocal square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst. The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rsqrt14_ps&expand=4819)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14ps))]
pub unsafe fn _mm512_rsqrt14_ps(a: __m512) -> __m512 {
    transmute(vrsqrt14ps(
        a.as_f32x16(),
        _mm512_setzero_ps().as_f32x16(),
        0b11111111_11111111,
    ))
}

/// Compute the approximate reciprocal square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rsqrt14_ps&expand=4817)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14ps))]
pub unsafe fn _mm512_mask_rsqrt14_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    transmute(vrsqrt14ps(a.as_f32x16(), src.as_f32x16(), k))
}

/// Compute the approximate reciprocal square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rsqrt14_ps&expand=4818)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14ps))]
pub unsafe fn _mm512_maskz_rsqrt14_ps(k: __mmask16, a: __m512) -> __m512 {
    transmute(vrsqrt14ps(
        a.as_f32x16(),
        _mm512_setzero_ps().as_f32x16(),
        k,
    ))
}

/// Compute the approximate reciprocal square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst. The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rsqrt14_pd&expand=4812)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14pd))]
pub unsafe fn _mm512_rsqrt14_pd(a: __m512d) -> __m512d {
    transmute(vrsqrt14pd(
        a.as_f64x8(),
        _mm512_setzero_pd().as_f64x8(),
        0b11111111,
    ))
}

/// Compute the approximate reciprocal square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rsqrt14_pd&expand=4810)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14pd))]
pub unsafe fn _mm512_mask_rsqrt14_pd(src: __m512d, k: __mmask8, a: __m512d) -> __m512d {
    transmute(vrsqrt14pd(a.as_f64x8(), src.as_f64x8(), k))
}

/// Compute the approximate reciprocal square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). The maximum relative error for this approximation is less than 2^-14.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rsqrt14_pd&expand=4811)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vrsqrt14pd))]
pub unsafe fn _mm512_maskz_rsqrt14_pd(k: __mmask8, a: __m512d) -> __m512d {
    transmute(vrsqrt14pd(a.as_f64x8(), _mm512_setzero_pd().as_f64x8(), k))
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst. This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getexp_ps&expand=2844)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps))]
pub unsafe fn _mm512_getexp_ps(a: __m512) -> __m512 {
    transmute(vgetexpps(
        a.as_f32x16(),
        _mm512_setzero_ps().as_f32x16(),
        0b11111111_11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getexp_ps&expand=2845)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps))]
pub unsafe fn _mm512_mask_getexp_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    transmute(vgetexpps(
        a.as_f32x16(),
        src.as_f32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getexp_ps&expand=2846)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps))]
pub unsafe fn _mm512_maskz_getexp_ps(k: __mmask16, a: __m512) -> __m512 {
    transmute(vgetexpps(
        a.as_f32x16(),
        _mm512_setzero_ps().as_f32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst. This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getexp_pd&expand=2835)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd))]
pub unsafe fn _mm512_getexp_pd(a: __m512d) -> __m512d {
    transmute(vgetexppd(
        a.as_f64x8(),
        _mm512_setzero_pd().as_f64x8(),
        0b11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getexp_pd&expand=2836)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd))]
pub unsafe fn _mm512_mask_getexp_pd(src: __m512d, k: __mmask8, a: __m512d) -> __m512d {
    transmute(vgetexppd(
        a.as_f64x8(),
        src.as_f64x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getexp_pd&expand=2837)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd))]
pub unsafe fn _mm512_maskz_getexp_pd(k: __mmask8, a: __m512d) -> __m512d {
    transmute(vgetexppd(
        a.as_f64x8(),
        _mm512_setzero_pd().as_f64x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst. This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getmant_ps&expand=2880)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0))]
#[rustc_args_required_const(1, 2)]
pub unsafe fn _mm512_getmant_ps(
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4,
                _mm512_setzero_ps().as_f32x16(),
                0b11111111_11111111,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getmant_ps&expand=2881)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0))]
#[rustc_args_required_const(3, 4)]
pub unsafe fn _mm512_mask_getmant_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4,
                src.as_f32x16(),
                k,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getmant_ps&expand=2882)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0))]
#[rustc_args_required_const(2, 3)]
pub unsafe fn _mm512_maskz_getmant_ps(
    k: __mmask16,
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4,
                _mm512_setzero_ps().as_f32x16(),
                k,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst. This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getmant_pd&expand=2871)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0))]
#[rustc_args_required_const(1, 2)]
pub unsafe fn _mm512_getmant_pd(
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4,
                _mm512_setzero_pd().as_f64x8(),
                0b11111111,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getmant_pd&expand=2872)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0))]
#[rustc_args_required_const(3, 4)]
pub unsafe fn _mm512_mask_getmant_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4,
                src.as_f64x8(),
                k,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getmant_pd&expand=2873)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0))]
#[rustc_args_required_const(2, 3)]
pub unsafe fn _mm512_maskz_getmant_pd(
    k: __mmask8,
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr, $imm2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4,
                _mm512_setzero_pd().as_f64x8(),
                k,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm4_mantissas!(norm, sign, call);
    transmute(r)
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_round_ps&expand=145)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_add_round_ps(a: __m512, b: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vaddps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_round_ps&expand=146)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_add_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vaddps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let addround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, addround, src.as_f32x16()))
}

/// Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_round_ps&expand=147)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddps, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_add_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vaddps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let addround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, addround, zero))
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_add_round_pd&expand=142)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_add_round_pd(a: __m512d, b: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vaddpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_add_round_pd&expand=143)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_add_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vaddpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let addround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, addround, src.as_f64x8()))
}

/// Add packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_add_round_pd&expand=144)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vaddpd, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_add_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vaddpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let addround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, addround, zero))
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_round_ps&expand=5739)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_sub_round_ps(a: __m512, b: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsubps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_round_ps&expand=5737)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_sub_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsubps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let subround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, subround, src.as_f32x16()))
}

/// Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_round_ps&expand=5738)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubps, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_sub_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsubps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let subround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, subround, zero))
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sub_round_pd&expand=5736)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_sub_round_pd(a: __m512d, b: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsubpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sub_round_pd&expand=5734)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_sub_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsubpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let subround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, subround, src.as_f64x8()))
}

/// Subtract packed double-precision (64-bit) floating-point elements in b from packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sub_round_pd&expand=5735)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsubpd, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_sub_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsubpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let subround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, subround, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mul_round_ps&expand=3940)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_mul_round_ps(a: __m512, b: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmulps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_round_ps&expand=3938)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_mul_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmulps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let mulround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, mulround, src.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_round_ps&expand=3939)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulps, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_mul_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmulps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let mulround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, mulround, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mul_round_pd&expand=3937)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_mul_round_pd(a: __m512d, b: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmulpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_mul_round_pd&expand=3935)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_mul_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmulpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let mulround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, mulround, src.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_mul_round_ps&expand=3939)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmulpd, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_mul_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmulpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let mulround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, mulround, zero))
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_div_round_ps&expand=2168)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_div_round_ps(a: __m512, b: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vdivps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_div_round_ps&expand=2169)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_div_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vdivps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let divround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, divround, src.as_f32x16()))
}

/// Divide packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_div_round_ps&expand=2170)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivps, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_div_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vdivps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let divround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, divround, zero))
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, =and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_div_round_pd&expand=2165)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_div_round_pd(a: __m512d, b: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vdivpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_div_round_pd&expand=2166)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd, rounding = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_div_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vdivpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let divround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, divround, src.as_f64x8()))
}

/// Divide packed double-precision (64-bit) floating-point elements in a by packed elements in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_div_round_pd&expand=2167)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vdivpd, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_div_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vdivpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let divround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, divround, zero))
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sqrt_round_ps&expand=5377)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps, rounding = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_sqrt_round_ps(a: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtps(a.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sqrt_round_ps&expand=5375)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_sqrt_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtps(a.as_f32x16(), $imm4)
        };
    }
    let sqrtround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, sqrtround, src.as_f32x16()))
}

/// Compute the square root of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sqrt_round_ps&expand=5376)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtps, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_sqrt_round_ps(k: __mmask16, a: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtps(a.as_f32x16(), $imm4)
        };
    }
    let sqrtround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, sqrtround, zero))
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sqrt_round_pd&expand=5374)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd, rounding = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_sqrt_round_pd(a: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtpd(a.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sqrt_round_pd&expand=5372)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_sqrt_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtpd(a.as_f64x8(), $imm4)
        };
    }
    let sqrtround = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, sqrtround, src.as_f64x8()))
}

/// Compute the square root of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sqrt_round_pd&expand=5373)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vsqrtpd, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_sqrt_round_pd(k: __mmask8, a: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vsqrtpd(a.as_f64x8(), $imm4)
        };
    }
    let sqrtround = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, sqrtround, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmadd_round_ps&expand=2565)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmadd_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmadd_round_ps&expand=2566)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmadd_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in a using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmadd_round_ps&expand=2568)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmadd_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmadd_round_ps&expand=2567)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132ps or vfmadd213ps or vfmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmadd_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmadd_round_pd&expand=2561)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmadd_round_pd(a: __m512d, b: __m512d, c: __m512d, rounding: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmadd_round_pd&expand=2562)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmadd_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmadd_round_pd&expand=2564)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmadd_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmadd_round_pd&expand=2563)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmadd132pd or vfmadd213pd or vfmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmadd_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsub_round_ps&expand=2651)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generates vfmadd, gcc generates vfmsub
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmsub_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsub_round_ps&expand=2652)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generates vfmadd, gcc generates vfmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmsub_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsub_round_ps&expand=2654)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generates vfmadd, gcc generates vfmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmsub_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsub_round_ps&expand=2653)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132ps or vfmsub213ps or vfmsub231ps, clang generates vfmadd, gcc generates vfmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmsub_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsub_round_pd&expand=2647)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang generates fmadd, gcc generates fmsub
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmsub_round_pd(a: __m512d, b: __m512d, c: __m512d, rounding: i32) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsub_round_pd&expand=2648)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang generates fmadd, gcc generates fmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmsub_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsub_round_pd&expand=2650)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang generates fmadd, gcc generates fmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmsub_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsub_round_pd&expand=2649)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfmsub132pd or vfmsub213pd or vfmsub231pd. clang generates fmadd, gcc generates fmsub
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmsub_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsub, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmaddsub_round_ps&expand=2619)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmaddsub_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmaddsub_round_ps&expand=2620)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmaddsub_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmaddsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmaddsub_round_ps&expand=2622)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmaddsub_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, fmaddsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmaddsub_round_ps&expand=2621)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132ps or vfmaddsub213ps or vfmaddsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmaddsub_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmaddsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmaddsub_round_pd&expand=2615)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmaddsub_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmaddsub_round_pd&expand=2616)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmaddsub_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmaddsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmaddsub_round_pd&expand=2618)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmaddsub_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, fmaddsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmaddsub_round_pd&expand=2617)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmaddsub132pd or vfmaddsub213pd or vfmaddsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmaddsub_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fmaddsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmaddsub, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsubadd_round_ps&expand=2699)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmsubadd_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsubadd_round_ps&expand=2700)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmsubadd_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsubadd_round_ps&expand=2702)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmsubadd_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsubadd_round_ps&expand=2701)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132ps or vfmsubadd213ps or vfmsubadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmsubadd_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213ps(a.as_f32x16(), b.as_f32x16(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fmsubadd_round_pd&expand=2695)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fmsubadd_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fmsubadd_round_pd&expand=2696)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fmsubadd_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively add and subtract packed elements in c to/from the intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fmsubadd_round_pd&expand=2698)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fmsubadd_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, alternatively subtract and add packed elements in c from/to the intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fmsubadd_round_pd&expand=2697)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmaddsub, rounding = 8))] //vfmsubadd132pd or vfmsubadd213pd or vfmsubadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fmsubadd_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmaddsub213pd(a.as_f64x8(), b.as_f64x8(), sub, $imm4)
        };
    }
    let fmsubadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fmsubadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmadd_round_ps&expand=2731)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fnmadd_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(sub, b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmadd_round_ps&expand=2732)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fnmadd_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(sub, b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmadd_round_ps&expand=2734)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fnmadd_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(sub, b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmadd_round_ps&expand=2733)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132ps or vfnmadd213ps or vfnmadd231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fnmadd_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(sub, b.as_f32x16(), c.as_f32x16(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmadd_pd&expand=2711)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fnmadd_round_pd(a: __m512d, b: __m512d, c: __m512d, rounding: i32) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(sub, b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmadd_round_pd&expand=2728)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fnmadd_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(sub, b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmadd_round_pd&expand=2730)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fnmadd_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(sub, b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, add the negated intermediate result to packed elements in c, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmadd_round_pd&expand=2729)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmadd132pd or vfnmadd213pd or vfnmadd231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fnmadd_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let sub = simd_sub(zero, a.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(sub, b.as_f64x8(), c.as_f64x8(), $imm4)
        };
    }
    let fnmadd = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmadd, c.as_f64x8()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmsub_round_ps&expand=2779)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fnmsub_round_ps(a: __m512, b: __m512, c: __m512, rounding: i32) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f32x16());
    let subc = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(suba, b.as_f32x16(), subc, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmsub_round_ps&expand=2780)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fnmsub_round_ps(
    a: __m512,
    k: __mmask16,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f32x16());
    let subc = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(suba, b.as_f32x16(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, a.as_f32x16()))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmsub_round_ps&expand=2782)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fnmsub_round_ps(
    k: __mmask16,
    a: __m512,
    b: __m512,
    c: __m512,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f32x16());
    let subc = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(suba, b.as_f32x16(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, zero))
}

/// Multiply packed single-precision (32-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmsub_round_ps&expand=2781)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132ps or vfnmsub213ps or vfnmsub231ps
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fnmsub_round_ps(
    a: __m512,
    b: __m512,
    c: __m512,
    k: __mmask16,
    rounding: i32,
) -> __m512 {
    let zero: f32x16 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f32x16());
    let subc = simd_sub(zero, c.as_f32x16());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132ps(suba, b.as_f32x16(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, c.as_f32x16()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_fnmsub_round_pd&expand=2775)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_fnmsub_round_pd(a: __m512d, b: __m512d, c: __m512d, rounding: i32) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f64x8());
    let subc = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(suba, b.as_f64x8(), subc, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_fnmsub_round_pd&expand=2776)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_fnmsub_round_pd(
    a: __m512d,
    k: __mmask8,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f64x8());
    let subc = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(suba, b.as_f64x8(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, a.as_f64x8()))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_fnmsub_round_pd&expand=2778)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_maskz_fnmsub_round_pd(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    c: __m512d,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f64x8());
    let subc = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(suba, b.as_f64x8(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, zero))
}

/// Multiply packed double-precision (64-bit) floating-point elements in a and b, subtract packed elements in c from the negated intermediate result, and store the results in dst using writemask k (elements are copied from c when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask3_fnmsub_round_pd&expand=2777)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vfmadd, rounding = 8))] //vfnmsub132pd or vfnmsub213pd or vfnmsub231pd
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask3_fnmsub_round_pd(
    a: __m512d,
    b: __m512d,
    c: __m512d,
    k: __mmask8,
    rounding: i32,
) -> __m512d {
    let zero: f64x8 = mem::zeroed();
    let suba = simd_sub(zero, a.as_f64x8());
    let subc = simd_sub(zero, c.as_f64x8());
    macro_rules! call {
        ($imm4:expr) => {
            vfmadd132pd(suba, b.as_f64x8(), subc, $imm4)
        };
    }
    let fnmsub = constify_imm4_round!(rounding, call);
    transmute(simd_select_bitmask(k, fnmsub, c.as_f64x8()))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=max_round_ps&expand=3662)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_max_round_ps(a: __m512, b: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_round_ps&expand=3660)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps, sae = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_max_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    transmute(simd_select_bitmask(k, max, src.as_f32x16()))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_round_ps&expand=3661)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxps, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_max_round_ps(k: __mmask16, a: __m512, b: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_max_round_pd&expand=3659)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_max_round_pd(a: __m512d, b: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_max_round_pd&expand=3657)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd, sae = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_max_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    transmute(simd_select_bitmask(k, max, src.as_f64x8()))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed maximum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_max_round_pd&expand=3658)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmaxpd, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_max_round_pd(k: __mmask8, a: __m512d, b: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vmaxpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_round_ps&expand=3776)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_min_round_ps(a: __m512, b: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vminps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_round_ps&expand=3774)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps, sae = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_min_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vminps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    transmute(simd_select_bitmask(k, max, src.as_f32x16()))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_round_ps&expand=3775)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminps, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_min_round_ps(k: __mmask16, a: __m512, b: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vminps(a.as_f32x16(), b.as_f32x16(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_min_round_pd&expand=3773)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_min_round_pd(a: __m512d, b: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vminpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_min_round_pd&expand=3771)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd, sae = 8))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_min_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vminpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    transmute(simd_select_bitmask(k, max, src.as_f64x8()))
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b, and store packed minimum values in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_min_round_pd&expand=3772)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vminpd, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_min_round_pd(k: __mmask8, a: __m512d, b: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vminpd(a.as_f64x8(), b.as_f64x8(), $imm4)
        };
    }
    let max = constify_imm4_sae!(sae, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, max, zero))
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst. This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getexp_round_ps&expand=2850)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_getexp_round_ps(a: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexpps(
                a.as_f32x16(),
                _mm512_setzero_ps().as_f32x16(),
                0b11111111_11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getexp_round_ps&expand=2851)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_getexp_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexpps(a.as_f32x16(), src.as_f32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert the exponent of each packed single-precision (32-bit) floating-point element in a to a single-precision (32-bit) floating-point number representing the integer exponent, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getexp_round_ps&expand=2852)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexpps, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_getexp_round_ps(k: __mmask16, a: __m512, sae: i32) -> __m512 {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexpps(a.as_f32x16(), _mm512_setzero_ps().as_f32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst. This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getexp_round_pd&expand=2847)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_getexp_round_pd(a: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexppd(
                a.as_f64x8(),
                _mm512_setzero_pd().as_f64x8(),
                0b11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getexp_round_pd&expand=2848)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_getexp_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexppd(a.as_f64x8(), src.as_f64x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert the exponent of each packed double-precision (64-bit) floating-point element in a to a double-precision (64-bit) floating-point number representing the integer exponent, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates floor(log2(x)) for each element.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getexp_round_pd&expand=2849)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetexppd, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_getexp_round_pd(k: __mmask8, a: __m512d, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vgetexppd(a.as_f64x8(), _mm512_setzero_pd().as_f64x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst. This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getmant_round_ps&expand=2886)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(1, 2, 3)]
pub unsafe fn _mm512_getmant_round_ps(
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4_1,
                _mm512_setzero_ps().as_f32x16(),
                0b11111111_11111111,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getmant_round_ps&expand=2887)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(3, 4, 5)]
pub unsafe fn _mm512_mask_getmant_round_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4_1,
                src.as_f32x16(),
                k,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed single-precision (32-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getmant_round_ps&expand=2888)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantps, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(2, 3, 4)]
pub unsafe fn _mm512_maskz_getmant_round_ps(
    k: __mmask16,
    a: __m512,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512 {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantps(
                a.as_f32x16(),
                $imm2 << 2 | $imm4_1,
                _mm512_setzero_ps().as_f32x16(),
                k,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst. This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_getmant_round_pd&expand=2883)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(1, 2, 3)]
pub unsafe fn _mm512_getmant_round_pd(
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4_1,
                _mm512_setzero_pd().as_f64x8(),
                0b11111111,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_getmant_round_pd&expand=2884)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(3, 4, 5)]
pub unsafe fn _mm512_mask_getmant_round_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4_1,
                src.as_f64x8(),
                k,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Normalize the mantissas of packed double-precision (64-bit) floating-point elements in a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set). This intrinsic essentially calculates ±(2^k)*|x.significand|, where k depends on the interval range defined by interv and the sign depends on sc and the source sign.
/// The mantissa is normalized to the interval specified by interv, which can take the following values:
///    _MM_MANT_NORM_1_2     // interval [1, 2)
///    _MM_MANT_NORM_p5_2    // interval [0.5, 2)
///    _MM_MANT_NORM_p5_1    // interval [0.5, 1)
///    _MM_MANT_NORM_p75_1p5 // interval [0.75, 1.5)
/// The sign is determined by sc which can take the following values:
///    _MM_MANT_SIGN_src     // sign = sign(src)
///    _MM_MANT_SIGN_zero    // sign = 0
///    _MM_MANT_SIGN_nan     // dst = NaN if sign(src) = 1
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_getmant_round_pd&expand=2885)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgetmantpd, norm = 0, sign = 0, sae = 4))]
#[rustc_args_required_const(2, 3, 4)]
pub unsafe fn _mm512_maskz_getmant_round_pd(
    k: __mmask8,
    a: __m512d,
    norm: _MM_MANTISSA_NORM_ENUM,
    sign: _MM_MANTISSA_SIGN_ENUM,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4_1:expr, $imm2:expr, $imm4_2:expr) => {
            vgetmantpd(
                a.as_f64x8(),
                $imm2 << 2 | $imm4_1,
                _mm512_setzero_pd().as_f64x8(),
                k,
                $imm4_2,
            )
        };
    }
    let r = constify_imm4_mantissas_sae!(norm, sign, sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=cvtps_epi32&expand=1737)   
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq))]
pub unsafe fn _mm512_cvtps_epi32(a: __m512) -> __m512i {
    transmute(vcvtps2dq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        0b11111111_11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtps_epi32&expand=1738)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq))]
pub unsafe fn _mm512_mask_cvtps_epi32(src: __m512i, k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvtps2dq(
        a.as_f32x16(),
        src.as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtps_epi32&expand=1739)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq))]
pub unsafe fn _mm512_maskz_cvtps_epi32(k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvtps2dq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtps_epu32&expand=1755)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq))]
pub unsafe fn _mm512_cvtps_epu32(a: __m512) -> __m512i {
    transmute(vcvtps2udq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_u32x16(),
        0b11111111_11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtps_epu32&expand=1756)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq))]
pub unsafe fn _mm512_mask_cvtps_epu32(src: __m512i, k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvtps2udq(
        a.as_f32x16(),
        src.as_u32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=maskz_cvt_roundps_epu32&expand=1343)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq))]
pub unsafe fn _mm512_maskz_cvtps_epu32(k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvtps2udq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_u32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtps_pd&expand=1769)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd))]
pub unsafe fn _mm512_cvtps_pd(a: __m256) -> __m512d {
    transmute(vcvtps2pd(
        a.as_f32x8(),
        _mm512_setzero_pd().as_f64x8(),
        0b11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtps_pd&expand=1770)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd))]
pub unsafe fn _mm512_mask_cvtps_pd(src: __m512d, k: __mmask8, a: __m256) -> __m512d {
    transmute(vcvtps2pd(
        a.as_f32x8(),
        src.as_f64x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtps_pd&expand=1771)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd))]
pub unsafe fn _mm512_maskz_cvtps_pd(k: __mmask8, a: __m256) -> __m512d {
    transmute(vcvtps2pd(
        a.as_f32x8(),
        _mm512_setzero_pd().as_f64x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvt_roundps_epi32&expand=1335)   
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq, rounding = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvt_roundps_epi32(a: __m512, rounding: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2dq(
                a.as_f32x16(),
                _mm512_setzero_si512().as_i32x16(),
                0b11111111_11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvt_roundps_epi32&expand=1336)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvt_roundps_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512,
    rounding: i32,
) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2dq(a.as_f32x16(), src.as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvt_roundps_epi32&expand=1337)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2dq, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvt_roundps_epi32(k: __mmask16, a: __m512, rounding: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2dq(a.as_f32x16(), _mm512_setzero_si512().as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst.
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvt_roundps_epu32&expand=1341)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq, rounding = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvt_roundps_epu32(a: __m512, rounding: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2udq(
                a.as_f32x16(),
                _mm512_setzero_si512().as_u32x16(),
                0b11111111_11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvt_roundps_epu32&expand=1342)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq, rounding = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvt_roundps_epu32(
    src: __m512i,
    k: __mmask16,
    a: __m512,
    rounding: i32,
) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2udq(a.as_f32x16(), src.as_u32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// Rounding is done according to the rounding\[3:0\] parameter, which can be one of:
///    (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC) // round to nearest, and suppress exceptions
///    (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)     // round down, and suppress exceptions
///    (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)     // round up, and suppress exceptions
///    (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)        // truncate, and suppress exceptions
///    _MM_FROUND_CUR_DIRECTION // use MXCSR.RC; see _MM_SET_ROUNDING_MODE
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=maskz_cvt_roundps_epu32&expand=1343)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2udq, rounding = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvt_roundps_epu32(k: __mmask16, a: __m512, rounding: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2udq(a.as_f32x16(), _mm512_setzero_si512().as_u32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_round!(rounding, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=cvt_roundps_pd&expand=1347)   
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvt_roundps_pd(a: __m256, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2pd(
                a.as_f32x8(),
                _mm512_setzero_pd().as_f64x8(),
                0b11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvt_roundps_epi32&expand=1336)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvt_roundps_pd(
    src: __m512d,
    k: __mmask8,
    a: __m256,
    sae: i32,
) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2pd(a.as_f32x8(), src.as_f64x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed double-precision (64-bit) floating-point elements, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvt_roundps_epi32&expand=1337)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvtps2pd, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvt_roundps_pd(k: __mmask8, a: __m256, sae: i32) -> __m512d {
    macro_rules! call {
        ($imm4:expr) => {
            vcvtps2pd(a.as_f32x8(), _mm512_setzero_pd().as_f64x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtt_roundps_epi32&expand=1916)   
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvtt_roundps_epi32(a: __m512, sae: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2dq(
                a.as_f32x16(),
                _mm512_setzero_si512().as_i32x16(),
                0b11111111_11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtt_roundps_epi32&expand=1917)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvtt_roundps_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512,
    sae: i32,
) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2dq(a.as_f32x16(), src.as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtt_roundps_epi32&expand=1918)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvtt_roundps_epi32(k: __mmask16, a: __m512, sae: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2dq(a.as_f32x16(), _mm512_setzero_si512().as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtt_roundps_epu32&expand=1922)   
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvtt_roundps_epu32(a: __m512, sae: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2udq(
                a.as_f32x16(),
                _mm512_setzero_si512().as_i32x16(),
                0b11111111_11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtt_roundps_epu32&expand=1923)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvtt_roundps_epu32(
    src: __m512i,
    k: __mmask16,
    a: __m512,
    sae: i32,
) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2udq(a.as_f32x16(), src.as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtt_roundps_epu32&expand=1924)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvtt_roundps_epu32(k: __mmask16, a: __m512, sae: i32) -> __m512i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttps2udq(a.as_f32x16(), _mm512_setzero_si512().as_i32x16(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtt_roundpd_epi32&expand=1904)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvtt_roundpd_epi32(a: __m512d, sae: i32) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2dq(
                a.as_f64x8(),
                _mm256_setzero_si256().as_i32x8(),
                0b11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtt_roundpd_epi32&expand=1905)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvtt_roundpd_epi32(
    src: __m256i,
    k: __mmask8,
    a: __m512d,
    sae: i32,
) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2dq(a.as_f64x8(), src.as_i32x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtt_roundps_epi32&expand=1918)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvtt_roundpd_epi32(k: __mmask8, a: __m512d, sae: i32) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2dq(a.as_f64x8(), _mm256_setzero_si256().as_i32x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst.
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///    
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvtt_roundpd_epu32&expand=1910)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq, sae = 8))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_cvtt_roundpd_epu32(a: __m512d, sae: i32) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2udq(
                a.as_f64x8(),
                _mm256_setzero_si256().as_i32x8(),
                0b11111111,
                $imm4,
            )
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvtt_roundpd_epu32&expand=1911)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq, sae = 8))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_cvtt_roundpd_epu32(
    src: __m256i,
    k: __mmask8,
    a: __m512d,
    sae: i32,
) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2udq(a.as_f64x8(), src.as_i32x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst.    
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvttps_epi32&expand=1984)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq))]
pub unsafe fn _mm512_cvttps_epi32(a: __m512) -> __m512i {
    transmute(vcvttps2dq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        0b11111111_11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvttps_epi32&expand=1985)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq))]
pub unsafe fn _mm512_mask_cvttps_epi32(src: __m512i, k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvttps2dq(
        a.as_f32x16(),
        src.as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvttps_epi32&expand=1986)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2dq))]
pub unsafe fn _mm512_maskz_cvttps_epi32(k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvttps2dq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed single-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst.    
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvttps_epu32&expand=2002)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq))]
pub unsafe fn _mm512_cvttps_epu32(a: __m512) -> __m512i {
    transmute(vcvttps2udq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        0b11111111_11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvttps_epu32&expand=2003)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq))]
pub unsafe fn _mm512_mask_cvttps_epu32(src: __m512i, k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvttps2udq(
        a.as_f32x16(),
        src.as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (32-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvttps_epu32&expand=2004)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttps2udq))]
pub unsafe fn _mm512_maskz_cvttps_epu32(k: __mmask16, a: __m512) -> __m512i {
    transmute(vcvttps2udq(
        a.as_f32x16(),
        _mm512_setzero_si512().as_i32x16(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
/// Exceptions can be suppressed by passing _MM_FROUND_NO_EXC in the sae parameter.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvtt_roundpd_epu32&expand=1912)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq, sae = 8))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_cvtt_roundpd_epu32(k: __mmask8, a: __m512d, sae: i32) -> __m256i {
    macro_rules! call {
        ($imm4:expr) => {
            vcvttpd2udq(a.as_f64x8(), _mm256_setzero_si256().as_i32x8(), k, $imm4)
        };
    }
    let r = constify_imm4_sae!(sae, call);
    transmute(r)
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst.  
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvttpd_epi32&expand=1947)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq))]
pub unsafe fn _mm512_cvttpd_epi32(a: __m512d) -> __m256i {
    transmute(vcvttpd2dq(
        a.as_f64x8(),
        _mm256_setzero_si256().as_i32x8(),
        0b11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvttpd_epi32&expand=1948)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq))]
pub unsafe fn _mm512_mask_cvttpd_epi32(src: __m256i, k: __mmask8, a: __m512d) -> __m256i {
    transmute(vcvttpd2dq(
        a.as_f64x8(),
        src.as_i32x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvttpd_epi32&expand=1949)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2dq))]
pub unsafe fn _mm512_maskz_cvttpd_epi32(k: __mmask8, a: __m512d) -> __m256i {
    transmute(vcvttpd2dq(
        a.as_f64x8(),
        _mm256_setzero_si256().as_i32x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst.    
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_cvttpd_epu32&expand=1965)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq))]
pub unsafe fn _mm512_cvttpd_epu32(a: __m512d) -> __m256i {
    transmute(vcvttpd2udq(
        a.as_f64x8(),
        _mm256_setzero_si256().as_i32x8(),
        0b11111111,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_cvttpd_epu32&expand=1966)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq))]
pub unsafe fn _mm512_mask_cvttpd_epu32(src: __m256i, k: __mmask8, a: __m512d) -> __m256i {
    transmute(vcvttpd2udq(
        a.as_f64x8(),
        src.as_i32x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Convert packed double-precision (64-bit) floating-point elements in a to packed unsigned 32-bit integers with truncation, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_cvttpd_epu32&expand=1967)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcvttpd2udq))]
pub unsafe fn _mm512_maskz_cvttpd_epu32(k: __mmask8, a: __m512d) -> __m256i {
    transmute(vcvttpd2udq(
        a.as_f64x8(),
        _mm256_setzero_si256().as_i32x8(),
        k,
        _MM_FROUND_CUR_DIRECTION,
    ))
}

/// Returns vector of type `__m512d` with all elements set to zero.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990&text=_mm512_setzero_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vxorps))]
pub unsafe fn _mm512_setzero_pd() -> __m512d {
    // All-0 is a properly initialized __m512d
    mem::zeroed()
}

/// Returns vector of type `__m512d` with all elements set to zero.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990&text=_mm512_setzero_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vxorps))]
pub unsafe fn _mm512_setzero_ps() -> __m512 {
    // All-0 is a properly initialized __m512
    mem::zeroed()
}

/// Returns vector of type `__m512i` with all elements set to zero.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#avx512techs=AVX512F&expand=33,34,4990&text=_mm512_setzero_si512)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vxorps))]
pub unsafe fn _mm512_setzero_si512() -> __m512i {
    // All-0 is a properly initialized __m512i
    mem::zeroed()
}

/// Sets packed 32-bit integers in `dst` with the supplied values in reverse
/// order.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_setr_epi32(
    e15: i32,
    e14: i32,
    e13: i32,
    e12: i32,
    e11: i32,
    e10: i32,
    e9: i32,
    e8: i32,
    e7: i32,
    e6: i32,
    e5: i32,
    e4: i32,
    e3: i32,
    e2: i32,
    e1: i32,
    e0: i32,
) -> __m512i {
    let r = i32x16(
        e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0,
    );
    transmute(r)
}

/// Gather double-precision (64-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32gather_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherdpd, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i32gather_pd(offsets: __m256i, slice: *const u8, scale: i32) -> __m512d {
    let zero = _mm512_setzero_pd().as_f64x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherdpd(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather double-precision (64-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32gather_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherdpd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32gather_pd(
    src: __m512d,
    mask: __mmask8,
    offsets: __m256i,
    slice: *const u8,
    scale: i32,
) -> __m512d {
    let src = src.as_f64x8();
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherdpd(src, slice, offsets, mask as i8, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather double-precision (64-bit) floating-point elements from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64gather_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherqpd, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i64gather_pd(offsets: __m512i, slice: *const u8, scale: i32) -> __m512d {
    let zero = _mm512_setzero_pd().as_f64x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherqpd(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather double-precision (64-bit) floating-point elements from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64gather_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherqpd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64gather_pd(
    src: __m512d,
    mask: __mmask8,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m512d {
    let src = src.as_f64x8();
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherqpd(src, slice, offsets, mask as i8, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather single-precision (32-bit) floating-point elements from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64gather_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherqps, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i64gather_ps(offsets: __m512i, slice: *const u8, scale: i32) -> __m256 {
    let zero = _mm256_setzero_ps().as_f32x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherqps(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather single-precision (32-bit) floating-point elements from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64gather_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherqps, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64gather_ps(
    src: __m256,
    mask: __mmask8,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m256 {
    let src = src.as_f32x8();
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherqps(src, slice, offsets, mask as i8, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather single-precision (32-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32gather_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherdps, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i32gather_ps(offsets: __m512i, slice: *const u8, scale: i32) -> __m512 {
    let zero = _mm512_setzero_ps().as_f32x16();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherdps(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather single-precision (32-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32gather_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vgatherdps, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32gather_ps(
    src: __m512,
    mask: __mmask16,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m512 {
    let src = src.as_f32x16();
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vgatherdps(src, slice, offsets, mask as i16, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 32-bit integers from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32gather_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherdd, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i32gather_epi32(offsets: __m512i, slice: *const u8, scale: i32) -> __m512i {
    let zero = _mm512_setzero_si512().as_i32x16();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherdd(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 32-bit integers from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32gather_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherdd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32gather_epi32(
    src: __m512i,
    mask: __mmask16,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m512i {
    let src = src.as_i32x16();
    let mask = mask as i16;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherdd(src, slice, offsets, mask, $imm8)
        };
    }
    let r = constify_imm8!(scale, call);
    transmute(r)
}

/// Gather 64-bit integers from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32gather_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherdq, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i32gather_epi64(offsets: __m256i, slice: *const u8, scale: i32) -> __m512i {
    let zero = _mm512_setzero_si512().as_i64x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherdq(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 64-bit integers from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32gather_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherdq, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32gather_epi64(
    src: __m512i,
    mask: __mmask8,
    offsets: __m256i,
    slice: *const u8,
    scale: i32,
) -> __m512i {
    let src = src.as_i64x8();
    let mask = mask as i8;
    let slice = slice as *const i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherdq(src, slice, offsets, mask, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 64-bit integers from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64gather_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherqq, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i64gather_epi64(offsets: __m512i, slice: *const u8, scale: i32) -> __m512i {
    let zero = _mm512_setzero_si512().as_i64x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherqq(zero, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 64-bit integers from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64gather_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherqq, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64gather_epi64(
    src: __m512i,
    mask: __mmask8,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m512i {
    let src = src.as_i64x8();
    let mask = mask as i8;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherqq(src, slice, offsets, mask, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 32-bit integers from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64gather_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherqd, scale = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_i64gather_epi32(offsets: __m512i, slice: *const u8, scale: i32) -> __m256i {
    let zeros = _mm256_setzero_si256().as_i32x8();
    let neg_one = -1;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherqd(zeros, slice, offsets, neg_one, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Gather 32-bit integers from memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64gather_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpgatherqd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64gather_epi32(
    src: __m256i,
    mask: __mmask8,
    offsets: __m512i,
    slice: *const u8,
    scale: i32,
) -> __m256i {
    let src = src.as_i32x8();
    let mask = mask as i8;
    let slice = slice as *const i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpgatherqd(src, slice, offsets, mask, $imm8)
        };
    }
    let r = constify_imm8_gather!(scale, call);
    transmute(r)
}

/// Scatter double-precision (64-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32scatter_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterdpd, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i32scatter_pd(slice: *mut u8, offsets: __m256i, src: __m512d, scale: i32) {
    let src = src.as_f64x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterdpd(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter double-precision (64-bit) floating-point elements from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32scatter_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterdpd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32scatter_pd(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m256i,
    src: __m512d,
    scale: i32,
) {
    let src = src.as_f64x8();
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterdpd(slice, mask as i8, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter double-precision (64-bit) floating-point elements from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64scatter_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterqpd, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i64scatter_pd(slice: *mut u8, offsets: __m512i, src: __m512d, scale: i32) {
    let src = src.as_f64x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterqpd(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter double-precision (64-bit) floating-point elements from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64scatter_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterqpd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64scatter_pd(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m512i,
    src: __m512d,
    scale: i32,
) {
    let src = src.as_f64x8();
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterqpd(slice, mask as i8, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter single-precision (32-bit) floating-point elements from memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32scatter_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterdps, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i32scatter_ps(slice: *mut u8, offsets: __m512i, src: __m512, scale: i32) {
    let src = src.as_f32x16();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterdps(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter single-precision (32-bit) floating-point elements from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32scatter_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterdps, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32scatter_ps(
    slice: *mut u8,
    mask: __mmask16,
    offsets: __m512i,
    src: __m512,
    scale: i32,
) {
    let src = src.as_f32x16();
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterdps(slice, mask as i16, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter single-precision (32-bit) floating-point elements from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64scatter_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterqps, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i64scatter_ps(slice: *mut u8, offsets: __m512i, src: __m256, scale: i32) {
    let src = src.as_f32x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterqps(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter single-precision (32-bit) floating-point elements from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64scatter_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vscatterqps, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64scatter_ps(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m512i,
    src: __m256,
    scale: i32,
) {
    let src = src.as_f32x8();
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vscatterqps(slice, mask as i8, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 64-bit integers from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i32scatter_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterdq, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i32scatter_epi64(slice: *mut u8, offsets: __m256i, src: __m512i, scale: i32) {
    let src = src.as_i64x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterdq(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 64-bit integers from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32scatter_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterdq, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32scatter_epi64(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m256i,
    src: __m512i,
    scale: i32,
) {
    let src = src.as_i64x8();
    let mask = mask as i8;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterdq(slice, mask, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 64-bit integers from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64scatter_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterqq, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i64scatter_epi64(slice: *mut u8, offsets: __m512i, src: __m512i, scale: i32) {
    let src = src.as_i64x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterqq(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 64-bit integers from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64scatter_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterqq, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64scatter_epi64(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m512i,
    src: __m512i,
    scale: i32,
) {
    let src = src.as_i64x8();
    let mask = mask as i8;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterqq(slice, mask, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 32-bit integers from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64scatter_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterdd, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i32scatter_epi32(slice: *mut u8, offsets: __m512i, src: __m512i, scale: i32) {
    let src = src.as_i32x16();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterdd(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 32-bit integers from src into memory using 32-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i32scatter_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterdd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i32scatter_epi32(
    slice: *mut u8,
    mask: __mmask16,
    offsets: __m512i,
    src: __m512i,
    scale: i32,
) {
    let src = src.as_i32x16();
    let mask = mask as i16;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i32x16();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterdd(slice, mask, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 32-bit integers from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_i64scatter_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterqd, scale = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_i64scatter_epi32(slice: *mut u8, offsets: __m512i, src: __m256i, scale: i32) {
    let src = src.as_i32x8();
    let neg_one = -1;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterqd(slice, neg_one, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Scatter 32-bit integers from src into memory using 64-bit indices.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_mask_i64scatter_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpscatterqd, scale = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_i64scatter_epi32(
    slice: *mut u8,
    mask: __mmask8,
    offsets: __m512i,
    src: __m256i,
    scale: i32,
) {
    let src = src.as_i32x8();
    let mask = mask as i8;
    let slice = slice as *mut i8;
    let offsets = offsets.as_i64x8();
    macro_rules! call {
        ($imm8:expr) => {
            vpscatterqd(slice, mask, offsets, src, $imm8)
        };
    }
    constify_imm8_gather!(scale, call);
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rol_epi32&expand=4685)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_rol_epi32(a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprold(a.as_i32x16(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rol_epi32&expand=4683)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_rol_epi32(src: __m512i, k: __mmask16, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprold(a.as_i32x16(), $imm8)
        };
    }
    let rol = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, rol, src.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rol_epi32&expand=4684)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_rol_epi32(k: __mmask16, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprold(a.as_i32x16(), $imm8)
        };
    }
    let rol = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, rol, zero))
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_ror_epi32&expand=4721)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_ror_epi32(a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprord(a.as_i32x16(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_ror_epi32&expand=4719)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 123))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_ror_epi32(src: __m512i, k: __mmask16, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprord(a.as_i32x16(), $imm8)
        };
    }
    let ror = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, ror, src.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_ror_epi32&expand=4720)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprold, imm8 = 123))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_ror_epi32(k: __mmask16, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprord(a.as_i32x16(), $imm8)
        };
    }
    let ror = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, ror, zero))
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rol_epi64&expand=4694)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_rol_epi64(a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprolq(a.as_i64x8(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rol_epi64&expand=4692)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_rol_epi64(src: __m512i, k: __mmask8, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprolq(a.as_i64x8(), $imm8)
        };
    }
    let rol = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, rol, src.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rol_epi64&expand=4693)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_rol_epi64(k: __mmask8, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprolq(a.as_i64x8(), $imm8)
        };
    }
    let rol = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, rol, zero))
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_ror_epi64&expand=4730)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 15))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_ror_epi64(a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprorq(a.as_i64x8(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_ror_epi64&expand=4728)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 15))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_ror_epi64(src: __m512i, k: __mmask8, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprorq(a.as_i64x8(), $imm8)
        };
    }
    let ror = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, ror, src.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_ror_epi64&expand=4729)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolq, imm8 = 15))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_ror_epi64(k: __mmask8, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vprorq(a.as_i64x8(), $imm8)
        };
    }
    let ror = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, ror, zero))
}

/// Shift packed 32-bit integers in a left by imm8 while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_slli_epi32&expand=5310)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld, imm8 = 5))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_slli_epi32(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsllid(a.as_i32x16(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 32-bit integers in a left by imm8 while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_slli_epi32&expand=5308)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld, imm8 = 5))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_slli_epi32(src: __m512i, k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsllid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a left by imm8 while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_slli_epi32&expand=5309)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld, imm8 = 5))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_slli_epi32(k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsllid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srli_epi32&expand=5522)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_srli_epi32(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrlid(a.as_i32x16(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srli_epi32&expand=5520)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_srli_epi32(src: __m512i, k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrlid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srli_epi32&expand=5521)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_srli_epi32(k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrlid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a left by imm8 while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_slli_epi64&expand=5319)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq, imm8 = 5))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_slli_epi64(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpslliq(a.as_i64x8(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 64-bit integers in a left by imm8 while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_slli_epi64&expand=5317)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq, imm8 = 5))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_slli_epi64(src: __m512i, k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpslliq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a left by imm8 while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_slli_epi64&expand=5318)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq, imm8 = 5))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_slli_epi64(k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpslliq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srli_epi64&expand=5531)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_srli_epi64(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrliq(a.as_i64x8(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srli_epi64&expand=5529)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_srli_epi64(src: __m512i, k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrliq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srli_epi64&expand=5530)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_srli_epi64(k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsrliq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a left by count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sll_epi32&expand=5280)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld))]
pub unsafe fn _mm512_sll_epi32(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpslld(a.as_i32x16(), count.as_i32x4()))
}

/// Shift packed 32-bit integers in a left by count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sll_epi32&expand=5278)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld))]
pub unsafe fn _mm512_mask_sll_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_sll_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a left by count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sll_epi32&expand=5279)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpslld))]
pub unsafe fn _mm512_maskz_sll_epi32(k: __mmask16, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_sll_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srl_epi32&expand=5492)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld))]
pub unsafe fn _mm512_srl_epi32(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpsrld(a.as_i32x16(), count.as_i32x4()))
}

/// Shift packed 32-bit integers in a right by count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srl_epi32&expand=5490)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld))]
pub unsafe fn _mm512_mask_srl_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_srl_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srl_epi32&expand=5491)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrld))]
pub unsafe fn _mm512_maskz_srl_epi32(k: __mmask16, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_srl_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a left by count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sll_epi64&expand=5289)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq))]
pub unsafe fn _mm512_sll_epi64(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpsllq(a.as_i64x8(), count.as_i64x2()))
}

/// Shift packed 64-bit integers in a left by count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sll_epi64&expand=5287)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq))]
pub unsafe fn _mm512_mask_sll_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_sll_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a left by count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sll_epi64&expand=5288)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllq))]
pub unsafe fn _mm512_maskz_sll_epi64(k: __mmask8, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_sll_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srl_epi64&expand=5501)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq))]
pub unsafe fn _mm512_srl_epi64(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpsrlq(a.as_i64x8(), count.as_i64x2()))
}

/// Shift packed 64-bit integers in a right by count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srl_epi64&expand=5499)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq))]
pub unsafe fn _mm512_mask_srl_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_srl_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a left by count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sll_epi64&expand=5288)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlq))]
pub unsafe fn _mm512_maskz_srl_epi64(k: __mmask8, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_srl_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by count while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sra_epi32&expand=5407)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad))]
pub unsafe fn _mm512_sra_epi32(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpsrad(a.as_i32x16(), count.as_i32x4()))
}

/// Shift packed 32-bit integers in a right by count while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sra_epi32&expand=5405)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad))]
pub unsafe fn _mm512_mask_sra_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_sra_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by count while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sra_epi32&expand=5406)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad))]
pub unsafe fn _mm512_maskz_sra_epi32(k: __mmask16, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_sra_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by count while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sra_epi64&expand=5416)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq))]
pub unsafe fn _mm512_sra_epi64(a: __m512i, count: __m128i) -> __m512i {
    transmute(vpsraq(a.as_i64x8(), count.as_i64x2()))
}

/// Shift packed 64-bit integers in a right by count while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sra_epi64&expand=5414)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq))]
pub unsafe fn _mm512_mask_sra_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m128i,
) -> __m512i {
    let shf = _mm512_sra_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by count while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sra_epi64&expand=5415)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq))]
pub unsafe fn _mm512_maskz_sra_epi64(k: __mmask8, a: __m512i, count: __m128i) -> __m512i {
    let shf = _mm512_sra_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srai_epi32&expand=5436)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_srai_epi32(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraid(a.as_i32x16(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srai_epi32&expand=5434)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_srai_epi32(src: __m512i, k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srai_epi32&expand=5435)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrad, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_srai_epi32(k: __mmask16, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraid(a.as_i32x16(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srai_epi64&expand=5445)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_srai_epi64(a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraiq(a.as_i64x8(), $imm8)
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srai_epi64&expand=5443)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_srai_epi64(src: __m512i, k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraiq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by imm8 while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srai_epi64&expand=5444)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsraq, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_srai_epi64(k: __mmask8, a: __m512i, imm8: u32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpsraiq(a.as_i64x8(), $imm8)
        };
    }
    let shf = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srav_epi32&expand=5465)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravd))]
pub unsafe fn _mm512_srav_epi32(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsravd(a.as_i32x16(), count.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srav_epi32&expand=5463)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravd))]
pub unsafe fn _mm512_mask_srav_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_srav_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srav_epi32&expand=5464)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravd))]
pub unsafe fn _mm512_maskz_srav_epi32(k: __mmask16, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_srav_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srav_epi64&expand=5474)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravq))]
pub unsafe fn _mm512_srav_epi64(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsravq(a.as_i64x8(), count.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srav_epi64&expand=5472)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravq))]
pub unsafe fn _mm512_mask_srav_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_srav_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in sign bits, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srav_epi64&expand=5473)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsravq))]
pub unsafe fn _mm512_maskz_srav_epi64(k: __mmask8, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_srav_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rolv_epi32&expand=4703)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvd))]
pub unsafe fn _mm512_rolv_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vprolvd(a.as_i32x16(), b.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rolv_epi32&expand=4701)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvd))]
pub unsafe fn _mm512_mask_rolv_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let rol = _mm512_rolv_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, rol, src.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rolv_epi32&expand=4702)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvd))]
pub unsafe fn _mm512_maskz_rolv_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let rol = _mm512_rolv_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, rol, zero))
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rorv_epi32&expand=4739)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvd))]
pub unsafe fn _mm512_rorv_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(vprorvd(a.as_i32x16(), b.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rorv_epi32&expand=4737)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvd))]
pub unsafe fn _mm512_mask_rorv_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let ror = _mm512_rorv_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, ror, src.as_i32x16()))
}

/// Rotate the bits in each packed 32-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rorv_epi32&expand=4738)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvd))]
pub unsafe fn _mm512_maskz_rorv_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let ror = _mm512_rorv_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, ror, zero))
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rolv_epi64&expand=4712)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvq))]
pub unsafe fn _mm512_rolv_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vprolvq(a.as_i64x8(), b.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rolv_epi64&expand=4710)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvq))]
pub unsafe fn _mm512_mask_rolv_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let rol = _mm512_rolv_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, rol, src.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the left by the number of bits specified in the corresponding element of b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rolv_epi64&expand=4711)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprolvq))]
pub unsafe fn _mm512_maskz_rolv_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let rol = _mm512_rolv_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, rol, zero))
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_rorv_epi64&expand=4748)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvq))]
pub unsafe fn _mm512_rorv_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(vprorvq(a.as_i64x8(), b.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_rorv_epi64&expand=4746)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvq))]
pub unsafe fn _mm512_mask_rorv_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let ror = _mm512_rorv_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, ror, src.as_i64x8()))
}

/// Rotate the bits in each packed 64-bit integer in a to the right by the number of bits specified in the corresponding element of b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_rorv_epi64&expand=4747)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vprorvq))]
pub unsafe fn _mm512_maskz_rorv_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let ror = _mm512_rorv_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, ror, zero))
}

/// Shift packed 32-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sllv_epi32&expand=5342)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvd))]
pub unsafe fn _mm512_sllv_epi32(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsllvd(a.as_i32x16(), count.as_i32x16()))
}

/// Shift packed 32-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sllv_epi32&expand=5340)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvd))]
pub unsafe fn _mm512_mask_sllv_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_sllv_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sllv_epi32&expand=5341)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvd))]
pub unsafe fn _mm512_maskz_sllv_epi32(k: __mmask16, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_sllv_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srlv_epi32&expand=5554)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvd))]
pub unsafe fn _mm512_srlv_epi32(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsrlvd(a.as_i32x16(), count.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_srlv_epi32&expand=5552)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvd))]
pub unsafe fn _mm512_mask_srlv_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_srlv_epi32(a, count).as_i32x16();
    transmute(simd_select_bitmask(k, shf, src.as_i32x16()))
}

/// Shift packed 32-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srlv_epi32&expand=5553)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvd))]
pub unsafe fn _mm512_maskz_srlv_epi32(k: __mmask16, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_srlv_epi32(a, count).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_sllv_epi64&expand=5351)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvq))]
pub unsafe fn _mm512_sllv_epi64(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsllvq(a.as_i64x8(), count.as_i64x8()))
}

/// Shift packed 64-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_sllv_epi64&expand=5349)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvq))]
pub unsafe fn _mm512_mask_sllv_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_sllv_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a left by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_sllv_epi64&expand=5350)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsllvq))]
pub unsafe fn _mm512_maskz_sllv_epi64(k: __mmask8, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_sllv_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_srlv_epi64&expand=5563)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvq))]
pub unsafe fn _mm512_srlv_epi64(a: __m512i, count: __m512i) -> __m512i {
    transmute(vpsrlvq(a.as_i64x8(), count.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=mask_srlv_epi64&expand=5561)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvq))]
pub unsafe fn _mm512_mask_srlv_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    count: __m512i,
) -> __m512i {
    let shf = _mm512_srlv_epi64(a, count).as_i64x8();
    transmute(simd_select_bitmask(k, shf, src.as_i64x8()))
}

/// Shift packed 64-bit integers in a right by the amount specified by the corresponding element in count while shifting in zeros, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_srlv_epi64&expand=5562)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpsrlvq))]
pub unsafe fn _mm512_maskz_srlv_epi64(k: __mmask8, a: __m512i, count: __m512i) -> __m512i {
    let shf = _mm512_srlv_epi64(a, count).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shf, zero))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permute_ps&expand=4170)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps, imm8 = 1))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_permute_ps(a: __m512, imm8: i32) -> __m512 {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilps(a.as_f32x16(), _mm512_set1_epi32($imm8).as_i32x16())
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permute_ps&expand=4168)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_permute_ps(src: __m512, k: __mmask16, a: __m512, imm8: i32) -> __m512 {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilps(a.as_f32x16(), _mm512_set1_epi32($imm8).as_i32x16())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, permute, src.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permute_ps&expand=4169)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_permute_ps(k: __mmask16, a: __m512, imm8: i32) -> __m512 {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilps(a.as_f32x16(), _mm512_set1_epi32($imm8).as_i32x16())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permute_pd&expand=4161)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd, imm8 = 2))]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_permute_pd(a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permute_pd&expand=4159)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd, imm8 = 2))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_permute_pd(src: __m512d, k: __mmask8, a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, permute, src.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permute_pd&expand=4160)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd, imm8 = 2))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_permute_pd(k: __mmask8, a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermilpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 64-bit integers in a within 256-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex_epi64&expand=4208)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcast, imm8 = 0b11111111))]
//shoud be vpermq, but generate vpermpd. It generates vpermq with mask. change to vbroadcast becaise CI Windows
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_permutex_epi64(a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpermq(a.as_i64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shuffle 64-bit integers in a within 256-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex_epi64&expand=4206)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast, imm8 = 0b11111111))] //shoud be vpermq. change to vpbroadcast becaise CI Windows
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_permutex_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    imm8: i32,
) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpermq(a.as_i64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, permute, src.as_i64x8()))
}

/// Shuffle 64-bit integers in a within 256-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex_epi64&expand=4207)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast, imm8 = 0b11111111))] //shoud be vpermq. change to vpbroadcast becaise CI Windows
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_permutex_epi64(k: __mmask8, a: __m512i, imm8: i32) -> __m512i {
    macro_rules! call {
        ($imm8:expr) => {
            vpermq(a.as_i64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 256-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex_pd&expand=4214)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcast, imm8 = 0b11111111))] //shoud be vpermpd. change to vbroadcast becaise CI Windows
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_permutex_pd(a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let r = constify_imm8_sae!(imm8, call);
    transmute(r)
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 256-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex_pd&expand=4212)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcast, imm8 = 0b11111111))] //shoud be vpermpd. change to vbroadcast becaise CI Windows
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_permutex_pd(src: __m512d, k: __mmask8, a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    transmute(simd_select_bitmask(k, permute, src.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 256-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex_pd&expand=4213)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcast, imm8 = 0b11111111))] //shoud be vpermpd. change to vbroadcast becaise CI Windows
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_permutex_pd(k: __mmask8, a: __m512d, imm8: i32) -> __m512d {
    macro_rules! call {
        ($imm8:expr) => {
            vpermpd(a.as_f64x8(), _mm512_set1_epi64($imm8).as_i64x8())
        };
    }
    let permute = constify_imm8_sae!(imm8, call);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst. Note that this intrinsic shuffles across 128-bit lanes, unlike past intrinsics that use the permutevar name. This intrinsic is identical to _mm512_permutexvar_epi32, and it is recommended that you use that intrinsic name.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutevar_epi32&expand=4182)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //should be vpermd, but generate vpermps. It generates vpermd with mask
pub unsafe fn _mm512_permutevar_epi32(idx: __m512i, a: __m512i) -> __m512i {
    transmute(vpermd(a.as_i32x16(), idx.as_i32x16()))
}

/// Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set). Note that this intrinsic shuffles across 128-bit lanes, unlike past intrinsics that use the permutevar name. This intrinsic is identical to _mm512_mask_permutexvar_epi32, and it is recommended that you use that intrinsic name.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutevar_epi32&expand=4181)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermd))]
pub unsafe fn _mm512_mask_permutevar_epi32(
    src: __m512i,
    k: __mmask16,
    idx: __m512i,
    a: __m512i,
) -> __m512i {
    let permute = _mm512_permutevar_epi32(idx, a).as_i32x16();
    transmute(simd_select_bitmask(k, permute, src.as_i32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutevar_ps&expand=4200)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps))]
pub unsafe fn _mm512_permutevar_ps(a: __m512, b: __m512i) -> __m512 {
    transmute(vpermilps(a.as_f32x16(), b.as_i32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutevar_ps&expand=4198)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps))]
pub unsafe fn _mm512_mask_permutevar_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512i,
) -> __m512 {
    let permute = _mm512_permutevar_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, permute, src.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutevar_ps&expand=4199)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps))]
pub unsafe fn _mm512_maskz_permutevar_ps(k: __mmask16, a: __m512, b: __m512i) -> __m512 {
    let permute = _mm512_permutevar_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutevar_pd&expand=4191)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd))]
pub unsafe fn _mm512_permutevar_pd(a: __m512d, b: __m512i) -> __m512d {
    transmute(vpermilpd(a.as_f64x8(), b.as_i64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutevar_pd&expand=4189)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd))]
pub unsafe fn _mm512_mask_permutevar_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512i,
) -> __m512d {
    let permute = _mm512_permutevar_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, permute, src.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a within 128-bit lanes using the control in b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutevar_pd&expand=4190)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilpd))]
pub unsafe fn _mm512_maskz_permutevar_pd(k: __mmask8, a: __m512d, b: __m512i) -> __m512d {
    let permute = _mm512_permutevar_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutexvar_epi32&expand=4301)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //should be vpermd, but generate vpermps. It generates vpermd with mask
pub unsafe fn _mm512_permutexvar_epi32(idx: __m512i, a: __m512i) -> __m512i {
    transmute(vpermd(a.as_i32x16(), idx.as_i32x16()))
}

/// Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutexvar_epi32&expand=4299)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermd))]
pub unsafe fn _mm512_mask_permutexvar_epi32(
    src: __m512i,
    k: __mmask16,
    idx: __m512i,
    a: __m512i,
) -> __m512i {
    let permute = _mm512_permutexvar_epi32(idx, a).as_i32x16();
    transmute(simd_select_bitmask(k, permute, src.as_i32x16()))
}

/// Shuffle 32-bit integers in a across lanes using the corresponding index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutexvar_epi32&expand=4300)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermd))]
pub unsafe fn _mm512_maskz_permutexvar_epi32(k: __mmask16, idx: __m512i, a: __m512i) -> __m512i {
    let permute = _mm512_permutexvar_epi32(idx, a).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 64-bit integers in a across lanes using the corresponding index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutexvar_epi64&expand=4307)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //should be vpermq, but generate vpermpd. It generates vpermd with mask
pub unsafe fn _mm512_permutexvar_epi64(idx: __m512i, a: __m512i) -> __m512i {
    transmute(vpermq(a.as_i64x8(), idx.as_i64x8()))
}

/// Shuffle 64-bit integers in a across lanes using the corresponding index in idx, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutexvar_epi64&expand=4305)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermq))]
pub unsafe fn _mm512_mask_permutexvar_epi64(
    src: __m512i,
    k: __mmask8,
    idx: __m512i,
    a: __m512i,
) -> __m512i {
    let permute = _mm512_permutexvar_epi64(idx, a).as_i64x8();
    transmute(simd_select_bitmask(k, permute, src.as_i64x8()))
}

/// Shuffle 64-bit integers in a across lanes using the corresponding index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutexvar_epi64&expand=4306)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermq))]
pub unsafe fn _mm512_maskz_permutexvar_epi64(k: __mmask8, idx: __m512i, a: __m512i) -> __m512i {
    let permute = _mm512_permutexvar_epi64(idx, a).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle single-precision (32-bit) floating-point elements in a across lanes using the corresponding index in idx.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutevar_ps&expand=4200)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermps))]
pub unsafe fn _mm512_permutexvar_ps(idx: __m512i, a: __m512) -> __m512 {
    transmute(vpermps(a.as_f32x16(), idx.as_i32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a across lanes using the corresponding index in idx, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutexvar_ps&expand=4326)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermps))]
pub unsafe fn _mm512_mask_permutexvar_ps(
    src: __m512,
    k: __mmask16,
    idx: __m512i,
    a: __m512,
) -> __m512 {
    let permute = _mm512_permutexvar_ps(idx, a).as_f32x16();
    transmute(simd_select_bitmask(k, permute, src.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a across lanes using the corresponding index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutexvar_ps&expand=4327)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermps))]
pub unsafe fn _mm512_maskz_permutexvar_ps(k: __mmask16, idx: __m512i, a: __m512) -> __m512 {
    let permute = _mm512_permutexvar_ps(idx, a).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a across lanes using the corresponding index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutexvar_pd&expand=4322)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermpd))]
pub unsafe fn _mm512_permutexvar_pd(idx: __m512i, a: __m512d) -> __m512d {
    transmute(vpermpd(a.as_f64x8(), idx.as_i64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a across lanes using the corresponding index in idx, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutexvar_pd&expand=4320)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermpd))]
pub unsafe fn _mm512_mask_permutexvar_pd(
    src: __m512d,
    k: __mmask8,
    idx: __m512i,
    a: __m512d,
) -> __m512d {
    let permute = _mm512_permutexvar_pd(idx, a).as_f64x8();
    transmute(simd_select_bitmask(k, permute, src.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a across lanes using the corresponding index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutexvar_pd&expand=4321)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermpd))]
pub unsafe fn _mm512_maskz_permutexvar_pd(k: __mmask8, idx: __m512i, a: __m512d) -> __m512d {
    let permute = _mm512_permutexvar_pd(idx, a).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 32-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex2var_epi32&expand=4238)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2d or vpermt2d
pub unsafe fn _mm512_permutex2var_epi32(a: __m512i, idx: __m512i, b: __m512i) -> __m512i {
    transmute(vpermi2d(a.as_i32x16(), idx.as_i32x16(), b.as_i32x16()))
}

/// Shuffle 32-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex2var_epi32&expand=4235)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermt2d))]
pub unsafe fn _mm512_mask_permutex2var_epi32(
    a: __m512i,
    k: __mmask16,
    idx: __m512i,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi32(a, idx, b).as_i32x16();
    transmute(simd_select_bitmask(k, permute, a.as_i32x16()))
}

/// Shuffle 32-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex2var_epi32&expand=4237)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2d or vpermt2d
pub unsafe fn _mm512_maskz_permutex2var_epi32(
    k: __mmask16,
    a: __m512i,
    idx: __m512i,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi32(a, idx, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 32-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from idx when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask2_permutex2var_epi32&expand=4236)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermi2d))]
pub unsafe fn _mm512_mask2_permutex2var_epi32(
    a: __m512i,
    idx: __m512i,
    k: __mmask16,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi32(a, idx, b).as_i32x16();
    transmute(simd_select_bitmask(k, permute, idx.as_i32x16()))
}

/// Shuffle 64-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex2var_epi64&expand=4250)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2q or vpermt2q
pub unsafe fn _mm512_permutex2var_epi64(a: __m512i, idx: __m512i, b: __m512i) -> __m512i {
    transmute(vpermi2q(a.as_i64x8(), idx.as_i64x8(), b.as_i64x8()))
}

/// Shuffle 64-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex2var_epi64&expand=4247)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermt2q))]
pub unsafe fn _mm512_mask_permutex2var_epi64(
    a: __m512i,
    k: __mmask8,
    idx: __m512i,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi64(a, idx, b).as_i64x8();
    transmute(simd_select_bitmask(k, permute, a.as_i64x8()))
}

/// Shuffle 64-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex2var_epi64&expand=4249)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2q or vpermt2q
pub unsafe fn _mm512_maskz_permutex2var_epi64(
    k: __mmask8,
    a: __m512i,
    idx: __m512i,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi64(a, idx, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle 64-bit integers in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from idx when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask2_permutex2var_epi64&expand=4248)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermi2q))]
pub unsafe fn _mm512_mask2_permutex2var_epi64(
    a: __m512i,
    idx: __m512i,
    k: __mmask8,
    b: __m512i,
) -> __m512i {
    let permute = _mm512_permutex2var_epi64(a, idx, b).as_i64x8();
    transmute(simd_select_bitmask(k, permute, idx.as_i64x8()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex2var_ps&expand=4286)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2ps or vpermt2ps
pub unsafe fn _mm512_permutex2var_ps(a: __m512, idx: __m512i, b: __m512) -> __m512 {
    transmute(vpermi2ps(a.as_f32x16(), idx.as_i32x16(), b.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex2var_ps&expand=4283)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermt2ps))]
pub unsafe fn _mm512_mask_permutex2var_ps(
    a: __m512,
    k: __mmask16,
    idx: __m512i,
    b: __m512,
) -> __m512 {
    let permute = _mm512_permutex2var_ps(a, idx, b).as_f32x16();
    transmute(simd_select_bitmask(k, permute, a.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex2var_ps&expand=4285)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2ps or vpermt2ps
pub unsafe fn _mm512_maskz_permutex2var_ps(
    k: __mmask16,
    a: __m512,
    idx: __m512i,
    b: __m512,
) -> __m512 {
    let permute = _mm512_permutex2var_ps(a, idx, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle single-precision (32-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from idx when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask2_permutex2var_ps&expand=4284)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //should be vpermi2ps, but it shows vpermt2ps
pub unsafe fn _mm512_mask2_permutex2var_ps(
    a: __m512,
    idx: __m512i,
    k: __mmask16,
    b: __m512,
) -> __m512 {
    let permute = _mm512_permutex2var_ps(a, idx, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_permutex2var_pd&expand=4274)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2pd or vpermt2pd
pub unsafe fn _mm512_permutex2var_pd(a: __m512d, idx: __m512i, b: __m512d) -> __m512d {
    transmute(vpermi2pd(a.as_f64x8(), idx.as_i64x8(), b.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from a when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_permutex2var_pd&expand=4271)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermt2pd))]
pub unsafe fn _mm512_mask_permutex2var_pd(
    a: __m512d,
    k: __mmask8,
    idx: __m512i,
    b: __m512d,
) -> __m512d {
    let permute = _mm512_permutex2var_pd(a, idx, b).as_f64x8();
    transmute(simd_select_bitmask(k, permute, a.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_permutex2var_pd&expand=4273)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //vpermi2pd or vpermt2pd
pub unsafe fn _mm512_maskz_permutex2var_pd(
    k: __mmask8,
    a: __m512d,
    idx: __m512i,
    b: __m512d,
) -> __m512d {
    let permute = _mm512_permutex2var_pd(a, idx, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements in a and b across lanes using the corresponding selector and index in idx, and store the results in dst using writemask k (elements are copied from idx when the corresponding mask bit is not set)
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask2_permutex2var_pd&expand=4272)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vperm))] //should be vpermi2pd, but it shows vpermt2pd
pub unsafe fn _mm512_mask2_permutex2var_pd(
    a: __m512d,
    idx: __m512i,
    k: __mmask8,
    b: __m512d,
) -> __m512d {
    let permute = _mm512_permutex2var_pd(a, idx, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, permute, zero))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_epi32&expand=5150)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpermilps, imm8 = 9))] //should be vpshufd, but generate vpermilps
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_shuffle_epi32(a: __m512i, imm8: _MM_PERM_ENUM) -> __m512i {
    let imm8 = (imm8 & 0xFF) as u8;

    let a = a.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                a,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    let r: i32x16 = match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    };
    transmute(r)
}

/// Shuffle 32-bit integers in a within 128-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_epi32&expand=5148)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpshufd, imm8 = 9))] //should be vpshufd, but generate vpermilps
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_mask_shuffle_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    imm8: _MM_PERM_ENUM,
) -> __m512i {
    let imm8 = (imm8 & 0xFF) as u8;

    let a = a.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                a,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    let shuffle: i32x16 = match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    };
    transmute(simd_select_bitmask(k, shuffle, src.as_i32x16()))
}

/// Shuffle 32-bit integers in a within 128-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_epi32&expand=5149)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpshufd, imm8 = 9))] //should be vpshufd, but generate vpermilps
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_maskz_shuffle_epi32(k: __mmask16, a: __m512i, imm8: _MM_PERM_ENUM) -> __m512i {
    let imm8 = (imm8 & 0xFF) as u8;

    let a = a.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                a,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    let shuffle: i32x16 = match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    };
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_ps&expand=5203)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufps, imm8 = 0))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_ps(a: __m512, b: __m512, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    }
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_ps&expand=5201)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufps, imm8 = 0))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_ps(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    imm8: i32,
) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_f32x16()))
}

/// Shuffle single-precision (32-bit) floating-point elements in a within 128-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_ps&expand=5202)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufps, imm8 = 0))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_ps(k: __mmask16, a: __m512, b: __m512, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, 16, $e, $f, $g, 20, $i, $j, $k, 24, $m, $n, $o, 28),
                1 => shuffle4!($a, $b, $c, 17, $e, $f, $g, 21, $i, $j, $k, 25, $m, $n, $o, 29),
                2 => shuffle4!($a, $b, $c, 18, $e, $f, $g, 22, $i, $j, $k, 26, $m, $n, $o, 30),
                _ => shuffle4!($a, $b, $c, 19, $e, $f, $g, 23, $i, $j, $k, 27, $m, $n, $o, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, 16, $e, $f, 20, $i, $j, 24, $m, $n, 28),
                1 => shuffle3!($a, $b, 17, $e, $f, 21, $i, $j, 25, $m, $n, 29),
                2 => shuffle3!($a, $b, 18, $e, $f, 22, $i, $j, 26, $m, $n, 30),
                _ => shuffle3!($a, $b, 19, $e, $f, 23, $i, $j, 27, $m, $n, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, 0, $e, 4, $i, 8, $m, 12),
                1 => shuffle2!($a, 1, $e, 5, $i, 9, $m, 13),
                2 => shuffle2!($a, 2, $e, 6, $i, 10, $m, 14),
                _ => shuffle2!($a, 3, $e, 7, $i, 11, $m, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 4, 8, 12),
        1 => shuffle1!(1, 5, 9, 13),
        2 => shuffle1!(2, 6, 10, 14),
        _ => shuffle1!(3, 7, 11, 15),
    };

    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle double-precision (64-bit) floating-point elements within 128-bit lanes using the control in imm8, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_pd&expand=5192)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufpd, imm8 = 3))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_pd(a: __m512d, b: __m512d, imm8: i32) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle8 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr, $h:expr) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle7 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 7) & 0x1 {
                0 => shuffle8!($a, $b, $c, $d, $e, $f, $g, 14),
                _ => shuffle8!($a, $b, $c, $d, $e, $f, $g, 15),
            }
        };
    }
    macro_rules! shuffle6 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr) => {
            match (imm8 >> 6) & 0x1 {
                0 => shuffle7!($a, $b, $c, $d, $e, $f, 6),
                _ => shuffle7!($a, $b, $c, $d, $e, $f, 7),
            }
        };
    }
    macro_rules! shuffle5 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr) => {
            match (imm8 >> 5) & 0x1 {
                0 => shuffle6!($a, $b, $c, $d, $e, 12),
                _ => shuffle6!($a, $b, $c, $d, $e, 13),
            }
        };
    }
    macro_rules! shuffle4 {
        ($a:expr, $b:expr, $c:expr, $d:expr) => {
            match (imm8 >> 4) & 0x1 {
                0 => shuffle5!($a, $b, $c, $d, 4),
                _ => shuffle5!($a, $b, $c, $d, 5),
            }
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr) => {
            match (imm8 >> 3) & 0x1 {
                0 => shuffle4!($a, $b, $c, 10),
                _ => shuffle4!($a, $b, $c, 11),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr) => {
            match (imm8 >> 2) & 0x1 {
                0 => shuffle3!($a, $b, 2),
                _ => shuffle3!($a, $b, 3),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr) => {
            match (imm8 >> 1) & 0x1 {
                0 => shuffle2!($a, 8),
                _ => shuffle2!($a, 9),
            }
        };
    }
    match imm8 & 0x1 {
        0 => shuffle1!(0),
        _ => shuffle1!(1),
    }
}

/// Shuffle double-precision (64-bit) floating-point elements within 128-bit lanes using the control in imm8, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_pd&expand=5190)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufpd, imm8 = 3))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    imm8: i32,
) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle8 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr, $h:expr) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle7 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 7) & 0x1 {
                0 => shuffle8!($a, $b, $c, $d, $e, $f, $g, 14),
                _ => shuffle8!($a, $b, $c, $d, $e, $f, $g, 15),
            }
        };
    }
    macro_rules! shuffle6 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr) => {
            match (imm8 >> 6) & 0x1 {
                0 => shuffle7!($a, $b, $c, $d, $e, $f, 6),
                _ => shuffle7!($a, $b, $c, $d, $e, $f, 7),
            }
        };
    }
    macro_rules! shuffle5 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr) => {
            match (imm8 >> 5) & 0x1 {
                0 => shuffle6!($a, $b, $c, $d, $e, 12),
                _ => shuffle6!($a, $b, $c, $d, $e, 13),
            }
        };
    }
    macro_rules! shuffle4 {
        ($a:expr, $b:expr, $c:expr, $d:expr) => {
            match (imm8 >> 4) & 0x1 {
                0 => shuffle5!($a, $b, $c, $d, 4),
                _ => shuffle5!($a, $b, $c, $d, 5),
            }
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr) => {
            match (imm8 >> 3) & 0x1 {
                0 => shuffle4!($a, $b, $c, 10),
                _ => shuffle4!($a, $b, $c, 11),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr) => {
            match (imm8 >> 2) & 0x1 {
                0 => shuffle3!($a, $b, 2),
                _ => shuffle3!($a, $b, 3),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr) => {
            match (imm8 >> 1) & 0x1 {
                0 => shuffle2!($a, 8),
                _ => shuffle2!($a, 9),
            }
        };
    }
    let shuffle = match imm8 & 0x1 {
        0 => shuffle1!(0),
        _ => shuffle1!(1),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_f64x8()))
}

/// Shuffle double-precision (64-bit) floating-point elements within 128-bit lanes using the control in imm8, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_pd&expand=5191)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufpd, imm8 = 3))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_pd(k: __mmask8, a: __m512d, b: __m512d, imm8: i32) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle8 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr, $h:expr) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle7 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 7) & 0x1 {
                0 => shuffle8!($a, $b, $c, $d, $e, $f, $g, 14),
                _ => shuffle8!($a, $b, $c, $d, $e, $f, $g, 15),
            }
        };
    }
    macro_rules! shuffle6 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr) => {
            match (imm8 >> 6) & 0x1 {
                0 => shuffle7!($a, $b, $c, $d, $e, $f, 6),
                _ => shuffle7!($a, $b, $c, $d, $e, $f, 7),
            }
        };
    }
    macro_rules! shuffle5 {
        ($a:expr, $b:expr, $c:expr, $d:expr, $e:expr) => {
            match (imm8 >> 5) & 0x1 {
                0 => shuffle6!($a, $b, $c, $d, $e, 12),
                _ => shuffle6!($a, $b, $c, $d, $e, 13),
            }
        };
    }
    macro_rules! shuffle4 {
        ($a:expr, $b:expr, $c:expr, $d:expr) => {
            match (imm8 >> 4) & 0x1 {
                0 => shuffle5!($a, $b, $c, $d, 4),
                _ => shuffle5!($a, $b, $c, $d, 5),
            }
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr) => {
            match (imm8 >> 3) & 0x1 {
                0 => shuffle4!($a, $b, $c, 10),
                _ => shuffle4!($a, $b, $c, 11),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr) => {
            match (imm8 >> 2) & 0x1 {
                0 => shuffle3!($a, $b, 2),
                _ => shuffle3!($a, $b, 3),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr) => {
            match (imm8 >> 1) & 0x1 {
                0 => shuffle2!($a, 8),
                _ => shuffle2!($a, 9),
            }
        };
    }
    let shuffle = match imm8 & 0x1 {
        0 => shuffle1!(0),
        _ => shuffle1!(1),
    };

    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle 128-bits (composed of 4 32-bit integers) selected by imm8 from a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_i32&expand=5177)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi64x2, imm8 = 0b10111111))] //should be vshufi32x4, but generate vshufi64x2
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_i32x4(a: __m512i, b: __m512i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    let a = a.as_i32x16();
    let b = b.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    let r: i32x16 = match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    };

    transmute(r)
}

/// Shuffle 128-bits (composed of 4 32-bit integers) selected by imm8 from a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_i32x&expand=5175)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi32x4, imm8 = 0b10111111))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_i32x4(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    let a = a.as_i32x16();
    let b = b.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_i32x16()))
}

/// Shuffle 128-bits (composed of 4 32-bit integers) selected by imm8 from a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_i32&expand=5176)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi32x4, imm8 = 0b10111111))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_i32x4(
    k: __mmask16,
    a: __m512i,
    b: __m512i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    let a = a.as_i32x16();
    let b = b.as_i32x16();
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    };

    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle 128-bits (composed of 2 64-bit integers) selected by imm8 from a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_i64x2&expand=5183)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_i64x2(a: __m512i, b: __m512i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    }
}

/// Shuffle 128-bits (composed of 2 64-bit integers) selected by imm8 from a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_i64x&expand=5181)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_i64x2(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m512i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_i64x8()))
}

/// Shuffle 128-bits (composed of 2 64-bit integers) selected by imm8 from a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_i64&expand=5182)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshufi64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_i64x2(
    k: __mmask8,
    a: __m512i,
    b: __m512i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    };

    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle 128-bits (composed of 4 single-precision (32-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_f32x4&expand=5165)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff64x2, imm8 = 0b10111111))] //should be vshuff32x4, but generate vshuff64x2
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_f32x4(a: __m512, b: __m512, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    }
}

/// Shuffle 128-bits (composed of 4 single-precision (32-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_f32&expand=5163)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff32x4, imm8 = 0b10111111))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_f32x4(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m512,
    imm8: i32,
) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_f32x16()))
}

/// Shuffle 128-bits (composed of 4 single-precision (32-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_f32&expand=5164)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff32x4, imm8 = 0b10111111))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_f32x4(k: __mmask16, a: __m512, b: __m512, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr,
            $i:expr,
            $j:expr,
            $k:expr,
            $l:expr,
            $m:expr,
            $n:expr,
            $o:expr,
            $p:expr
        ) => {
            simd_shuffle16(
                a,
                b,
                [
                    $a, $b, $c, $d, $e, $f, $g, $h, $i, $j, $k, $l, $m, $n, $o, $p,
                ],
            );
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr, $i:expr, $j:expr, $k:expr, $m:expr, $n:expr, $o:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 16, 17, 18, 19),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 20, 21, 22, 23),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 24, 25, 26, 27),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, $i, $j, $k, $m, $n, $o, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr, $i:expr, $j:expr, $m:expr, $n:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 16, 17, 18, 19),
                1 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 20, 21, 22, 23),
                2 => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 24, 25, 26, 27),
                _ => shuffle3!($a, $b, $e, $f, $i, $j, $m, $n, 28, 29, 30, 31),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr, $i: expr, $m: expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, $i, $m, 0, 1, 2, 3),
                1 => shuffle2!($a, $e, $i, $m, 4, 5, 6, 7),
                2 => shuffle2!($a, $e, $i, $m, 8, 9, 10, 11),
                _ => shuffle2!($a, $e, $i, $m, 12, 13, 14, 15),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1, 2, 3),
        1 => shuffle1!(4, 5, 6, 7),
        2 => shuffle1!(8, 9, 10, 11),
        _ => shuffle1!(12, 13, 14, 15),
    };

    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Shuffle 128-bits (composed of 2 double-precision (64-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_shuffle_f64x2&expand=5171)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_shuffle_f64x2(a: __m512d, b: __m512d, imm8: i32) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    }
}

/// Shuffle 128-bits (composed of 2 double-precision (64-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_shuffle_f64x2&expand=5169)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_shuffle_f64x2(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    imm8: i32,
) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    };

    transmute(simd_select_bitmask(k, shuffle, src.as_f64x8()))
}

/// Shuffle 128-bits (composed of 2 double-precision (64-bit) floating-point elements) selected by imm8 from a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_shuffle_f64x2&expand=5170)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vshuff64x2, imm8 = 0b10111111))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_shuffle_f64x2(
    k: __mmask8,
    a: __m512d,
    b: __m512d,
    imm8: i32,
) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 255);
    let imm8 = (imm8 & 0xFF) as u8;
    macro_rules! shuffle4 {
        (
            $a:expr,
            $b:expr,
            $c:expr,
            $d:expr,
            $e:expr,
            $f:expr,
            $g:expr,
            $h:expr
        ) => {
            simd_shuffle8(a, b, [$a, $b, $c, $d, $e, $f, $g, $h]);
        };
    }
    macro_rules! shuffle3 {
        ($a:expr, $b:expr, $c:expr, $e:expr, $f:expr, $g:expr) => {
            match (imm8 >> 6) & 0x3 {
                0 => shuffle4!($a, $b, $c, $e, $f, $g, 8, 9),
                1 => shuffle4!($a, $b, $c, $e, $f, $g, 10, 11),
                2 => shuffle4!($a, $b, $c, $e, $f, $g, 12, 13),
                _ => shuffle4!($a, $b, $c, $e, $f, $g, 14, 15),
            }
        };
    }
    macro_rules! shuffle2 {
        ($a:expr, $b:expr, $e:expr, $f:expr) => {
            match (imm8 >> 4) & 0x3 {
                0 => shuffle3!($a, $b, $e, $f, 8, 9),
                1 => shuffle3!($a, $b, $e, $f, 10, 11),
                2 => shuffle3!($a, $b, $e, $f, 12, 13),
                _ => shuffle3!($a, $b, $e, $f, 14, 15),
            }
        };
    }
    macro_rules! shuffle1 {
        ($a:expr, $e:expr) => {
            match (imm8 >> 2) & 0x3 {
                0 => shuffle2!($a, $e, 0, 1),
                1 => shuffle2!($a, $e, 2, 3),
                2 => shuffle2!($a, $e, 4, 5),
                _ => shuffle2!($a, $e, 6, 7),
            }
        };
    }
    let shuffle = match imm8 & 0x3 {
        0 => shuffle1!(0, 1),
        1 => shuffle1!(2, 3),
        2 => shuffle1!(4, 5),
        _ => shuffle1!(6, 7),
    };

    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, shuffle, zero))
}

/// Extract 128 bits (composed of 4 packed single-precision (32-bit) floating-point elements) from a, selected with imm8, and store the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_extractf32x4_ps&expand=2442)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(
    all(test, not(target_os = "windows")),
    assert_instr(vextractf32x4, imm8 = 3)
)]
#[rustc_args_required_const(1)]
pub unsafe fn _mm512_extractf32x4_ps(a: __m512, imm8: i32) -> __m128 {
    assert!(imm8 >= 0 && imm8 <= 3);
    match imm8 & 0x3 {
        0 => simd_shuffle4(a, _mm512_undefined_ps(), [0, 1, 2, 3]),
        1 => simd_shuffle4(a, _mm512_undefined_ps(), [4, 5, 6, 7]),
        2 => simd_shuffle4(a, _mm512_undefined_ps(), [8, 9, 10, 11]),
        _ => simd_shuffle4(a, _mm512_undefined_ps(), [12, 13, 14, 15]),
    }
}

/// Duplicate even-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_moveldup_ps&expand=3862)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovsldup))]
pub unsafe fn _mm512_moveldup_ps(a: __m512) -> __m512 {
    let r: f32x16 = simd_shuffle16(a, a, [0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14]);
    transmute(r)
}

/// Duplicate even-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_moveldup_ps&expand=3860)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovsldup))]
pub unsafe fn _mm512_mask_moveldup_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    let mov: f32x16 = simd_shuffle16(a, a, [0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14]);
    transmute(simd_select_bitmask(k, mov, src.as_f32x16()))
}

/// Duplicate even-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_moveldup_ps&expand=3861)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovsldup))]
pub unsafe fn _mm512_maskz_moveldup_ps(k: __mmask16, a: __m512) -> __m512 {
    let mov: f32x16 = simd_shuffle16(a, a, [0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14]);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, mov, zero))
}

/// Duplicate odd-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_movehdup_ps&expand=3852)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovshdup))]
pub unsafe fn _mm512_movehdup_ps(a: __m512) -> __m512 {
    let r: f32x16 = simd_shuffle16(a, a, [1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15]);
    transmute(r)
}

/// Duplicate odd-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_movehdup&expand=3850)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovshdup))]
pub unsafe fn _mm512_mask_movehdup_ps(src: __m512, k: __mmask16, a: __m512) -> __m512 {
    let mov: f32x16 = simd_shuffle16(a, a, [1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15]);
    transmute(simd_select_bitmask(k, mov, src.as_f32x16()))
}

/// Duplicate odd-indexed single-precision (32-bit) floating-point elements from a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_moveh&expand=3851)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovshdup))]
pub unsafe fn _mm512_maskz_movehdup_ps(k: __mmask16, a: __m512) -> __m512 {
    let mov: f32x16 = simd_shuffle16(a, a, [1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15]);
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, mov, zero))
}

/// Duplicate even-indexed double-precision (64-bit) floating-point elements from a, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_movedup_pd&expand=3843)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovddup))]
pub unsafe fn _mm512_movedup_pd(a: __m512d) -> __m512d {
    let r: f64x8 = simd_shuffle8(a, a, [0, 0, 2, 2, 4, 4, 6, 6]);
    transmute(r)
}

/// Duplicate even-indexed double-precision (64-bit) floating-point elements from a, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_movedup_pd&expand=3841)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovddup))]
pub unsafe fn _mm512_mask_movedup_pd(src: __m512d, k: __mmask8, a: __m512d) -> __m512d {
    let mov: f64x8 = simd_shuffle8(a, a, [0, 0, 2, 2, 4, 4, 6, 6]);
    transmute(simd_select_bitmask(k, mov, src.as_f64x8()))
}

/// Duplicate even-indexed double-precision (64-bit) floating-point elements from a, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_movedup_pd&expand=3842)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovddup))]
pub unsafe fn _mm512_maskz_movedup_pd(k: __mmask8, a: __m512d) -> __m512d {
    let mov: f64x8 = simd_shuffle8(a, a, [0, 0, 2, 2, 4, 4, 6, 6]);
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, mov, zero))
}

/// Copy a to dst, then insert 128 bits (composed of 4 packed 32-bit integers) from b into dst at the location specified by imm8.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_inserti32x4&expand=3174)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf32x4, imm8 = 2))] //should be vinserti32x4
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_inserti32x4(a: __m512i, b: __m128i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 3);
    let a = a.as_i32x16();
    let b = _mm512_castsi128_si512(b).as_i32x16();
    let ret: i32x16 = match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    };
    transmute(ret)
}

/// Copy a to tmp, then insert 128 bits (composed of 4 packed 32-bit integers) from b into tmp at the location specified by imm8. Store tmp to dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_inserti32x4&expand=3175)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinserti32x4, imm8 = 2))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_inserti32x4(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m128i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 3);
    let a = a.as_i32x16();
    let b = _mm512_castsi128_si512(b).as_i32x16();
    let insert: i32x16 = match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    };
    transmute(simd_select_bitmask(k, insert, src.as_i32x16()))
}

/// Copy a to tmp, then insert 128 bits (composed of 4 packed 32-bit integers) from b into tmp at the location specified by imm8. Store tmp to dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_inserti32x4&expand=3176)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinserti32x4, imm8 = 2))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_inserti32x4(k: __mmask16, a: __m512i, b: __m128i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 3);
    let a = a.as_i32x16();
    let b = _mm512_castsi128_si512(b).as_i32x16();
    let insert = match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    };
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, insert, zero))
}

/// Copy a to dst, then insert 256 bits (composed of 4 packed 64-bit integers) from b into dst at the location specified by imm8.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_inserti64x4&expand=3186)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf64x4, imm8 = 1))] //should be vinserti64x4
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_inserti64x4(a: __m512i, b: __m256i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castsi256_si512(b);
    match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    }
}

/// Copy a to tmp, then insert 256 bits (composed of 4 packed 64-bit integers) from b into tmp at the location specified by imm8. Store tmp to dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_inserti64x4&expand=3187)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinserti64x4, imm8 = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_inserti64x4(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m256i,
    imm8: i32,
) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castsi256_si512(b);
    let insert = match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    };
    transmute(simd_select_bitmask(k, insert, src.as_i64x8()))
}

/// Copy a to tmp, then insert 256 bits (composed of 4 packed 64-bit integers) from b into tmp at the location specified by imm8. Store tmp to dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_inserti64x4&expand=3188)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinserti64x4, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_inserti64x4(k: __mmask8, a: __m512i, b: __m256i, imm8: i32) -> __m512i {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castsi256_si512(b);
    let insert = match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    };
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, insert, zero))
}

/// Copy a to dst, then insert 128 bits (composed of 4 packed single-precision (32-bit) floating-point elements) from b into dst at the location specified by imm8.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_insertf32x4&expand=3155)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf32x4, imm8 = 2))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_insertf32x4(a: __m512, b: __m128, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 3);
    let b = _mm512_castps128_ps512(b);
    match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    }
}

/// Copy a to tmp, then insert 128 bits (composed of 4 packed single-precision (32-bit) floating-point elements) from b into tmp at the location specified by imm8. Store tmp to dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_insertf32x4&expand=3156)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf32x4, imm8 = 2))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_insertf32x4(
    src: __m512,
    k: __mmask16,
    a: __m512,
    b: __m128,
    imm8: i32,
) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 3);
    let b = _mm512_castps128_ps512(b);
    let insert = match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    };
    transmute(simd_select_bitmask(k, insert, src.as_f32x16()))
}

/// Copy a to tmp, then insert 128 bits (composed of 4 packed single-precision (32-bit) floating-point elements) from b into tmp at the location specified by imm8. Store tmp to dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_insertf32x4&expand=3157)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf32x4, imm8 = 2))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_insertf32x4(k: __mmask16, a: __m512, b: __m128, imm8: i32) -> __m512 {
    assert!(imm8 >= 0 && imm8 <= 3);
    let b = _mm512_castps128_ps512(b);
    let insert = match imm8 & 0b11 {
        0 => simd_shuffle16(
            a,
            b,
            [16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        1 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 16, 17, 18, 19, 8, 9, 10, 11, 12, 13, 14, 15],
        ),
        2 => simd_shuffle16(
            a,
            b,
            [0, 1, 2, 3, 4, 5, 6, 7, 16, 17, 18, 19, 12, 13, 14, 15],
        ),
        _ => simd_shuffle16(a, b, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 17, 18, 19]),
    };
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, insert, zero))
}

/// Copy a to dst, then insert 256 bits (composed of 4 packed double-precision (64-bit) floating-point elements) from b into dst at the location specified by imm8.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_insertf64x4&expand=3167)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf64x4, imm8 = 1))]
#[rustc_args_required_const(2)]
pub unsafe fn _mm512_insertf64x4(a: __m512d, b: __m256d, imm8: i32) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castpd256_pd512(b);
    match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    }
}

/// Copy a to tmp, then insert 256 bits (composed of 4 packed double-precision (64-bit) floating-point elements) from b into tmp at the location specified by imm8. Store tmp to dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_insertf64x4&expand=3168)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf64x4, imm8 = 1))]
#[rustc_args_required_const(4)]
pub unsafe fn _mm512_mask_insertf64x4(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m256d,
    imm8: i32,
) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castpd256_pd512(b);
    let insert = match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    };
    transmute(simd_select_bitmask(k, insert, src.as_f64x8()))
}

/// Copy a to tmp, then insert 256 bits (composed of 4 packed double-precision (64-bit) floating-point elements) from b into tmp at the location specified by imm8. Store tmp to dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_insertf64x4&expand=3169)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vinsertf64x4, imm8 = 1))]
#[rustc_args_required_const(3)]
pub unsafe fn _mm512_maskz_insertf64x4(k: __mmask8, a: __m512d, b: __m256d, imm8: i32) -> __m512d {
    assert!(imm8 >= 0 && imm8 <= 1);
    let b = _mm512_castpd256_pd512(b);
    let insert = match imm8 & 0b1 {
        0 => simd_shuffle8(a, b, [8, 9, 10, 11, 4, 5, 6, 7]),
        _ => simd_shuffle8(a, b, [0, 1, 2, 3, 8, 9, 10, 11]),
    };
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, insert, zero))
}

/// Unpack and interleave 32-bit integers from the high half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpackhi_epi32&expand=6021)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhps))] //should be vpunpckhdq
pub unsafe fn _mm512_unpackhi_epi32(a: __m512i, b: __m512i) -> __m512i {
    let a = a.as_i32x16();
    let b = b.as_i32x16();
    let r: i32x16 = simd_shuffle16(
        a,
        b,
        [
            2,
            18,
            3,
            19,
            2 + 4,
            18 + 4,
            3 + 4,
            19 + 4,
            2 + 8,
            18 + 8,
            3 + 8,
            19 + 8,
            2 + 12,
            18 + 12,
            3 + 12,
            19 + 12,
        ],
    );
    transmute(r)
}

/// Unpack and interleave 32-bit integers from the high half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpackhi_epi32&expand=6019)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckhdq))]
pub unsafe fn _mm512_mask_unpackhi_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let unpackhi = _mm512_unpackhi_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, unpackhi, src.as_i32x16()))
}

/// Unpack and interleave 32-bit integers from the high half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpackhi_epi32&expand=6020)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckhdq))]
pub unsafe fn _mm512_maskz_unpackhi_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let unpackhi = _mm512_unpackhi_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave 64-bit integers from the high half of each 128-bit lane in a and b, and
/// store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpackhi_epi64&expand=6030)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhpd))] //should be vpunpckhqdq
pub unsafe fn _mm512_unpackhi_epi64(a: __m512i, b: __m512i) -> __m512i {
    simd_shuffle8(a, b, [1, 9, 1 + 2, 9 + 2, 1 + 4, 9 + 4, 1 + 6, 9 + 6])
}

/// Unpack and interleave 64-bit integers from the high half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpackhi_epi64&expand=6028)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckhqdq))]
pub unsafe fn _mm512_mask_unpackhi_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let unpackhi = _mm512_unpackhi_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, unpackhi, src.as_i64x8()))
}

/// Unpack and interleave 64-bit integers from the high half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpackhi_epi64&expand=6029)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckhqdq))]
pub unsafe fn _mm512_maskz_unpackhi_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let unpackhi = _mm512_unpackhi_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpackhi_ps&expand=6060)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhps))]
pub unsafe fn _mm512_unpackhi_ps(a: __m512, b: __m512) -> __m512 {
    simd_shuffle16(
        a,
        b,
        [
            2,
            18,
            3,
            19,
            2 + 4,
            18 + 4,
            3 + 4,
            19 + 4,
            2 + 8,
            18 + 8,
            3 + 8,
            19 + 8,
            2 + 12,
            18 + 12,
            3 + 12,
            19 + 12,
        ],
    )
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpackhi_ps&expand=6058)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhps))]
pub unsafe fn _mm512_mask_unpackhi_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let unpackhi = _mm512_unpackhi_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, unpackhi, src.as_f32x16()))
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpackhi_ps&expand=6059)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhps))]
pub unsafe fn _mm512_maskz_unpackhi_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let unpackhi = _mm512_unpackhi_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpackhi_pd&expand=6048)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhpd))]
pub unsafe fn _mm512_unpackhi_pd(a: __m512d, b: __m512d) -> __m512d {
    simd_shuffle8(a, b, [1, 9, 1 + 2, 9 + 2, 1 + 4, 9 + 4, 1 + 6, 9 + 6])
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpackhi_pd&expand=6046)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhpd))]
pub unsafe fn _mm512_mask_unpackhi_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
) -> __m512d {
    let unpackhi = _mm512_unpackhi_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, unpackhi, src.as_f64x8()))
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the high half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpackhi_pd&expand=6047)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpckhpd))]
pub unsafe fn _mm512_maskz_unpackhi_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let unpackhi = _mm512_unpackhi_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave 32-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpacklo_epi32&expand=6078)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklps))] //should be vpunpckldq
pub unsafe fn _mm512_unpacklo_epi32(a: __m512i, b: __m512i) -> __m512i {
    let a = a.as_i32x16();
    let b = b.as_i32x16();
    let r: i32x16 = simd_shuffle16(
        a,
        b,
        [
            0,
            16,
            1,
            17,
            0 + 4,
            16 + 4,
            1 + 4,
            17 + 4,
            0 + 8,
            16 + 8,
            1 + 8,
            17 + 8,
            0 + 12,
            16 + 12,
            1 + 12,
            17 + 12,
        ],
    );
    transmute(r)
}

/// Unpack and interleave 32-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpacklo_epi32&expand=6076)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckldq))]
pub unsafe fn _mm512_mask_unpacklo_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let unpackhi = _mm512_unpacklo_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, unpackhi, src.as_i32x16()))
}

/// Unpack and interleave 32-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpacklo_epi32&expand=6077)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpckldq))]
pub unsafe fn _mm512_maskz_unpacklo_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let unpackhi = _mm512_unpacklo_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave 64-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpacklo_epi64&expand=6087)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklpd))] //should be vpunpcklqdq
pub unsafe fn _mm512_unpacklo_epi64(a: __m512i, b: __m512i) -> __m512i {
    simd_shuffle8(a, b, [0, 8, 0 + 2, 8 + 2, 0 + 4, 8 + 4, 0 + 6, 8 + 6])
}

/// Unpack and interleave 64-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpacklo_epi64&expand=6085)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpcklqdq))]
pub unsafe fn _mm512_mask_unpacklo_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let unpackhi = _mm512_unpacklo_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, unpackhi, src.as_i64x8()))
}

/// Unpack and interleave 64-bit integers from the low half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpacklo_epi64&expand=6086)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpunpcklqdq))]
pub unsafe fn _mm512_maskz_unpacklo_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let unpackhi = _mm512_unpacklo_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpacklo_ps&expand=6117)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklps))]
pub unsafe fn _mm512_unpacklo_ps(a: __m512, b: __m512) -> __m512 {
    simd_shuffle16(
        a,
        b,
        [
            0,
            16,
            1,
            17,
            0 + 4,
            16 + 4,
            1 + 4,
            17 + 4,
            0 + 8,
            16 + 8,
            1 + 8,
            17 + 8,
            0 + 12,
            16 + 12,
            1 + 12,
            17 + 12,
        ],
    )
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpacklo_ps&expand=6115)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklps))]
pub unsafe fn _mm512_mask_unpacklo_ps(src: __m512, k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let unpackhi = _mm512_unpacklo_ps(a, b).as_f32x16();
    transmute(simd_select_bitmask(k, unpackhi, src.as_f32x16()))
}

/// Unpack and interleave single-precision (32-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpacklo_ps&expand=6116)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklps))]
pub unsafe fn _mm512_maskz_unpacklo_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    let unpackhi = _mm512_unpacklo_ps(a, b).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_unpacklo_pd&expand=6105)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklpd))]
pub unsafe fn _mm512_unpacklo_pd(a: __m512d, b: __m512d) -> __m512d {
    simd_shuffle8(a, b, [0, 8, 0 + 2, 8 + 2, 0 + 4, 8 + 4, 0 + 6, 8 + 6])
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_unpacklo_pd&expand=6103)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklpd))]
pub unsafe fn _mm512_mask_unpacklo_pd(
    src: __m512d,
    k: __mmask8,
    a: __m512d,
    b: __m512d,
) -> __m512d {
    let unpackhi = _mm512_unpacklo_pd(a, b).as_f64x8();
    transmute(simd_select_bitmask(k, unpackhi, src.as_f64x8()))
}

/// Unpack and interleave double-precision (64-bit) floating-point elements from the low half of each 128-bit lane in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_unpacklo_pd&expand=6104)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vunpcklpd))]
pub unsafe fn _mm512_maskz_unpacklo_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    let unpackhi = _mm512_unpacklo_pd(a, b).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, unpackhi, zero))
}

/// Cast vector of type __m128 to type __m512; the upper 384 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps128_ps512&expand=621)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps128_ps512(a: __m128) -> __m512 {
    simd_shuffle16(
        a,
        _mm_set1_ps(-1.),
        [0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4],
    )
}

/// Cast vector of type __m256 to type __m512; the upper 256 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps256_ps512&expand=623)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps256_ps512(a: __m256) -> __m512 {
    simd_shuffle16(
        a,
        _mm256_set1_ps(-1.),
        [0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8],
    )
}

/// Cast vector of type __m512 to type __m128. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps512_ps128&expand=624)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps512_ps128(a: __m512) -> __m128 {
    simd_shuffle4(a, a, [0, 1, 2, 3])
}

/// Cast vector of type __m512 to type __m256. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps512_ps256&expand=625)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps512_ps256(a: __m512) -> __m256 {
    simd_shuffle8(a, a, [0, 1, 2, 3, 4, 5, 6, 7])
}

/// Cast vector of type __m512 to type __m512d. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps_pd&expand=616)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps_pd(a: __m512) -> __m512d {
    transmute(a.as_m512())
}

/// Cast vector of type __m512 to type __m512i. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castps_si512&expand=619)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castps_si512(a: __m512) -> __m512i {
    transmute(a.as_m512())
}

/// Cast vector of type __m128d to type __m512d; the upper 384 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd128_pd512&expand=609)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd128_pd512(a: __m128d) -> __m512d {
    simd_shuffle8(a, _mm_set1_pd(-1.), [0, 1, 2, 2, 2, 2, 2, 2])
}

/// Cast vector of type __m256d to type __m512d; the upper 256 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd256_pd512&expand=611)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd256_pd512(a: __m256d) -> __m512d {
    simd_shuffle8(a, _mm256_set1_pd(-1.), [0, 1, 2, 3, 4, 4, 4, 4])
}

/// Cast vector of type __m512d to type __m128d. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd512_pd128&expand=612)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd512_pd128(a: __m512d) -> __m128d {
    simd_shuffle2(a, a, [0, 1])
}

/// Cast vector of type __m512d to type __m256d. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd512_pd256&expand=613)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd512_pd256(a: __m512d) -> __m256d {
    simd_shuffle4(a, a, [0, 1, 2, 3])
}

/// Cast vector of type __m512d to type __m512. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd_ps&expand=604)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd_ps(a: __m512d) -> __m512 {
    transmute(a.as_m512d())
}

/// Cast vector of type __m512d to type __m512i. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castpd_si512&expand=607)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castpd_si512(a: __m512d) -> __m512i {
    transmute(a.as_m512d())
}

/// Cast vector of type __m128i to type __m512i; the upper 384 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi128_si512&expand=629)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi128_si512(a: __m128i) -> __m512i {
    simd_shuffle8(a, _mm_set1_epi64x(-1), [0, 1, 2, 2, 2, 2, 2, 2])
}

/// Cast vector of type __m256i to type __m512i; the upper 256 bits of the result are undefined. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi256_si512&expand=633)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi256_si512(a: __m256i) -> __m512i {
    simd_shuffle8(a, _mm256_set1_epi64x(-1), [0, 1, 2, 3, 4, 4, 4, 4])
}

/// Cast vector of type __m512i to type __m128i. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi512_si128&expand=636)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi512_si128(a: __m512i) -> __m128i {
    simd_shuffle2(a, a, [0, 1])
}

/// Cast vector of type __m512i to type __m256i. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi512_si256&expand=637)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi512_si256(a: __m512i) -> __m256i {
    simd_shuffle4(a, a, [0, 1, 2, 3])
}

/// Cast vector of type __m512i to type __m512. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi512_ps&expand=635)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi512_ps(a: __m512i) -> __m512 {
    transmute(a)
}

/// Cast vector of type __m512i to type __m512d. This intrinsic is only used for compilation and does not generate any instructions, thus it has zero latency.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_castsi512_pd&expand=634)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_castsi512_pd(a: __m512i) -> __m512d {
    transmute(a)
}

/// Broadcast the low packed 32-bit integer from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcastd_epi32&expand=545)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcast))] //should be vpbroadcastd
pub unsafe fn _mm512_broadcastd_epi32(a: __m128i) -> __m512i {
    let a = _mm512_castsi128_si512(a).as_i32x16();
    let ret: i32x16 = simd_shuffle16(a, a, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
    transmute(ret)
}

/// Broadcast the low packed 32-bit integer from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcastd_epi32&expand=546)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast))] //should be vpbroadcastd
pub unsafe fn _mm512_mask_broadcastd_epi32(src: __m512i, k: __mmask16, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcastd_epi32(a).as_i32x16();
    transmute(simd_select_bitmask(k, broadcast, src.as_i32x16()))
}

/// Broadcast the low packed 32-bit integer from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcastd_epi32&expand=547)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast))] //should be vpbroadcastd
pub unsafe fn _mm512_maskz_broadcastd_epi32(k: __mmask16, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcastd_epi32(a).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the low packed 64-bit integer from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcastq_epi64&expand=560)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcas))] //should be vpbroadcastq
pub unsafe fn _mm512_broadcastq_epi64(a: __m128i) -> __m512i {
    simd_shuffle8(a, a, [0, 0, 0, 0, 0, 0, 0, 0])
}

/// Broadcast the low packed 64-bit integer from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcastq_epi64&expand=561)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast))] //should be vpbroadcastq
pub unsafe fn _mm512_mask_broadcastq_epi64(src: __m512i, k: __mmask8, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcastq_epi64(a).as_i64x8();
    transmute(simd_select_bitmask(k, broadcast, src.as_i64x8()))
}

/// Broadcast the low packed 64-bit integer from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcastq_epi64&expand=562)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpbroadcast))] //should be vpbroadcastq
pub unsafe fn _mm512_maskz_broadcastq_epi64(k: __mmask8, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcastq_epi64(a).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the low single-precision (32-bit) floating-point element from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcastss_ps&expand=578)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastss))]
pub unsafe fn _mm512_broadcastss_ps(a: __m128) -> __m512 {
    simd_shuffle16(a, a, [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
}

/// Broadcast the low single-precision (32-bit) floating-point element from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcastss_ps&expand=579)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastss))]
pub unsafe fn _mm512_mask_broadcastss_ps(src: __m512, k: __mmask16, a: __m128) -> __m512 {
    let broadcast = _mm512_broadcastss_ps(a).as_f32x16();
    transmute(simd_select_bitmask(k, broadcast, src.as_f32x16()))
}

/// Broadcast the low single-precision (32-bit) floating-point element from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcastss_ps&expand=580)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastss))]
pub unsafe fn _mm512_maskz_broadcastss_ps(k: __mmask16, a: __m128) -> __m512 {
    let broadcast = _mm512_broadcastss_ps(a).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the low double-precision (64-bit) floating-point element from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcastsd_pd&expand=567)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastsd))]
pub unsafe fn _mm512_broadcastsd_pd(a: __m128d) -> __m512d {
    simd_shuffle8(a, a, [1, 1, 1, 1, 1, 1, 1, 1])
}

/// Broadcast the low double-precision (64-bit) floating-point element from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcastsd_pd&expand=568)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastsd))]
pub unsafe fn _mm512_mask_broadcastsd_pd(src: __m512d, k: __mmask8, a: __m128d) -> __m512d {
    let broadcast = _mm512_broadcastsd_pd(a).as_f64x8();
    transmute(simd_select_bitmask(k, broadcast, src.as_f64x8()))
}

/// Broadcast the low double-precision (64-bit) floating-point element from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcastsd_pd&expand=569)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vbroadcastsd))]
pub unsafe fn _mm512_maskz_broadcastsd_pd(k: __mmask8, a: __m128d) -> __m512d {
    let broadcast = _mm512_broadcastsd_pd(a).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the 4 packed 32-bit integers from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcast_i32x4&expand=510)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti32x4, linux: vshuf
pub unsafe fn _mm512_broadcast_i32x4(a: __m128i) -> __m512i {
    let a = _mm512_castsi128_si512(a).as_i32x16();
    let ret: i32x16 = simd_shuffle16(a, a, [0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3]);
    transmute(ret)
}

/// Broadcast the 4 packed 32-bit integers from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcast_i32x4&expand=511)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti32x4, linux: vshuf
pub unsafe fn _mm512_mask_broadcast_i32x4(src: __m512i, k: __mmask16, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcast_i32x4(a).as_i32x16();
    transmute(simd_select_bitmask(k, broadcast, src.as_i32x16()))
}

/// Broadcast the 4 packed 32-bit integers from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcast_i32x4&expand=512)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti32x4, linux: vshuf
pub unsafe fn _mm512_maskz_broadcast_i32x4(k: __mmask16, a: __m128i) -> __m512i {
    let broadcast = _mm512_broadcast_i32x4(a).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the 4 packed 64-bit integers from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcast_i64x4&expand=522)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti64x4, linux: vperm
pub unsafe fn _mm512_broadcast_i64x4(a: __m256i) -> __m512i {
    simd_shuffle8(a, a, [0, 1, 2, 3, 0, 1, 2, 3])
}

/// Broadcast the 4 packed 64-bit integers from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcast_i64x4&expand=523)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti64x4, linux: vperm
pub unsafe fn _mm512_mask_broadcast_i64x4(src: __m512i, k: __mmask8, a: __m256i) -> __m512i {
    let broadcast = _mm512_broadcast_i64x4(a).as_i64x8();
    transmute(simd_select_bitmask(k, broadcast, src.as_i64x8()))
}

/// Broadcast the 4 packed 64-bit integers from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcast_i64x4&expand=524)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcasti64x4, linux: vperm
pub unsafe fn _mm512_maskz_broadcast_i64x4(k: __mmask8, a: __m256i) -> __m512i {
    let broadcast = _mm512_broadcast_i64x4(a).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the 4 packed single-precision (32-bit) floating-point elements from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcast_f32x4&expand=483)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf32x4, linux: vshuf
pub unsafe fn _mm512_broadcast_f32x4(a: __m128) -> __m512 {
    simd_shuffle16(a, a, [0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3])
}

/// Broadcast the 4 packed single-precision (32-bit) floating-point elements from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcast_f32x4&expand=484)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf32x4, linux: vshu
pub unsafe fn _mm512_mask_broadcast_f32x4(src: __m512, k: __mmask16, a: __m128) -> __m512 {
    let broadcast = _mm512_broadcast_f32x4(a).as_f32x16();
    transmute(simd_select_bitmask(k, broadcast, src.as_f32x16()))
}

/// Broadcast the 4 packed single-precision (32-bit) floating-point elements from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcast_f32x4&expand=485)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf32x4, linux: vshu
pub unsafe fn _mm512_maskz_broadcast_f32x4(k: __mmask16, a: __m128) -> __m512 {
    let broadcast = _mm512_broadcast_f32x4(a).as_f32x16();
    let zero = _mm512_setzero_ps().as_f32x16();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Broadcast the 4 packed double-precision (64-bit) floating-point elements from a to all elements of dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_broadcast_f64x4&expand=495)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf64x4, linux: vperm
pub unsafe fn _mm512_broadcast_f64x4(a: __m256d) -> __m512d {
    simd_shuffle8(a, a, [0, 1, 2, 3, 0, 1, 2, 3])
}

/// Broadcast the 4 packed double-precision (64-bit) floating-point elements from a to all elements of dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_broadcast_f64x4&expand=496)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf64x4, linux: vper
pub unsafe fn _mm512_mask_broadcast_f64x4(src: __m512d, k: __mmask8, a: __m256d) -> __m512d {
    let broadcast = _mm512_broadcast_f64x4(a).as_f64x8();
    transmute(simd_select_bitmask(k, broadcast, src.as_f64x8()))
}

/// Broadcast the 4 packed double-precision (64-bit) floating-point elements from a to all elements of dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_broadcast_f64x4&expand=497)
#[inline]
#[target_feature(enable = "avx512f")] //msvc: vbroadcastf64x4, linux: vper
pub unsafe fn _mm512_maskz_broadcast_f64x4(k: __mmask8, a: __m256d) -> __m512d {
    let broadcast = _mm512_broadcast_f64x4(a).as_f64x8();
    let zero = _mm512_setzero_pd().as_f64x8();
    transmute(simd_select_bitmask(k, broadcast, zero))
}

/// Blend packed 32-bit integers from a and b using control mask k, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_blend_epi32&expand=435)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovdqa32))] //should be vpblendmd
pub unsafe fn _mm512_mask_blend_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_select_bitmask(k, b.as_i32x16(), a.as_i32x16()))
}

/// Blend packed 64-bit integers from a and b using control mask k, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_blend_epi64&expand=438)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovdqa64))] //should be vpblendmq
pub unsafe fn _mm512_mask_blend_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_select_bitmask(k, b.as_i64x8(), a.as_i64x8()))
}

/// Blend packed single-precision (32-bit) floating-point elements from a and b using control mask k, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_blend_ps&expand=451)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovaps))] //should be vpblendmps
pub unsafe fn _mm512_mask_blend_ps(k: __mmask16, a: __m512, b: __m512) -> __m512 {
    transmute(simd_select_bitmask(k, b.as_f32x16(), a.as_f32x16()))
}

/// Blend packed double-precision (64-bit) floating-point elements from a and b using control mask k, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_blend_pd&expand=446)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovapd))] //should be vpblendmpd
pub unsafe fn _mm512_mask_blend_pd(k: __mmask8, a: __m512d, b: __m512d) -> __m512d {
    transmute(simd_select_bitmask(k, b.as_f64x8(), a.as_f64x8()))
}

/// Compute the bitwise AND of packed 32-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_and_epi32&expand=272)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))] //should be vpandd, but generate vpandq
pub unsafe fn _mm512_and_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_and(a.as_i32x16(), b.as_i32x16()))
}

/// Performs element-by-element bitwise AND between packed 32-bit integer elements of v2 and v3, storing the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_and_epi32&expand=273)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandd))]
pub unsafe fn _mm512_mask_and_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let and = _mm512_and_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, and, src.as_i32x16()))
}

/// Compute the bitwise AND of packed 32-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_and_epi32&expand=274)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandd))]
pub unsafe fn _mm512_maskz_and_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let and = _mm512_and_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, and, zero))
}

/// Compute the bitwise AND of 512 bits (composed of packed 64-bit integers) in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_and_epi64&expand=279)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_and_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_and(a.as_i64x8(), b.as_i64x8()))
}

/// Compute the bitwise AND of packed 64-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_and_epi64&expand=280)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_mask_and_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let and = _mm512_and_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, and, src.as_i64x8()))
}

/// Compute the bitwise AND of packed 32-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_and_Epi32&expand=274)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_maskz_and_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let and = _mm512_and_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, and, zero))
}

/// Compute the bitwise AND of 512 bits (representing integer data) in a and b, and store the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_and_si512&expand=302)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandq))]
pub unsafe fn _mm512_and_si512(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_and(a.as_i32x16(), b.as_i32x16()))
}

/// Compute the bitwise OR of packed 32-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_or_epi32&expand=4042)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vporq))]
pub unsafe fn _mm512_or_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_or(a.as_i32x16(), b.as_i32x16()))
}

/// Compute the bitwise OR of packed 32-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_or_epi32&expand=4040)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpord))]
pub unsafe fn _mm512_mask_or_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let or = _mm512_or_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, or, src.as_i32x16()))
}

/// Compute the bitwise OR of packed 32-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_or_epi32&expand=4041)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpord))]
pub unsafe fn _mm512_maskz_or_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let or = _mm512_or_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, or, zero))
}

/// Compute the bitwise OR of packed 64-bit integers in a and b, and store the resut in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_or_epi64&expand=4051)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vporq))]
pub unsafe fn _mm512_or_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_or(a.as_i64x8(), b.as_i64x8()))
}

/// Compute the bitwise OR of packed 64-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_or_epi64&expand=4049)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vporq))]
pub unsafe fn _mm512_mask_or_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let or = _mm512_or_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, or, src.as_i64x8()))
}

/// Compute the bitwise OR of packed 64-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_or_epi64&expand=4050)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vporq))]
pub unsafe fn _mm512_maskz_or_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let or = _mm512_or_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, or, zero))
}

/// Compute the bitwise OR of 512 bits (representing integer data) in a and b, and store the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_or_si512&expand=4072)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vporq))]
pub unsafe fn _mm512_or_si512(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_or(a.as_i32x16(), b.as_i32x16()))
}

/// Compute the bitwise XOR of packed 32-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_xor_epi32&expand=6142)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxorq))]
pub unsafe fn _mm512_xor_epi32(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_xor(a.as_i32x16(), b.as_i32x16()))
}

/// Compute the bitwise XOR of packed 32-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_xor_epi32&expand=6140)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxord))]
pub unsafe fn _mm512_mask_xor_epi32(src: __m512i, k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let xor = _mm512_xor_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, xor, src.as_i32x16()))
}

/// Compute the bitwise XOR of packed 32-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_xor_epi32&expand=6141)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxord))]
pub unsafe fn _mm512_maskz_xor_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let xor = _mm512_xor_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, xor, zero))
}

/// Compute the bitwise XOR of packed 64-bit integers in a and b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_xor_epi64&expand=6151)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxorq))]
pub unsafe fn _mm512_xor_epi64(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_xor(a.as_i64x8(), b.as_i64x8()))
}

/// Compute the bitwise XOR of packed 64-bit integers in a and b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_xor_epi64&expand=6149)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxorq))]
pub unsafe fn _mm512_mask_xor_epi64(src: __m512i, k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let xor = _mm512_xor_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, xor, src.as_i64x8()))
}

/// Compute the bitwise XOR of packed 64-bit integers in a and b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_xor_epi64&expand=6150)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxorq))]
pub unsafe fn _mm512_maskz_xor_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let xor = _mm512_xor_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, xor, zero))
}

/// Compute the bitwise XOR of 512 bits (representing integer data) in a and b, and store the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_xor_si512&expand=6172)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpxorq))]
pub unsafe fn _mm512_xor_si512(a: __m512i, b: __m512i) -> __m512i {
    transmute(simd_xor(a.as_i32x16(), b.as_i32x16()))
}

/// Compute the bitwise NOT of packed 32-bit integers in a and then AND with b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_andnot_epi32&expand=310)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnq))] //should be vpandnd
pub unsafe fn _mm512_andnot_epi32(a: __m512i, b: __m512i) -> __m512i {
    _mm512_and_epi32(_mm512_xor_epi32(a, _mm512_set1_epi32(u32::MAX as i32)), b)
}

/// Compute the bitwise NOT of packed 32-bit integers in a and then AND with b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_andnot_epi32&expand=311)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnd))]
pub unsafe fn _mm512_mask_andnot_epi32(
    src: __m512i,
    k: __mmask16,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let andnot = _mm512_andnot_epi32(a, b).as_i32x16();
    transmute(simd_select_bitmask(k, andnot, src.as_i32x16()))
}

/// Compute the bitwise NOT of packed 32-bit integers in a and then AND with b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_andnot_epi32&expand=312)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnd))]
pub unsafe fn _mm512_maskz_andnot_epi32(k: __mmask16, a: __m512i, b: __m512i) -> __m512i {
    let andnot = _mm512_andnot_epi32(a, b).as_i32x16();
    let zero = _mm512_setzero_si512().as_i32x16();
    transmute(simd_select_bitmask(k, andnot, zero))
}

/// Compute the bitwise NOT of 512 bits (composed of packed 64-bit integers) in a and then AND with b, and store the results in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_andnot_epi64&expand=317)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnq))] //should be vpandnd
pub unsafe fn _mm512_andnot_epi64(a: __m512i, b: __m512i) -> __m512i {
    _mm512_and_epi64(_mm512_xor_epi64(a, _mm512_set1_epi64(u64::MAX as i64)), b)
}

/// Compute the bitwise NOT of packed 64-bit integers in a and then AND with b, and store the results in dst using writemask k (elements are copied from src when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_mask_andnot_epi64&expand=318)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnq))]
pub unsafe fn _mm512_mask_andnot_epi64(
    src: __m512i,
    k: __mmask8,
    a: __m512i,
    b: __m512i,
) -> __m512i {
    let andnot = _mm512_andnot_epi64(a, b).as_i64x8();
    transmute(simd_select_bitmask(k, andnot, src.as_i64x8()))
}

/// Compute the bitwise NOT of packed 64-bit integers in a and then AND with b, and store the results in dst using zeromask k (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_maskz_andnot_epi64&expand=319)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnq))]
pub unsafe fn _mm512_maskz_andnot_epi64(k: __mmask8, a: __m512i, b: __m512i) -> __m512i {
    let andnot = _mm512_andnot_epi64(a, b).as_i64x8();
    let zero = _mm512_setzero_si512().as_i64x8();
    transmute(simd_select_bitmask(k, andnot, zero))
}

/// Compute the bitwise NOT of 512 bits (representing integer data) in a and then AND with b, and store the result in dst.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_andnot_si512&expand=340)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpandnq))]
pub unsafe fn _mm512_andnot_si512(a: __m512i, b: __m512i) -> __m512i {
    _mm512_and_epi64(_mm512_xor_epi64(a, _mm512_set1_epi64(u64::MAX as i64)), b)
}

/// Compute the bitwise AND of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=kand_mask16&expand=3212)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(and))] // generate normal and code instead of kandw
pub unsafe fn _kand_mask16(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a & b)
}

/// Compute the bitwise AND of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_kand&expand=3210)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(and))] // generate normal and code instead of kandw
pub unsafe fn _mm512_kand(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a & b)
}

/// Compute the bitwise OR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=kor_mask16&expand=3239)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(or))] // generate normal or code instead of korw
pub unsafe fn _kor_mask16(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a | b)
}

/// Compute the bitwise OR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_kor&expand=3237)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(or))] // generate normal or code instead of korw
pub unsafe fn _mm512_kor(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a | b)
}

/// Compute the bitwise XOR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=kxor_mask16&expand=3291)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(xor))] // generate normal xor code instead of kxorw
pub unsafe fn _kxor_mask16(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a ^ b)
}

/// Compute the bitwise XOR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_kxor&expand=3289)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(xor))] // generate normal xor code instead of kxorw
pub unsafe fn _mm512_kxor(a: __mmask16, b: __mmask16) -> __mmask16 {
    transmute(a ^ b)
}

/// Compute the bitwise NOT of 16-bit mask a, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=knot_mask16&expand=3233)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _knot_mask16(a: __mmask16) -> __mmask16 {
    transmute(a ^ 0b11111111_11111111)
}

/// Compute the bitwise NOT of 16-bit mask a, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_knot&expand=3231)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_knot(a: __mmask16) -> __mmask16 {
    transmute(a ^ 0b11111111_11111111)
}

/// Compute the bitwise NOT of 16-bit masks a and then AND with b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=kandn_mask16&expand=3218)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(not))] // generate normal and, not code instead of kandnw
pub unsafe fn _kandn_mask16(a: __mmask16, b: __mmask16) -> __mmask16 {
    _mm512_kand(_mm512_knot(a), b)
}

/// Compute the bitwise NOT of 16-bit masks a and then AND with b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_kandn&expand=3216)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(not))] // generate normal and code instead of kandw
pub unsafe fn _mm512_kandn(a: __mmask16, b: __mmask16) -> __mmask16 {
    _mm512_kand(_mm512_knot(a), b)
}

/// Compute the bitwise XNOR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=kxnor_mask16&expand=3285)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(xor))] // generate normal xor, not code instead of kxnorw
pub unsafe fn _kxnor_mask16(a: __mmask16, b: __mmask16) -> __mmask16 {
    _mm512_knot(_mm512_kxor(a, b))
}

/// Compute the bitwise XNOR of 16-bit masks a and b, and store the result in k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=512_kxnor&expand=3283)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(xor))] // generate normal and code instead of kandw
pub unsafe fn _mm512_kxnor(a: __mmask16, b: __mmask16) -> __mmask16 {
    _mm512_knot(_mm512_kxor(a, b))
}

/// Copy 16-bit mask a to k.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=mm512_kmov&expand=3228)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(mov))] // generate normal and code instead of kmovw
pub unsafe fn _mm512_kmov(a: __mmask16) -> __mmask16 {
    let r: u16 = a;
    transmute(r)
}

/// Sets packed 32-bit integers in `dst` with the supplied values.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_ps)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set_ps(
    e0: f32,
    e1: f32,
    e2: f32,
    e3: f32,
    e4: f32,
    e5: f32,
    e6: f32,
    e7: f32,
    e8: f32,
    e9: f32,
    e10: f32,
    e11: f32,
    e12: f32,
    e13: f32,
    e14: f32,
    e15: f32,
) -> __m512 {
    _mm512_setr_ps(
        e15, e14, e13, e12, e11, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1, e0,
    )
}

/// Sets packed 32-bit integers in `dst` with the supplied values in
/// reverse order.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_ps)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_setr_ps(
    e0: f32,
    e1: f32,
    e2: f32,
    e3: f32,
    e4: f32,
    e5: f32,
    e6: f32,
    e7: f32,
    e8: f32,
    e9: f32,
    e10: f32,
    e11: f32,
    e12: f32,
    e13: f32,
    e14: f32,
    e15: f32,
) -> __m512 {
    let r = f32x16::new(
        e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
    );
    transmute(r)
}

/// Broadcast 64-bit float `a` to all elements of `dst`.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set1_pd(a: f64) -> __m512d {
    transmute(f64x8::splat(a))
}

/// Broadcast 32-bit float `a` to all elements of `dst`.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set1_ps(a: f32) -> __m512 {
    transmute(f32x16::splat(a))
}

/// Sets packed 32-bit integers in `dst` with the supplied values.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set_epi32(
    e15: i32,
    e14: i32,
    e13: i32,
    e12: i32,
    e11: i32,
    e10: i32,
    e9: i32,
    e8: i32,
    e7: i32,
    e6: i32,
    e5: i32,
    e4: i32,
    e3: i32,
    e2: i32,
    e1: i32,
    e0: i32,
) -> __m512i {
    _mm512_setr_epi32(
        e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
    )
}

/// Broadcast 32-bit integer `a` to all elements of `dst`.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set1_epi32(a: i32) -> __m512i {
    transmute(i32x16::splat(a))
}

/// Broadcast 64-bit integer `a` to all elements of `dst`.
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set1_epi64(a: i64) -> __m512i {
    transmute(i64x8::splat(a))
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmplt_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_LT_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmplt_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_LT_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpnlt_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpnlt_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_NLT_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpnlt_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpnlt_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_NLT_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmple_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_LE_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmple_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_LE_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpnle_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpnle_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_NLE_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpnle_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpnle_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_NLE_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpeq_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_EQ_OQ)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpeq_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_EQ_OQ)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpneq_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_NEQ_UQ)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpneq_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_NEQ_UQ)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmp_ps_mask(a: __m512, b: __m512, op: i32) -> __mmask16 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr) => {
            vcmpps(
                a.as_f32x16(),
                b.as_f32x16(),
                $imm5,
                neg_one,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_ps_mask(m: __mmask16, a: __m512, b: __m512, op: i32) -> __mmask16 {
    macro_rules! call {
        ($imm5:expr) => {
            vcmpps(
                a.as_f32x16(),
                b.as_f32x16(),
                $imm5,
                m as i16,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_round_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2, 3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm512_cmp_round_ps_mask(a: __m512, b: __m512, op: i32, sae: i32) -> __mmask16 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpps(a.as_f32x16(), b.as_f32x16(), $imm5, neg_one, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_round_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3, 4)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm512_mask_cmp_round_ps_mask(
    m: __mmask16,
    a: __m512,
    b: __m512,
    op: i32,
    sae: i32,
) -> __mmask16 {
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpps(a.as_f32x16(), b.as_f32x16(), $imm5, m as i16, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b to see if neither is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpord_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmpord_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_ORD_Q)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b to see if neither is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpord_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmpord_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_ORD_Q)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b to see if either is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpunord_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmpunord_ps_mask(a: __m512, b: __m512) -> __mmask16 {
    _mm512_cmp_ps_mask(a, b, _CMP_UNORD_Q)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b to see if either is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpunord_ps_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmpunord_ps_mask(m: __mmask16, a: __m512, b: __m512) -> __mmask16 {
    _mm512_mask_cmp_ps_mask(m, a, b, _CMP_UNORD_Q)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmplt_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_LT_OS)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmplt_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_LT_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpnlt_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpnlt_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_NLT_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpnlt_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpnlt_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_NLT_US)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmple_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_LE_OS)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmple_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_LE_OS)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpnle_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpnle_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_NLE_US)
}

/// Compare packed single-precision (32-bit) floating-point elements in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpnle_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpnle_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_NLE_US)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpeq_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_EQ_OQ)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpeq_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_EQ_OQ)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_cmpneq_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_NEQ_UQ)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp))]
pub unsafe fn _mm512_mask_cmpneq_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_NEQ_UQ)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmp_pd_mask(a: __m512d, b: __m512d, op: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr) => {
            vcmppd(
                a.as_f64x8(),
                b.as_f64x8(),
                $imm5,
                neg_one,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_pd_mask(m: __mmask8, a: __m512d, b: __m512d, op: i32) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr) => {
            vcmppd(
                a.as_f64x8(),
                b.as_f64x8(),
                $imm5,
                m as i8,
                _MM_FROUND_CUR_DIRECTION,
            )
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_round_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2, 3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm512_cmp_round_pd_mask(a: __m512d, b: __m512d, op: i32, sae: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmppd(a.as_f64x8(), b.as_f64x8(), $imm5, neg_one, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_round_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3, 4)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm512_mask_cmp_round_pd_mask(
    m: __mmask8,
    a: __m512d,
    b: __m512d,
    op: i32,
    sae: i32,
) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmppd(a.as_f64x8(), b.as_f64x8(), $imm5, m as i8, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b to see if neither is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpord_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmpord_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_ORD_Q)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b to see if neither is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpord_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmpord_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_ORD_Q)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b to see if either is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpunord_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_cmpunord_pd_mask(a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_cmp_pd_mask(a, b, _CMP_UNORD_Q)
}

/// Compare packed double-precision (64-bit) floating-point elements in a and b to see if either is NaN, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpunord_pd_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vcmp, op = 0))]
pub unsafe fn _mm512_mask_cmpunord_pd_mask(m: __mmask8, a: __m512d, b: __m512d) -> __mmask8 {
    _mm512_mask_cmp_pd_mask(m, a, b, _CMP_UNORD_Q)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_cmp_ss_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_cmp_ss_mask(a: __m128, b: __m128, op: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr) => {
            vcmpss(a, b, $imm5, neg_one, _MM_FROUND_CUR_DIRECTION)
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector using zeromask m (the element is zeroed out when mask bit 0 is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_mask_cmp_ss_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_mask_cmp_ss_mask(m: __mmask8, a: __m128, b: __m128, op: i32) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr) => {
            vcmpss(a, b, $imm5, m as i8, _MM_FROUND_CUR_DIRECTION)
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_cmp_round_ss_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2, 3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_cmp_round_ss_mask(a: __m128, b: __m128, op: i32, sae: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpss(a, b, $imm5, neg_one, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector using zeromask m (the element is zeroed out when mask bit 0 is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_mask_cmp_round_ss_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3, 4)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_mask_cmp_round_ss_mask(
    m: __mmask8,
    a: __m128,
    b: __m128,
    op: i32,
    sae: i32,
) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpss(a, b, $imm5, m as i8, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_cmp_sd_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_cmp_sd_mask(a: __m128d, b: __m128d, op: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr) => {
            vcmpsd(a, b, $imm5, neg_one, _MM_FROUND_CUR_DIRECTION)
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector using zeromask m (the element is zeroed out when mask bit 0 is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_mask_cmp_sd_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_mask_cmp_sd_mask(m: __mmask8, a: __m128d, b: __m128d, op: i32) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr) => {
            vcmpsd(a, b, $imm5, m as i8, _MM_FROUND_CUR_DIRECTION)
        };
    }
    let r = constify_imm5!(op, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_cmp_round_sd_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2, 3)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_cmp_round_sd_mask(a: __m128d, b: __m128d, op: i32, sae: i32) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpsd(a, b, $imm5, neg_one, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare the lower single-precision (32-bit) floating-point element in a and b based on the comparison operand specified by imm8, and store the result in a mask vector using zeromask m (the element is zeroed out when mask bit 0 is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm_mask_cmp_round_sd_mask&expand=5236,755,757)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3, 4)]
#[cfg_attr(test, assert_instr(vcmp, op = 0, sae = 4))]
pub unsafe fn _mm_mask_cmp_round_sd_mask(
    m: __mmask8,
    a: __m128d,
    b: __m128d,
    op: i32,
    sae: i32,
) -> __mmask8 {
    macro_rules! call {
        ($imm5:expr, $imm4:expr) => {
            vcmpsd(a, b, $imm5, m as i8, $imm4)
        };
    }
    let r = constify_imm5_sae!(op, sae, call);
    transmute(r)
}

/// Compare packed unsigned 32-bit integers in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmplt_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_lt(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmplt_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmplt_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpgt_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpgt_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_gt(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpgt_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpgt_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpgt_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmple_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_le(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmple_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmple_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpge_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpge_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_ge(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpge_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpge_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpge_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpeq_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_eq(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpeq_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpeq_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_epu32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpneq_epu32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<u32x16, _>(simd_ne(a.as_u32x16(), b.as_u32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_epu32_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpneq_epu32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpneq_epu32_mask(a, b) & m
}

/// Compare packed unsigned 32-bit integers in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epu32_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_cmp_epu32_mask(a: __m512i, b: __m512i, op: _MM_CMPINT_ENUM) -> __mmask16 {
    let neg_one = -1;
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpud(a.as_i32x16(), b.as_i32x16(), $imm3, neg_one)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed unsigned 32-bit integers in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epu32_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_epu32_mask(
    m: __mmask16,
    a: __m512i,
    b: __m512i,
    op: _MM_CMPINT_ENUM,
) -> __mmask16 {
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpud(a.as_i32x16(), b.as_i32x16(), $imm3, m as i16)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed unsigned 32-bit integers in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmplt_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_lt(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed unsigned 32-bit integers in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmplt_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmplt_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpgt_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpgt_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_gt(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpgt_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpgt_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpgt_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmple_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_le(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmple_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmple_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpge_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpge_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_ge(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpge_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpge_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpge_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpeq_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_eq(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpeq_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpeq_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpneq_epi32_mask(a: __m512i, b: __m512i) -> __mmask16 {
    simd_bitmask::<i32x16, _>(simd_ne(a.as_i32x16(), b.as_i32x16()))
}

/// Compare packed signed 32-bit integers in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_epi32)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpneq_epi32_mask(m: __mmask16, a: __m512i, b: __m512i) -> __mmask16 {
    _mm512_cmpneq_epi32_mask(a, b) & m
}

/// Compare packed signed 32-bit integers in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epi32_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_cmp_epi32_mask(a: __m512i, b: __m512i, op: _MM_CMPINT_ENUM) -> __mmask16 {
    let neg_one = -1;
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpd(a.as_i32x16(), b.as_i32x16(), $imm3, neg_one)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed signed 32-bit integers in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epi32_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_epi32_mask(
    m: __mmask16,
    a: __m512i,
    b: __m512i,
    op: _MM_CMPINT_ENUM,
) -> __mmask16 {
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpd(a.as_i32x16(), b.as_i32x16(), $imm3, m as i16)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed unsigned 64-bit integers in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmplt_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_lt(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmplt_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmplt_epu64_mask(a, b) & m
}

/// Compare packed unsigned 64-bit integers in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpgt_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpgt_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_gt(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpgt_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpgt_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpgt_epu64_mask(a, b) & m
}

/// Compare packed unsigned 64-bit integers in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmple_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_le(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmple_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmple_epu64_mask(a, b) & m
}

/// Compare packed unsigned 64-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpge_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpge_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_ge(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpge_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpge_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpge_epu64_mask(b, a) & m
}

/// Compare packed unsigned 64-bit integers in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpeq_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_eq(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpeq_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpeq_epu64_mask(a, b) & m
}

/// Compare packed unsigned 64-bit integers in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_epu64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpneq_epu64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_ne(a.as_u64x8(), b.as_u64x8()))
}

/// Compare packed unsigned 64-bit integers in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_epu64_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpneq_epu64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpneq_epu64_mask(a, b) & m
}

/// Compare packed unsigned 64-bit integers in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epu64_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_cmp_epu64_mask(a: __m512i, b: __m512i, op: _MM_CMPINT_ENUM) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpuq(a.as_i64x8(), b.as_i64x8(), $imm3, neg_one)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed unsigned 64-bit integers in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epu64_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_epu64_mask(
    m: __mmask8,
    a: __m512i,
    b: __m512i,
    op: _MM_CMPINT_ENUM,
) -> __mmask8 {
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpuq(a.as_i64x8(), b.as_i64x8(), $imm3, m as i8)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed signed 64-bit integers in a and b for less-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmplt_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmplt_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_lt(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for less-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmplt_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmplt_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmplt_epi64_mask(a, b) & m
}

/// Compare packed signed 64-bit integers in a and b for greater-than, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpgt_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpgt_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_gt(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for greater-than, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpgt_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpgt_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpgt_epi64_mask(a, b) & m
}

/// Compare packed signed 64-bit integers in a and b for less-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmple_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmple_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_le(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for less-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmple_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmple_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmple_epi64_mask(a, b) & m
}

/// Compare packed signed 64-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpge_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpge_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_ge(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for greater-than-or-equal, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpge_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpge_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpge_epi64_mask(b, a) & m
}

/// Compare packed signed 64-bit integers in a and b for equality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpeq_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpeq_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_eq(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for equality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpeq_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpeq_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpeq_epi64_mask(a, b) & m
}

/// Compare packed signed 64-bit integers in a and b for inequality, and store the results in a mask vector.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062&text=_mm512_cmpneq_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_cmpneq_epi64_mask(a: __m512i, b: __m512i) -> __mmask8 {
    simd_bitmask::<__m512i, _>(simd_ne(a.as_i64x8(), b.as_i64x8()))
}

/// Compare packed signed 64-bit integers in a and b for inequality, and store the results in a mask vector k
/// using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmpneq_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vpcmp))]
pub unsafe fn _mm512_mask_cmpneq_epi64_mask(m: __mmask8, a: __m512i, b: __m512i) -> __mmask8 {
    _mm512_cmpneq_epi64_mask(a, b) & m
}

/// Compare packed signed 64-bit integers in a and b based on the comparison operand specified by op.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epi64_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(2)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_cmp_epi64_mask(a: __m512i, b: __m512i, op: _MM_CMPINT_ENUM) -> __mmask8 {
    let neg_one = -1;
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpq(a.as_i64x8(), b.as_i64x8(), $imm3, neg_one)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Compare packed signed 64-bit integers in a and b based on the comparison operand specified by op,
///  using zeromask m (elements are zeroed out when the corresponding mask bit is not set).
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,1063&text=_mm512_mask_cmp_epi64_mask)
#[inline]
#[target_feature(enable = "avx512f")]
#[rustc_args_required_const(3)]
#[cfg_attr(test, assert_instr(vpcmp, op = 0))]
pub unsafe fn _mm512_mask_cmp_epi64_mask(
    m: __mmask8,
    a: __m512i,
    b: __m512i,
    op: _MM_CMPINT_ENUM,
) -> __mmask8 {
    macro_rules! call {
        ($imm3:expr) => {
            vpcmpq(a.as_i64x8(), b.as_i64x8(), $imm3, m as i8)
        };
    }
    let r = constify_imm3!(op, call);
    transmute(r)
}

/// Returns vector of type `__m512d` with undefined elements.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_undefined_pd)
#[inline]
#[target_feature(enable = "avx512f")]
// This intrinsic has no corresponding instruction.
pub unsafe fn _mm512_undefined_pd() -> __m512d {
    _mm512_set1_pd(0.0)
}

/// Returns vector of type `__m512` with undefined elements.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_undefined_ps)
#[inline]
#[target_feature(enable = "avx512f")]
// This intrinsic has no corresponding instruction.
pub unsafe fn _mm512_undefined_ps() -> __m512 {
    _mm512_set1_ps(0.0)
}

/// Loads 512-bits (composed of 8 packed double-precision (64-bit)
/// floating-point elements) from memory into result.
/// `mem_addr` does not need to be aligned on any particular boundary.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_loadu_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovups))]
pub unsafe fn _mm512_loadu_pd(mem_addr: *const f64) -> __m512d {
    ptr::read_unaligned(mem_addr as *const __m512d)
}

/// Stores 512-bits (composed of 8 packed double-precision (64-bit)
/// floating-point elements) from `a` into memory.
/// `mem_addr` does not need to be aligned on any particular boundary.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_storeu_pd)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovups))]
pub unsafe fn _mm512_storeu_pd(mem_addr: *mut f64, a: __m512d) {
    ptr::write_unaligned(mem_addr as *mut __m512d, a);
}

/// Loads 512-bits (composed of 16 packed single-precision (32-bit)
/// floating-point elements) from memory into result.
/// `mem_addr` does not need to be aligned on any particular boundary.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_loadu_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovups))]
pub unsafe fn _mm512_loadu_ps(mem_addr: *const f32) -> __m512 {
    ptr::read_unaligned(mem_addr as *const __m512)
}

/// Stores 512-bits (composed of 16 packed single-precision (32-bit)
/// floating-point elements) from `a` into memory.
/// `mem_addr` does not need to be aligned on any particular boundary.
///
/// [Intel's documentation](https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=_mm512_storeu_ps)
#[inline]
#[target_feature(enable = "avx512f")]
#[cfg_attr(test, assert_instr(vmovups))]
#[stable(feature = "simd_x86", since = "1.27.0")]
pub unsafe fn _mm512_storeu_ps(mem_addr: *mut f32, a: __m512) {
    ptr::write_unaligned(mem_addr as *mut __m512, a);
}

/// Sets packed 64-bit integers in `dst` with the supplied values in
/// reverse order.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_pd)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_setr_pd(
    e0: f64,
    e1: f64,
    e2: f64,
    e3: f64,
    e4: f64,
    e5: f64,
    e6: f64,
    e7: f64,
) -> __m512d {
    let r = f64x8::new(e0, e1, e2, e3, e4, e5, e6, e7);
    transmute(r)
}

/// Sets packed 64-bit integers in `dst` with the supplied values.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_pd)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set_pd(
    e0: f64,
    e1: f64,
    e2: f64,
    e3: f64,
    e4: f64,
    e5: f64,
    e6: f64,
    e7: f64,
) -> __m512d {
    _mm512_setr_pd(e7, e6, e5, e4, e3, e2, e1, e0)
}

/// Equal
pub const _MM_CMPINT_EQ: _MM_CMPINT_ENUM = 0x00;
/// Less-than
pub const _MM_CMPINT_LT: _MM_CMPINT_ENUM = 0x01;
/// Less-than-or-equal
pub const _MM_CMPINT_LE: _MM_CMPINT_ENUM = 0x02;
/// False
pub const _MM_CMPINT_FALSE: _MM_CMPINT_ENUM = 0x03;
/// Not-equal
pub const _MM_CMPINT_NE: _MM_CMPINT_ENUM = 0x04;
/// Not less-than
pub const _MM_CMPINT_NLT: _MM_CMPINT_ENUM = 0x05;
/// Not less-than-or-equal
pub const _MM_CMPINT_NLE: _MM_CMPINT_ENUM = 0x06;
/// True
pub const _MM_CMPINT_TRUE: _MM_CMPINT_ENUM = 0x07;

/// interval [1, 2)
pub const _MM_MANT_NORM_1_2: _MM_MANTISSA_NORM_ENUM = 0x00;
/// interval [0.5, 2)
pub const _MM_MANT_NORM_P5_2: _MM_MANTISSA_NORM_ENUM = 0x01;
/// interval [0.5, 1)
pub const _MM_MANT_NORM_P5_1: _MM_MANTISSA_NORM_ENUM = 0x02;
/// interval [0.75, 1.5)
pub const _MM_MANT_NORM_P75_1P5: _MM_MANTISSA_NORM_ENUM = 0x03;

/// sign = sign(SRC)
pub const _MM_MANT_SIGN_SRC: _MM_MANTISSA_SIGN_ENUM = 0x00;
/// sign = 0
pub const _MM_MANT_SIGN_ZERO: _MM_MANTISSA_SIGN_ENUM = 0x01;
/// DEST = NaN if sign(SRC) = 1
pub const _MM_MANT_SIGN_NAN: _MM_MANTISSA_SIGN_ENUM = 0x02;

pub const _MM_PERM_AAAA: _MM_PERM_ENUM = 0x00;
pub const _MM_PERM_AAAB: _MM_PERM_ENUM = 0x01;
pub const _MM_PERM_AAAC: _MM_PERM_ENUM = 0x02;
pub const _MM_PERM_AAAD: _MM_PERM_ENUM = 0x03;
pub const _MM_PERM_AABA: _MM_PERM_ENUM = 0x04;
pub const _MM_PERM_AABB: _MM_PERM_ENUM = 0x05;
pub const _MM_PERM_AABC: _MM_PERM_ENUM = 0x06;
pub const _MM_PERM_AABD: _MM_PERM_ENUM = 0x07;
pub const _MM_PERM_AACA: _MM_PERM_ENUM = 0x08;
pub const _MM_PERM_AACB: _MM_PERM_ENUM = 0x09;
pub const _MM_PERM_AACC: _MM_PERM_ENUM = 0x0A;
pub const _MM_PERM_AACD: _MM_PERM_ENUM = 0x0B;
pub const _MM_PERM_AADA: _MM_PERM_ENUM = 0x0C;
pub const _MM_PERM_AADB: _MM_PERM_ENUM = 0x0D;
pub const _MM_PERM_AADC: _MM_PERM_ENUM = 0x0E;
pub const _MM_PERM_AADD: _MM_PERM_ENUM = 0x0F;
pub const _MM_PERM_ABAA: _MM_PERM_ENUM = 0x10;
pub const _MM_PERM_ABAB: _MM_PERM_ENUM = 0x11;
pub const _MM_PERM_ABAC: _MM_PERM_ENUM = 0x12;
pub const _MM_PERM_ABAD: _MM_PERM_ENUM = 0x13;
pub const _MM_PERM_ABBA: _MM_PERM_ENUM = 0x14;
pub const _MM_PERM_ABBB: _MM_PERM_ENUM = 0x15;
pub const _MM_PERM_ABBC: _MM_PERM_ENUM = 0x16;
pub const _MM_PERM_ABBD: _MM_PERM_ENUM = 0x17;
pub const _MM_PERM_ABCA: _MM_PERM_ENUM = 0x18;
pub const _MM_PERM_ABCB: _MM_PERM_ENUM = 0x19;
pub const _MM_PERM_ABCC: _MM_PERM_ENUM = 0x1A;
pub const _MM_PERM_ABCD: _MM_PERM_ENUM = 0x1B;
pub const _MM_PERM_ABDA: _MM_PERM_ENUM = 0x1C;
pub const _MM_PERM_ABDB: _MM_PERM_ENUM = 0x1D;
pub const _MM_PERM_ABDC: _MM_PERM_ENUM = 0x1E;
pub const _MM_PERM_ABDD: _MM_PERM_ENUM = 0x1F;
pub const _MM_PERM_ACAA: _MM_PERM_ENUM = 0x20;
pub const _MM_PERM_ACAB: _MM_PERM_ENUM = 0x21;
pub const _MM_PERM_ACAC: _MM_PERM_ENUM = 0x22;
pub const _MM_PERM_ACAD: _MM_PERM_ENUM = 0x23;
pub const _MM_PERM_ACBA: _MM_PERM_ENUM = 0x24;
pub const _MM_PERM_ACBB: _MM_PERM_ENUM = 0x25;
pub const _MM_PERM_ACBC: _MM_PERM_ENUM = 0x26;
pub const _MM_PERM_ACBD: _MM_PERM_ENUM = 0x27;
pub const _MM_PERM_ACCA: _MM_PERM_ENUM = 0x28;
pub const _MM_PERM_ACCB: _MM_PERM_ENUM = 0x29;
pub const _MM_PERM_ACCC: _MM_PERM_ENUM = 0x2A;
pub const _MM_PERM_ACCD: _MM_PERM_ENUM = 0x2B;
pub const _MM_PERM_ACDA: _MM_PERM_ENUM = 0x2C;
pub const _MM_PERM_ACDB: _MM_PERM_ENUM = 0x2D;
pub const _MM_PERM_ACDC: _MM_PERM_ENUM = 0x2E;
pub const _MM_PERM_ACDD: _MM_PERM_ENUM = 0x2F;
pub const _MM_PERM_ADAA: _MM_PERM_ENUM = 0x30;
pub const _MM_PERM_ADAB: _MM_PERM_ENUM = 0x31;
pub const _MM_PERM_ADAC: _MM_PERM_ENUM = 0x32;
pub const _MM_PERM_ADAD: _MM_PERM_ENUM = 0x33;
pub const _MM_PERM_ADBA: _MM_PERM_ENUM = 0x34;
pub const _MM_PERM_ADBB: _MM_PERM_ENUM = 0x35;
pub const _MM_PERM_ADBC: _MM_PERM_ENUM = 0x36;
pub const _MM_PERM_ADBD: _MM_PERM_ENUM = 0x37;
pub const _MM_PERM_ADCA: _MM_PERM_ENUM = 0x38;
pub const _MM_PERM_ADCB: _MM_PERM_ENUM = 0x39;
pub const _MM_PERM_ADCC: _MM_PERM_ENUM = 0x3A;
pub const _MM_PERM_ADCD: _MM_PERM_ENUM = 0x3B;
pub const _MM_PERM_ADDA: _MM_PERM_ENUM = 0x3C;
pub const _MM_PERM_ADDB: _MM_PERM_ENUM = 0x3D;
pub const _MM_PERM_ADDC: _MM_PERM_ENUM = 0x3E;
pub const _MM_PERM_ADDD: _MM_PERM_ENUM = 0x3F;
pub const _MM_PERM_BAAA: _MM_PERM_ENUM = 0x40;
pub const _MM_PERM_BAAB: _MM_PERM_ENUM = 0x41;
pub const _MM_PERM_BAAC: _MM_PERM_ENUM = 0x42;
pub const _MM_PERM_BAAD: _MM_PERM_ENUM = 0x43;
pub const _MM_PERM_BABA: _MM_PERM_ENUM = 0x44;
pub const _MM_PERM_BABB: _MM_PERM_ENUM = 0x45;
pub const _MM_PERM_BABC: _MM_PERM_ENUM = 0x46;
pub const _MM_PERM_BABD: _MM_PERM_ENUM = 0x47;
pub const _MM_PERM_BACA: _MM_PERM_ENUM = 0x48;
pub const _MM_PERM_BACB: _MM_PERM_ENUM = 0x49;
pub const _MM_PERM_BACC: _MM_PERM_ENUM = 0x4A;
pub const _MM_PERM_BACD: _MM_PERM_ENUM = 0x4B;
pub const _MM_PERM_BADA: _MM_PERM_ENUM = 0x4C;
pub const _MM_PERM_BADB: _MM_PERM_ENUM = 0x4D;
pub const _MM_PERM_BADC: _MM_PERM_ENUM = 0x4E;
pub const _MM_PERM_BADD: _MM_PERM_ENUM = 0x4F;
pub const _MM_PERM_BBAA: _MM_PERM_ENUM = 0x50;
pub const _MM_PERM_BBAB: _MM_PERM_ENUM = 0x51;
pub const _MM_PERM_BBAC: _MM_PERM_ENUM = 0x52;
pub const _MM_PERM_BBAD: _MM_PERM_ENUM = 0x53;
pub const _MM_PERM_BBBA: _MM_PERM_ENUM = 0x54;
pub const _MM_PERM_BBBB: _MM_PERM_ENUM = 0x55;
pub const _MM_PERM_BBBC: _MM_PERM_ENUM = 0x56;
pub const _MM_PERM_BBBD: _MM_PERM_ENUM = 0x57;
pub const _MM_PERM_BBCA: _MM_PERM_ENUM = 0x58;
pub const _MM_PERM_BBCB: _MM_PERM_ENUM = 0x59;
pub const _MM_PERM_BBCC: _MM_PERM_ENUM = 0x5A;
pub const _MM_PERM_BBCD: _MM_PERM_ENUM = 0x5B;
pub const _MM_PERM_BBDA: _MM_PERM_ENUM = 0x5C;
pub const _MM_PERM_BBDB: _MM_PERM_ENUM = 0x5D;
pub const _MM_PERM_BBDC: _MM_PERM_ENUM = 0x5E;
pub const _MM_PERM_BBDD: _MM_PERM_ENUM = 0x5F;
pub const _MM_PERM_BCAA: _MM_PERM_ENUM = 0x60;
pub const _MM_PERM_BCAB: _MM_PERM_ENUM = 0x61;
pub const _MM_PERM_BCAC: _MM_PERM_ENUM = 0x62;
pub const _MM_PERM_BCAD: _MM_PERM_ENUM = 0x63;
pub const _MM_PERM_BCBA: _MM_PERM_ENUM = 0x64;
pub const _MM_PERM_BCBB: _MM_PERM_ENUM = 0x65;
pub const _MM_PERM_BCBC: _MM_PERM_ENUM = 0x66;
pub const _MM_PERM_BCBD: _MM_PERM_ENUM = 0x67;
pub const _MM_PERM_BCCA: _MM_PERM_ENUM = 0x68;
pub const _MM_PERM_BCCB: _MM_PERM_ENUM = 0x69;
pub const _MM_PERM_BCCC: _MM_PERM_ENUM = 0x6A;
pub const _MM_PERM_BCCD: _MM_PERM_ENUM = 0x6B;
pub const _MM_PERM_BCDA: _MM_PERM_ENUM = 0x6C;
pub const _MM_PERM_BCDB: _MM_PERM_ENUM = 0x6D;
pub const _MM_PERM_BCDC: _MM_PERM_ENUM = 0x6E;
pub const _MM_PERM_BCDD: _MM_PERM_ENUM = 0x6F;
pub const _MM_PERM_BDAA: _MM_PERM_ENUM = 0x70;
pub const _MM_PERM_BDAB: _MM_PERM_ENUM = 0x71;
pub const _MM_PERM_BDAC: _MM_PERM_ENUM = 0x72;
pub const _MM_PERM_BDAD: _MM_PERM_ENUM = 0x73;
pub const _MM_PERM_BDBA: _MM_PERM_ENUM = 0x74;
pub const _MM_PERM_BDBB: _MM_PERM_ENUM = 0x75;
pub const _MM_PERM_BDBC: _MM_PERM_ENUM = 0x76;
pub const _MM_PERM_BDBD: _MM_PERM_ENUM = 0x77;
pub const _MM_PERM_BDCA: _MM_PERM_ENUM = 0x78;
pub const _MM_PERM_BDCB: _MM_PERM_ENUM = 0x79;
pub const _MM_PERM_BDCC: _MM_PERM_ENUM = 0x7A;
pub const _MM_PERM_BDCD: _MM_PERM_ENUM = 0x7B;
pub const _MM_PERM_BDDA: _MM_PERM_ENUM = 0x7C;
pub const _MM_PERM_BDDB: _MM_PERM_ENUM = 0x7D;
pub const _MM_PERM_BDDC: _MM_PERM_ENUM = 0x7E;
pub const _MM_PERM_BDDD: _MM_PERM_ENUM = 0x7F;
pub const _MM_PERM_CAAA: _MM_PERM_ENUM = 0x80;
pub const _MM_PERM_CAAB: _MM_PERM_ENUM = 0x81;
pub const _MM_PERM_CAAC: _MM_PERM_ENUM = 0x82;
pub const _MM_PERM_CAAD: _MM_PERM_ENUM = 0x83;
pub const _MM_PERM_CABA: _MM_PERM_ENUM = 0x84;
pub const _MM_PERM_CABB: _MM_PERM_ENUM = 0x85;
pub const _MM_PERM_CABC: _MM_PERM_ENUM = 0x86;
pub const _MM_PERM_CABD: _MM_PERM_ENUM = 0x87;
pub const _MM_PERM_CACA: _MM_PERM_ENUM = 0x88;
pub const _MM_PERM_CACB: _MM_PERM_ENUM = 0x89;
pub const _MM_PERM_CACC: _MM_PERM_ENUM = 0x8A;
pub const _MM_PERM_CACD: _MM_PERM_ENUM = 0x8B;
pub const _MM_PERM_CADA: _MM_PERM_ENUM = 0x8C;
pub const _MM_PERM_CADB: _MM_PERM_ENUM = 0x8D;
pub const _MM_PERM_CADC: _MM_PERM_ENUM = 0x8E;
pub const _MM_PERM_CADD: _MM_PERM_ENUM = 0x8F;
pub const _MM_PERM_CBAA: _MM_PERM_ENUM = 0x90;
pub const _MM_PERM_CBAB: _MM_PERM_ENUM = 0x91;
pub const _MM_PERM_CBAC: _MM_PERM_ENUM = 0x92;
pub const _MM_PERM_CBAD: _MM_PERM_ENUM = 0x93;
pub const _MM_PERM_CBBA: _MM_PERM_ENUM = 0x94;
pub const _MM_PERM_CBBB: _MM_PERM_ENUM = 0x95;
pub const _MM_PERM_CBBC: _MM_PERM_ENUM = 0x96;
pub const _MM_PERM_CBBD: _MM_PERM_ENUM = 0x97;
pub const _MM_PERM_CBCA: _MM_PERM_ENUM = 0x98;
pub const _MM_PERM_CBCB: _MM_PERM_ENUM = 0x99;
pub const _MM_PERM_CBCC: _MM_PERM_ENUM = 0x9A;
pub const _MM_PERM_CBCD: _MM_PERM_ENUM = 0x9B;
pub const _MM_PERM_CBDA: _MM_PERM_ENUM = 0x9C;
pub const _MM_PERM_CBDB: _MM_PERM_ENUM = 0x9D;
pub const _MM_PERM_CBDC: _MM_PERM_ENUM = 0x9E;
pub const _MM_PERM_CBDD: _MM_PERM_ENUM = 0x9F;
pub const _MM_PERM_CCAA: _MM_PERM_ENUM = 0xA0;
pub const _MM_PERM_CCAB: _MM_PERM_ENUM = 0xA1;
pub const _MM_PERM_CCAC: _MM_PERM_ENUM = 0xA2;
pub const _MM_PERM_CCAD: _MM_PERM_ENUM = 0xA3;
pub const _MM_PERM_CCBA: _MM_PERM_ENUM = 0xA4;
pub const _MM_PERM_CCBB: _MM_PERM_ENUM = 0xA5;
pub const _MM_PERM_CCBC: _MM_PERM_ENUM = 0xA6;
pub const _MM_PERM_CCBD: _MM_PERM_ENUM = 0xA7;
pub const _MM_PERM_CCCA: _MM_PERM_ENUM = 0xA8;
pub const _MM_PERM_CCCB: _MM_PERM_ENUM = 0xA9;
pub const _MM_PERM_CCCC: _MM_PERM_ENUM = 0xAA;
pub const _MM_PERM_CCCD: _MM_PERM_ENUM = 0xAB;
pub const _MM_PERM_CCDA: _MM_PERM_ENUM = 0xAC;
pub const _MM_PERM_CCDB: _MM_PERM_ENUM = 0xAD;
pub const _MM_PERM_CCDC: _MM_PERM_ENUM = 0xAE;
pub const _MM_PERM_CCDD: _MM_PERM_ENUM = 0xAF;
pub const _MM_PERM_CDAA: _MM_PERM_ENUM = 0xB0;
pub const _MM_PERM_CDAB: _MM_PERM_ENUM = 0xB1;
pub const _MM_PERM_CDAC: _MM_PERM_ENUM = 0xB2;
pub const _MM_PERM_CDAD: _MM_PERM_ENUM = 0xB3;
pub const _MM_PERM_CDBA: _MM_PERM_ENUM = 0xB4;
pub const _MM_PERM_CDBB: _MM_PERM_ENUM = 0xB5;
pub const _MM_PERM_CDBC: _MM_PERM_ENUM = 0xB6;
pub const _MM_PERM_CDBD: _MM_PERM_ENUM = 0xB7;
pub const _MM_PERM_CDCA: _MM_PERM_ENUM = 0xB8;
pub const _MM_PERM_CDCB: _MM_PERM_ENUM = 0xB9;
pub const _MM_PERM_CDCC: _MM_PERM_ENUM = 0xBA;
pub const _MM_PERM_CDCD: _MM_PERM_ENUM = 0xBB;
pub const _MM_PERM_CDDA: _MM_PERM_ENUM = 0xBC;
pub const _MM_PERM_CDDB: _MM_PERM_ENUM = 0xBD;
pub const _MM_PERM_CDDC: _MM_PERM_ENUM = 0xBE;
pub const _MM_PERM_CDDD: _MM_PERM_ENUM = 0xBF;
pub const _MM_PERM_DAAA: _MM_PERM_ENUM = 0xC0;
pub const _MM_PERM_DAAB: _MM_PERM_ENUM = 0xC1;
pub const _MM_PERM_DAAC: _MM_PERM_ENUM = 0xC2;
pub const _MM_PERM_DAAD: _MM_PERM_ENUM = 0xC3;
pub const _MM_PERM_DABA: _MM_PERM_ENUM = 0xC4;
pub const _MM_PERM_DABB: _MM_PERM_ENUM = 0xC5;
pub const _MM_PERM_DABC: _MM_PERM_ENUM = 0xC6;
pub const _MM_PERM_DABD: _MM_PERM_ENUM = 0xC7;
pub const _MM_PERM_DACA: _MM_PERM_ENUM = 0xC8;
pub const _MM_PERM_DACB: _MM_PERM_ENUM = 0xC9;
pub const _MM_PERM_DACC: _MM_PERM_ENUM = 0xCA;
pub const _MM_PERM_DACD: _MM_PERM_ENUM = 0xCB;
pub const _MM_PERM_DADA: _MM_PERM_ENUM = 0xCC;
pub const _MM_PERM_DADB: _MM_PERM_ENUM = 0xCD;
pub const _MM_PERM_DADC: _MM_PERM_ENUM = 0xCE;
pub const _MM_PERM_DADD: _MM_PERM_ENUM = 0xCF;
pub const _MM_PERM_DBAA: _MM_PERM_ENUM = 0xD0;
pub const _MM_PERM_DBAB: _MM_PERM_ENUM = 0xD1;
pub const _MM_PERM_DBAC: _MM_PERM_ENUM = 0xD2;
pub const _MM_PERM_DBAD: _MM_PERM_ENUM = 0xD3;
pub const _MM_PERM_DBBA: _MM_PERM_ENUM = 0xD4;
pub const _MM_PERM_DBBB: _MM_PERM_ENUM = 0xD5;
pub const _MM_PERM_DBBC: _MM_PERM_ENUM = 0xD6;
pub const _MM_PERM_DBBD: _MM_PERM_ENUM = 0xD7;
pub const _MM_PERM_DBCA: _MM_PERM_ENUM = 0xD8;
pub const _MM_PERM_DBCB: _MM_PERM_ENUM = 0xD9;
pub const _MM_PERM_DBCC: _MM_PERM_ENUM = 0xDA;
pub const _MM_PERM_DBCD: _MM_PERM_ENUM = 0xDB;
pub const _MM_PERM_DBDA: _MM_PERM_ENUM = 0xDC;
pub const _MM_PERM_DBDB: _MM_PERM_ENUM = 0xDD;
pub const _MM_PERM_DBDC: _MM_PERM_ENUM = 0xDE;
pub const _MM_PERM_DBDD: _MM_PERM_ENUM = 0xDF;
pub const _MM_PERM_DCAA: _MM_PERM_ENUM = 0xE0;
pub const _MM_PERM_DCAB: _MM_PERM_ENUM = 0xE1;
pub const _MM_PERM_DCAC: _MM_PERM_ENUM = 0xE2;
pub const _MM_PERM_DCAD: _MM_PERM_ENUM = 0xE3;
pub const _MM_PERM_DCBA: _MM_PERM_ENUM = 0xE4;
pub const _MM_PERM_DCBB: _MM_PERM_ENUM = 0xE5;
pub const _MM_PERM_DCBC: _MM_PERM_ENUM = 0xE6;
pub const _MM_PERM_DCBD: _MM_PERM_ENUM = 0xE7;
pub const _MM_PERM_DCCA: _MM_PERM_ENUM = 0xE8;
pub const _MM_PERM_DCCB: _MM_PERM_ENUM = 0xE9;
pub const _MM_PERM_DCCC: _MM_PERM_ENUM = 0xEA;
pub const _MM_PERM_DCCD: _MM_PERM_ENUM = 0xEB;
pub const _MM_PERM_DCDA: _MM_PERM_ENUM = 0xEC;
pub const _MM_PERM_DCDB: _MM_PERM_ENUM = 0xED;
pub const _MM_PERM_DCDC: _MM_PERM_ENUM = 0xEE;
pub const _MM_PERM_DCDD: _MM_PERM_ENUM = 0xEF;
pub const _MM_PERM_DDAA: _MM_PERM_ENUM = 0xF0;
pub const _MM_PERM_DDAB: _MM_PERM_ENUM = 0xF1;
pub const _MM_PERM_DDAC: _MM_PERM_ENUM = 0xF2;
pub const _MM_PERM_DDAD: _MM_PERM_ENUM = 0xF3;
pub const _MM_PERM_DDBA: _MM_PERM_ENUM = 0xF4;
pub const _MM_PERM_DDBB: _MM_PERM_ENUM = 0xF5;
pub const _MM_PERM_DDBC: _MM_PERM_ENUM = 0xF6;
pub const _MM_PERM_DDBD: _MM_PERM_ENUM = 0xF7;
pub const _MM_PERM_DDCA: _MM_PERM_ENUM = 0xF8;
pub const _MM_PERM_DDCB: _MM_PERM_ENUM = 0xF9;
pub const _MM_PERM_DDCC: _MM_PERM_ENUM = 0xFA;
pub const _MM_PERM_DDCD: _MM_PERM_ENUM = 0xFB;
pub const _MM_PERM_DDDA: _MM_PERM_ENUM = 0xFC;
pub const _MM_PERM_DDDB: _MM_PERM_ENUM = 0xFD;
pub const _MM_PERM_DDDC: _MM_PERM_ENUM = 0xFE;
pub const _MM_PERM_DDDD: _MM_PERM_ENUM = 0xFF;

#[allow(improper_ctypes)]
extern "C" {
    #[link_name = "llvm.x86.avx512.pmul.dq.512"]
    fn vpmuldq(a: i32x16, b: i32x16) -> i64x8;
    #[link_name = "llvm.x86.avx512.pmulu.dq.512"]
    fn vpmuludq(a: u32x16, b: u32x16) -> u64x8;

    #[link_name = "llvm.x86.avx512.mask.pmaxs.d.512"]
    fn vpmaxsd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.pmaxs.q.512"]
    fn vpmaxsq(a: i64x8, b: i64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.mask.pmins.d.512"]
    fn vpminsd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.pmins.q.512"]
    fn vpminsq(a: i64x8, b: i64x8) -> i64x8;

    #[link_name = "llvm.x86.avx512.mask.pmaxu.d.512"]
    fn vpmaxud(a: u32x16, b: u32x16) -> u32x16;
    #[link_name = "llvm.x86.avx512.mask.pmaxu.q.512"]
    fn vpmaxuq(a: u64x8, b: u64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.mask.pminu.d.512"]
    fn vpminud(a: u32x16, b: u32x16) -> u32x16;
    #[link_name = "llvm.x86.avx512.mask.pminu.q.512"]
    fn vpminuq(a: u64x8, b: u64x8) -> i64x8;

    #[link_name = "llvm.x86.avx512.sqrt.ps.512"]
    fn vsqrtps(a: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.sqrt.pd.512"]
    fn vsqrtpd(a: f64x8, rounding: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.vfmadd.ps.512"]
    fn vfmadd132ps(a: f32x16, b: f32x16, c: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.vfmadd.pd.512"]
    fn vfmadd132pd(a: f64x8, b: f64x8, c: f64x8, rounding: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.vfmaddsub.ps.512"]
    fn vfmaddsub213ps(a: f32x16, b: f32x16, c: f32x16, d: i32) -> f32x16; //from clang
    #[link_name = "llvm.x86.avx512.vfmaddsub.pd.512"]
    fn vfmaddsub213pd(a: f64x8, b: f64x8, c: f64x8, d: i32) -> f64x8; //from clang

    #[link_name = "llvm.x86.avx512.add.ps.512"]
    fn vaddps(a: f32x16, b: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.add.pd.512"]
    fn vaddpd(a: f64x8, b: f64x8, rounding: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.sub.ps.512"]
    fn vsubps(a: f32x16, b: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.sub.pd.512"]
    fn vsubpd(a: f64x8, b: f64x8, rounding: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.mul.ps.512"]
    fn vmulps(a: f32x16, b: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.mul.pd.512"]
    fn vmulpd(a: f64x8, b: f64x8, rounding: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.div.ps.512"]
    fn vdivps(a: f32x16, b: f32x16, rounding: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.div.pd.512"]
    fn vdivpd(a: f64x8, b: f64x8, rounding: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.max.ps.512"]
    fn vmaxps(a: f32x16, b: f32x16, sae: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.max.pd.512"]
    fn vmaxpd(a: f64x8, b: f64x8, sae: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.min.ps.512"]
    fn vminps(a: f32x16, b: f32x16, sae: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.min.pd.512"]
    fn vminpd(a: f64x8, b: f64x8, sae: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.mask.getexp.ps.512"]
    fn vgetexpps(a: f32x16, src: f32x16, m: u16, sae: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.mask.getexp.pd.512"]
    fn vgetexppd(a: f64x8, src: f64x8, m: u8, sae: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.mask.getmant.ps.512"]
    fn vgetmantps(a: f32x16, mantissas: i32, src: f32x16, m: u16, sae: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.mask.getmant.pd.512"]
    fn vgetmantpd(a: f64x8, mantissas: i32, src: f64x8, m: u8, sae: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.rcp14.ps.512"]
    fn vrcp14ps(a: f32x16, src: f32x16, m: u16) -> f32x16;
    #[link_name = "llvm.x86.avx512.rcp14.pd.512"]
    fn vrcp14pd(a: f64x8, src: f64x8, m: u8) -> f64x8;
    #[link_name = "llvm.x86.avx512.rsqrt14.ps.512"]
    fn vrsqrt14ps(a: f32x16, src: f32x16, m: u16) -> f32x16;
    #[link_name = "llvm.x86.avx512.rsqrt14.pd.512"]
    fn vrsqrt14pd(a: f64x8, src: f64x8, m: u8) -> f64x8;

    #[link_name = "llvm.x86.avx512.mask.cvtps2dq.512"]
    fn vcvtps2dq(a: f32x16, src: i32x16, mask: u16, rounding: i32) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.cvtps2udq.512"]
    fn vcvtps2udq(a: f32x16, src: u32x16, mask: u16, rounding: i32) -> u32x16;
    #[link_name = "llvm.x86.avx512.mask.cvtps2pd.512"]
    fn vcvtps2pd(a: f32x8, src: f64x8, mask: u8, sae: i32) -> f64x8;

    #[link_name = "llvm.x86.avx512.mask.cvttps2dq.512"]
    fn vcvttps2dq(a: f32x16, src: i32x16, mask: u16, rounding: i32) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.cvttps2udq.512"]
    fn vcvttps2udq(a: f32x16, src: i32x16, mask: u16, rounding: i32) -> u32x16;
    #[link_name = "llvm.x86.avx512.mask.cvttpd2dq.512"]
    fn vcvttpd2dq(a: f64x8, src: i32x8, mask: u8, rounding: i32) -> i32x8;
    #[link_name = "llvm.x86.avx512.mask.cvttpd2udq.512"]
    fn vcvttpd2udq(a: f64x8, src: i32x8, mask: u8, rounding: i32) -> u32x8;

    #[link_name = "llvm.x86.avx512.gather.dpd.512"]
    fn vgatherdpd(src: f64x8, slice: *const i8, offsets: i32x8, mask: i8, scale: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.gather.dps.512"]
    fn vgatherdps(src: f32x16, slice: *const i8, offsets: i32x16, mask: i16, scale: i32) -> f32x16;
    #[link_name = "llvm.x86.avx512.gather.qpd.512"]
    fn vgatherqpd(src: f64x8, slice: *const i8, offsets: i64x8, mask: i8, scale: i32) -> f64x8;
    #[link_name = "llvm.x86.avx512.gather.qps.512"]
    fn vgatherqps(src: f32x8, slice: *const i8, offsets: i64x8, mask: i8, scale: i32) -> f32x8;
    #[link_name = "llvm.x86.avx512.gather.dpq.512"]
    fn vpgatherdq(src: i64x8, slice: *const i8, offsets: i32x8, mask: i8, scale: i32) -> i64x8;
    #[link_name = "llvm.x86.avx512.gather.dpi.512"]
    fn vpgatherdd(src: i32x16, slice: *const i8, offsets: i32x16, mask: i16, scale: i32) -> i32x16;
    #[link_name = "llvm.x86.avx512.gather.qpq.512"]
    fn vpgatherqq(src: i64x8, slice: *const i8, offsets: i64x8, mask: i8, scale: i32) -> i64x8;
    #[link_name = "llvm.x86.avx512.gather.qpi.512"]
    fn vpgatherqd(src: i32x8, slice: *const i8, offsets: i64x8, mask: i8, scale: i32) -> i32x8;

    #[link_name = "llvm.x86.avx512.scatter.dpd.512"]
    fn vscatterdpd(slice: *mut i8, mask: i8, offsets: i32x8, src: f64x8, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.dps.512"]
    fn vscatterdps(slice: *mut i8, mask: i16, offsets: i32x16, src: f32x16, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.qpd.512"]
    fn vscatterqpd(slice: *mut i8, mask: i8, offsets: i64x8, src: f64x8, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.qps.512"]
    fn vscatterqps(slice: *mut i8, mask: i8, offsets: i64x8, src: f32x8, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.dpq.512"]
    fn vpscatterdq(slice: *mut i8, mask: i8, offsets: i32x8, src: i64x8, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.dpi.512"]
    fn vpscatterdd(slice: *mut i8, mask: i16, offsets: i32x16, src: i32x16, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.qpq.512"]
    fn vpscatterqq(slice: *mut i8, mask: i8, offsets: i64x8, src: i64x8, scale: i32);
    #[link_name = "llvm.x86.avx512.scatter.qpi.512"]
    fn vpscatterqd(slice: *mut i8, mask: i8, offsets: i64x8, src: i32x8, scale: i32);

    #[link_name = "llvm.x86.avx512.mask.cmp.ss"]
    fn vcmpss(a: __m128, b: __m128, op: i32, m: i8, sae: i32) -> i8;
    #[link_name = "llvm.x86.avx512.mask.cmp.sd"]
    fn vcmpsd(a: __m128d, b: __m128d, op: i32, m: i8, sae: i32) -> i8;
    #[link_name = "llvm.x86.avx512.mask.cmp.ps.512"]
    fn vcmpps(a: f32x16, b: f32x16, op: i32, m: i16, sae: i32) -> i16;
    #[link_name = "llvm.x86.avx512.mask.cmp.pd.512"]
    fn vcmppd(a: f64x8, b: f64x8, op: i32, m: i8, sae: i32) -> i8;
    #[link_name = "llvm.x86.avx512.mask.ucmp.q.512"]
    fn vpcmpuq(a: i64x8, b: i64x8, op: i32, m: i8) -> i8;
    #[link_name = "llvm.x86.avx512.mask.cmp.q.512"]
    fn vpcmpq(a: i64x8, b: i64x8, op: i32, m: i8) -> i8;
    #[link_name = "llvm.x86.avx512.mask.ucmp.d.512"]
    fn vpcmpud(a: i32x16, b: i32x16, op: i32, m: i16) -> i16;
    #[link_name = "llvm.x86.avx512.mask.cmp.d.512"]
    fn vpcmpd(a: i32x16, b: i32x16, op: i32, m: i16) -> i16;

    #[link_name = "llvm.x86.avx512.mask.prol.d.512"]
    fn vprold(a: i32x16, i8: i32) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.pror.d.512"]
    fn vprord(a: i32x16, i8: i32) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.prol.q.512"]
    fn vprolq(a: i64x8, i8: i32) -> i64x8;
    #[link_name = "llvm.x86.avx512.mask.pror.q.512"]
    fn vprorq(a: i64x8, i8: i32) -> i64x8;

    #[link_name = "llvm.x86.avx512.mask.prolv.d.512"]
    fn vprolvd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.prorv.d.512"]
    fn vprorvd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.mask.prolv.q.512"]
    fn vprolvq(a: i64x8, b: i64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.mask.prorv.q.512"]
    fn vprorvq(a: i64x8, b: i64x8) -> i64x8;

    #[link_name = "llvm.x86.avx512.psllv.d.512"]
    fn vpsllvd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.psrlv.d.512"]
    fn vpsrlvd(a: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.psllv.q.512"]
    fn vpsllvq(a: i64x8, b: i64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.psrlv.q.512"]
    fn vpsrlvq(a: i64x8, b: i64x8) -> i64x8;

    #[link_name = "llvm.x86.avx512.pslli.d.512"]
    fn vpsllid(a: i32x16, imm8: u32) -> i32x16;
    #[link_name = "llvm.x86.avx512.psrli.d.512"]
    fn vpsrlid(a: i32x16, imm8: u32) -> i32x16;
    #[link_name = "llvm.x86.avx512.pslli.q.512"]
    fn vpslliq(a: i64x8, imm8: u32) -> i64x8;
    #[link_name = "llvm.x86.avx512.psrli.q.512"]
    fn vpsrliq(a: i64x8, imm8: u32) -> i64x8;

    #[link_name = "llvm.x86.avx512.psll.d.512"]
    fn vpslld(a: i32x16, count: i32x4) -> i32x16;
    #[link_name = "llvm.x86.avx512.psrl.d.512"]
    fn vpsrld(a: i32x16, count: i32x4) -> i32x16;
    #[link_name = "llvm.x86.avx512.psll.q.512"]
    fn vpsllq(a: i64x8, count: i64x2) -> i64x8;
    #[link_name = "llvm.x86.avx512.psrl.q.512"]
    fn vpsrlq(a: i64x8, count: i64x2) -> i64x8;

    #[link_name = "llvm.x86.avx512.psra.d.512"]
    fn vpsrad(a: i32x16, count: i32x4) -> i32x16;
    #[link_name = "llvm.x86.avx512.psra.q.512"]
    fn vpsraq(a: i64x8, count: i64x2) -> i64x8;

    #[link_name = "llvm.x86.avx512.psrai.d.512"]
    fn vpsraid(a: i32x16, imm8: u32) -> i32x16;
    #[link_name = "llvm.x86.avx512.psrai.q.512"]
    fn vpsraiq(a: i64x8, imm8: u32) -> i64x8;

    #[link_name = "llvm.x86.avx512.psrav.d.512"]
    fn vpsravd(a: i32x16, count: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.psrav.q.512"]
    fn vpsravq(a: i64x8, count: i64x8) -> i64x8;

    #[link_name = "llvm.x86.avx512.vpermilvar.ps.512"]
    fn vpermilps(a: f32x16, b: i32x16) -> f32x16;
    #[link_name = "llvm.x86.avx512.vpermilvar.pd.512"]
    fn vpermilpd(a: f64x8, b: i64x8) -> f64x8;

    #[link_name = "llvm.x86.avx512.permvar.si.512"]
    fn vpermd(a: i32x16, idx: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.permvar.di.512"]
    fn vpermq(a: i64x8, idx: i64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.permvar.sf.512"]
    fn vpermps(a: f32x16, idx: i32x16) -> f32x16;
    #[link_name = "llvm.x86.avx512.permvar.df.512"]
    fn vpermpd(a: f64x8, idx: i64x8) -> f64x8;

    #[link_name = "llvm.x86.avx512.vpermi2var.d.512"]
    fn vpermi2d(a: i32x16, idx: i32x16, b: i32x16) -> i32x16;
    #[link_name = "llvm.x86.avx512.vpermi2var.q.512"]
    fn vpermi2q(a: i64x8, idx: i64x8, b: i64x8) -> i64x8;
    #[link_name = "llvm.x86.avx512.vpermi2var.ps.512"]
    fn vpermi2ps(a: f32x16, idx: i32x16, b: f32x16) -> f32x16;
    #[link_name = "llvm.x86.avx512.vpermi2var.pd.512"]
    fn vpermi2pd(a: f64x8, idx: i64x8, b: f64x8) -> f64x8;
}

#[cfg(test)]
mod tests {
    use std;
    use stdarch_test::simd_test;

    use crate::core_arch::x86::*;
    use crate::hint::black_box;

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_abs_epi32() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi32(
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
        );
        let r = _mm512_abs_epi32(a);
        let e = _mm512_setr_epi32(
            0,
            1,
            1,
            i32::MAX,
            i32::MAX.wrapping_add(1),
            100,
            100,
            32,
            0,
            1,
            1,
            i32::MAX,
            i32::MAX.wrapping_add(1),
            100,
            100,
            32,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_abs_epi32() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi32(
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
        );
        let r = _mm512_mask_abs_epi32(a, 0, a);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_abs_epi32(a, 0b00000000_11111111, a);
        let e = _mm512_setr_epi32(
            0,
            1,
            1,
            i32::MAX,
            i32::MAX.wrapping_add(1),
            100,
            100,
            32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_abs_epi32() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi32(
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
        );
        let r = _mm512_maskz_abs_epi32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_abs_epi32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(
            0,
            1,
            1,
            i32::MAX,
            i32::MAX.wrapping_add(1),
            100,
            100,
            32,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_abs_ps() {
        #[rustfmt::skip]
        let a = _mm512_setr_ps(
            0., 1., -1., f32::MAX,
            f32::MIN, 100., -100., -32.,
            0., 1., -1., f32::MAX,
            f32::MIN, 100., -100., -32.,
        );
        let r = _mm512_abs_ps(a);
        let e = _mm512_setr_ps(
            0.,
            1.,
            1.,
            f32::MAX,
            f32::MAX,
            100.,
            100.,
            32.,
            0.,
            1.,
            1.,
            f32::MAX,
            f32::MAX,
            100.,
            100.,
            32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_abs_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let r = _mm512_mask_abs_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_abs_ps(a, 0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            0.,
            1.,
            1.,
            f32::MAX,
            f32::MAX,
            100.,
            100.,
            32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_add_epi32(a, b);
        let e = _mm512_setr_epi32(
            1,
            2,
            0,
            i32::MIN,
            i32::MIN + 1,
            101,
            -99,
            -31,
            1,
            2,
            0,
            i32::MIN,
            i32::MIN + 1,
            101,
            -99,
            -31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_epi32() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi32(
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_mask_add_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_add_epi32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(
            1,
            2,
            0,
            i32::MIN,
            i32::MIN + 1,
            101,
            -99,
            -31,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_epi32() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi32(
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
            0, 1, -1, i32::MAX,
            i32::MIN, 100, -100, -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_maskz_add_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_add_epi32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(
            1,
            2,
            0,
            i32::MIN,
            i32::MIN + 1,
            101,
            -99,
            -31,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_add_ps(a, b);
        let e = _mm512_setr_ps(
            1.,
            2.,
            0.,
            f32::MAX,
            f32::MIN + 1.,
            101.,
            -99.,
            -31.,
            1.,
            2.,
            0.,
            f32::MAX,
            f32::MIN + 1.,
            101.,
            -99.,
            -31.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_mask_add_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_add_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            1.,
            2.,
            0.,
            f32::MAX,
            f32::MIN + 1.,
            101.,
            -99.,
            -31.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_maskz_add_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_add_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            1.,
            2.,
            0.,
            f32::MAX,
            f32::MIN + 1.,
            101.,
            -99.,
            -31.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_sub_epi32(a, b);
        let e = _mm512_setr_epi32(
            -1,
            0,
            -2,
            i32::MAX - 1,
            i32::MAX,
            99,
            -101,
            -33,
            -1,
            0,
            -2,
            i32::MAX - 1,
            i32::MAX,
            99,
            -101,
            -33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_mask_sub_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_sub_epi32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(
            -1,
            0,
            -2,
            i32::MAX - 1,
            i32::MAX,
            99,
            -101,
            -33,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_maskz_sub_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_sub_epi32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(
            -1,
            0,
            -2,
            i32::MAX - 1,
            i32::MAX,
            99,
            -101,
            -33,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_sub_ps(a, b);
        let e = _mm512_setr_ps(
            -1.,
            0.,
            -2.,
            f32::MAX - 1.,
            f32::MIN,
            99.,
            -101.,
            -33.,
            -1.,
            0.,
            -2.,
            f32::MAX - 1.,
            f32::MIN,
            99.,
            -101.,
            -33.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_mask_sub_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_sub_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            -1.,
            0.,
            -2.,
            f32::MAX - 1.,
            f32::MIN,
            99.,
            -101.,
            -33.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_maskz_sub_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_sub_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            -1.,
            0.,
            -2.,
            f32::MAX - 1.,
            f32::MIN,
            99.,
            -101.,
            -33.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mullo_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(2);
        let r = _mm512_mullo_epi32(a, b);
        let e = _mm512_setr_epi32(
            0, 2, -2, -2, 0, 200, -200, -64, 0, 2, -2, -2, 0, 200, -200, -64,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mullo_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(2);
        let r = _mm512_mask_mullo_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_mullo_epi32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(
            0,
            2,
            -2,
            -2,
            0,
            200,
            -200,
            -64,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mullo_epi32() {
        let a = _mm512_setr_epi32(
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
            0,
            1,
            -1,
            i32::MAX,
            i32::MIN,
            100,
            -100,
            -32,
        );
        let b = _mm512_set1_epi32(2);
        let r = _mm512_maskz_mullo_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_mullo_epi32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(0, 2, -2, -2, 0, 200, -200, -64, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(2.);
        let r = _mm512_mul_ps(a, b);
        let e = _mm512_setr_ps(
            0.,
            2.,
            -2.,
            f32::INFINITY,
            f32::NEG_INFINITY,
            200.,
            -200.,
            -64.,
            0.,
            2.,
            -2.,
            f32::INFINITY,
            f32::NEG_INFINITY,
            200.,
            -200.,
            -64.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(2.);
        let r = _mm512_mask_mul_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_mul_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0.,
            2.,
            -2.,
            f32::INFINITY,
            f32::NEG_INFINITY,
            200.,
            -200.,
            -64.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
            0.,
            1.,
            -1.,
            f32::MAX,
            f32::MIN,
            100.,
            -100.,
            -32.,
        );
        let b = _mm512_set1_ps(2.);
        let r = _mm512_maskz_mul_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_mul_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0.,
            2.,
            -2.,
            f32::INFINITY,
            f32::NEG_INFINITY,
            200.,
            -200.,
            -64.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_div_ps() {
        let a = _mm512_setr_ps(
            0., 1., -1., -2., 100., 100., -100., -32., 0., 1., -1., 1000., -131., 100., -100., -32.,
        );
        let b = _mm512_setr_ps(
            2., 2., 2., 2., 2., 0., 2., 2., 2., 2., 2., 2., 0., 2., 2., 2.,
        );
        let r = _mm512_div_ps(a, b);
        let e = _mm512_setr_ps(
            0.,
            0.5,
            -0.5,
            -1.,
            50.,
            f32::INFINITY,
            -50.,
            -16.,
            0.,
            0.5,
            -0.5,
            500.,
            f32::NEG_INFINITY,
            50.,
            -50.,
            -16.,
        );
        assert_eq_m512(r, e); // 0/0 = NAN
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_div_ps() {
        let a = _mm512_setr_ps(
            0., 1., -1., -2., 100., 100., -100., -32., 0., 1., -1., 1000., -131., 100., -100., -32.,
        );
        let b = _mm512_setr_ps(
            2., 2., 2., 2., 2., 0., 2., 2., 2., 2., 2., 2., 0., 2., 2., 2.,
        );
        let r = _mm512_mask_div_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_div_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0.,
            0.5,
            -0.5,
            -1.,
            50.,
            f32::INFINITY,
            -50.,
            -16.,
            0.,
            1.,
            -1.,
            1000.,
            -131.,
            100.,
            -100.,
            -32.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_div_ps() {
        let a = _mm512_setr_ps(
            0., 1., -1., -2., 100., 100., -100., -32., 0., 1., -1., 1000., -131., 100., -100., -32.,
        );
        let b = _mm512_setr_ps(
            2., 2., 2., 2., 2., 0., 2., 2., 2., 2., 2., 2., 0., 2., 2., 2.,
        );
        let r = _mm512_maskz_div_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_div_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0.,
            0.5,
            -0.5,
            -1.,
            50.,
            f32::INFINITY,
            -50.,
            -16.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_max_epi32(a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_max_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_max_epi32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_max_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_max_epi32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_max_ps(a, b);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_mask_max_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_max_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_maskz_max_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_max_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_max_epu32(a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_max_epu32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_max_epu32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_max_epu32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_max_epu32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_min_epi32(a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_min_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_min_epi32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_epi32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_min_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_min_epi32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_min_ps(a, b);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_mask_min_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_min_ps(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_maskz_min_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_min_ps(0b00000000_11111111, a, b);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_min_epu32(a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_min_epu32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_min_epu32(a, 0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_epu32() {
        let a = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let b = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_min_epu32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_min_epu32(0b00000000_11111111, a, b);
        let e = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sqrt_ps() {
        let a = _mm512_setr_ps(
            0., 1., 4., 9., 16., 25., 36., 49., 64., 81., 100., 121., 144., 169., 196., 225.,
        );
        let r = _mm512_sqrt_ps(a);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sqrt_ps() {
        let a = _mm512_setr_ps(
            0., 1., 4., 9., 16., 25., 36., 49., 64., 81., 100., 121., 144., 169., 196., 225.,
        );
        let r = _mm512_mask_sqrt_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_sqrt_ps(a, 0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 64., 81., 100., 121., 144., 169., 196., 225.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sqrt_ps() {
        let a = _mm512_setr_ps(
            0., 1., 4., 9., 16., 25., 36., 49., 64., 81., 100., 121., 144., 169., 196., 225.,
        );
        let r = _mm512_maskz_sqrt_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_sqrt_ps(0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fmadd_ps(a, b, c);
        let e = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fmadd_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmadd_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fmadd_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmadd_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fmadd_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmadd_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fmsub_ps(a, b, c);
        let e = _mm512_setr_ps(
            -1., 0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fmsub_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmsub_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            -1., 0., 1., 2., 3., 4., 5., 6., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fmsub_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmsub_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            -1., 0., 1., 2., 3., 4., 5., 6., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fmsub_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmsub_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            -1., 0., 1., 2., 3., 4., 5., 6., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmaddsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fmaddsub_ps(a, b, c);
        let e = _mm512_setr_ps(
            -1., 2., 1., 4., 3., 6., 5., 8., 7., 10., 9., 12., 11., 14., 13., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmaddsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fmaddsub_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmaddsub_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            -1., 2., 1., 4., 3., 6., 5., 8., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmaddsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fmaddsub_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmaddsub_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            -1., 2., 1., 4., 3., 6., 5., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmaddsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fmaddsub_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmaddsub_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            -1., 2., 1., 4., 3., 6., 5., 8., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsubadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fmsubadd_ps(a, b, c);
        let e = _mm512_setr_ps(
            1., 0., 3., 2., 5., 4., 7., 6., 9., 8., 11., 10., 13., 12., 15., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsubadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fmsubadd_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmsubadd_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            1., 0., 3., 2., 5., 4., 7., 6., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsubadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fmsubadd_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmsubadd_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            1., 0., 3., 2., 5., 4., 7., 6., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsubadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fmsubadd_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmsubadd_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            1., 0., 3., 2., 5., 4., 7., 6., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fnmadd_ps(a, b, c);
        let e = _mm512_setr_ps(
            1., 0., -1., -2., -3., -4., -5., -6., -7., -8., -9., -10., -11., -12., -13., -14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fnmadd_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fnmadd_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            1., 0., -1., -2., -3., -4., -5., -6., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fnmadd_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fnmadd_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            1., 0., -1., -2., -3., -4., -5., -6., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmadd_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fnmadd_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fnmadd_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            1., 0., -1., -2., -3., -4., -5., -6., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_fnmsub_ps(a, b, c);
        let e = _mm512_setr_ps(
            -1., -2., -3., -4., -5., -6., -7., -8., -9., -10., -11., -12., -13., -14., -15., -16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_mask_fnmsub_ps(a, 0, b, c);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fnmsub_ps(a, 0b00000000_11111111, b, c);
        let e = _mm512_setr_ps(
            -1., -2., -3., -4., -5., -6., -7., -8., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let r = _mm512_maskz_fnmsub_ps(0, a, b, c);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fnmsub_ps(0b00000000_11111111, a, b, c);
        let e = _mm512_setr_ps(
            -1., -2., -3., -4., -5., -6., -7., -8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmsub_ps() {
        let a = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        let b = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let c = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        let r = _mm512_mask3_fnmsub_ps(a, b, c, 0);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fnmsub_ps(a, b, c, 0b00000000_11111111);
        let e = _mm512_setr_ps(
            -1., -2., -3., -4., -5., -6., -7., -8., 2., 2., 2., 2., 2., 2., 2., 2.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rcp14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_rcp14_ps(a);
        let e = _mm512_set1_ps(0.33333206);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rcp14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_mask_rcp14_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_rcp14_ps(a, 0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            3., 3., 3., 3., 3., 3., 3., 3., 0.33333206, 0.33333206, 0.33333206, 0.33333206,
            0.33333206, 0.33333206, 0.33333206, 0.33333206,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rcp14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_maskz_rcp14_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_rcp14_ps(0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 0.33333206, 0.33333206, 0.33333206, 0.33333206,
            0.33333206, 0.33333206, 0.33333206, 0.33333206,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rsqrt14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_rsqrt14_ps(a);
        let e = _mm512_set1_ps(0.5773392);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rsqrt14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_mask_rsqrt14_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_rsqrt14_ps(a, 0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            3., 3., 3., 3., 3., 3., 3., 3., 0.5773392, 0.5773392, 0.5773392, 0.5773392, 0.5773392,
            0.5773392, 0.5773392, 0.5773392,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rsqrt14_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_maskz_rsqrt14_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_rsqrt14_ps(0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 0.5773392, 0.5773392, 0.5773392, 0.5773392, 0.5773392,
            0.5773392, 0.5773392, 0.5773392,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getexp_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_getexp_ps(a);
        let e = _mm512_set1_ps(1.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getexp_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_mask_getexp_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_getexp_ps(a, 0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            3., 3., 3., 3., 3., 3., 3., 3., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getexp_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_maskz_getexp_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_getexp_ps(0b11111111_00000000, a);
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getmant_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_getmant_ps(a, _MM_MANT_NORM_P75_1P5, _MM_MANT_SIGN_NAN);
        let e = _mm512_set1_ps(1.25);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getmant_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_mask_getmant_ps(a, 0, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_getmant_ps(
            a,
            0b11111111_00000000,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
        );
        let e = _mm512_setr_ps(
            10., 10., 10., 10., 10., 10., 10., 10., 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getmant_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_maskz_getmant_ps(0, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r =
            _mm512_maskz_getmant_ps(0b11111111_00000000, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(-1.);
        let r = _mm512_add_round_ps(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -1.,
            0.5,
            1.,
            2.5,
            3.,
            4.5,
            5.,
            6.5,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
        let r = _mm512_add_round_ps(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -1., 0.5, 1., 2.5, 3., 4.5, 5., 6.5, 7., 8.5, 9., 10.5, 11., 12.5, 13., -0.9999999,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(-1.);
        let r = _mm512_mask_add_round_ps(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_add_round_ps(
            a,
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(-1.);
        let r = _mm512_maskz_add_round_ps(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_add_round_ps(
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_sub_round_ps(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -1.,
            0.5,
            1.,
            2.5,
            3.,
            4.5,
            5.,
            6.5,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
        let r = _mm512_sub_round_ps(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -1., 0.5, 1., 2.5, 3., 4.5, 5., 6.5, 7., 8.5, 9., 10.5, 11., 12.5, 13., -0.9999999,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_mask_sub_round_ps(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_sub_round_ps(
            a,
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_round_ps() {
        let a = _mm512_setr_ps(
            0., 1.5, 2., 3.5, 4., 5.5, 6., 7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 0.00000007,
        );
        let b = _mm512_set1_ps(1.);
        let r = _mm512_maskz_sub_round_ps(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_sub_round_ps(
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            7.,
            8.5,
            9.,
            10.5,
            11.,
            12.5,
            13.,
            -0.99999994,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_round_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            8.,
            9.5,
            10.,
            11.5,
            12.,
            13.5,
            14.,
            0.00000000000000000000007,
        );
        let b = _mm512_set1_ps(0.1);
        let r = _mm512_mul_round_ps(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            0.,
            0.15,
            0.2,
            0.35,
            0.4,
            0.55,
            0.6,
            0.75,
            0.8,
            0.95,
            1.0,
            1.15,
            1.2,
            1.35,
            1.4,
            0.000000000000000000000007000001,
        );
        assert_eq_m512(r, e);
        let r = _mm512_mul_round_ps(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            0.,
            0.14999999,
            0.2,
            0.35,
            0.4,
            0.54999995,
            0.59999996,
            0.75,
            0.8,
            0.95,
            1.0,
            1.15,
            1.1999999,
            1.3499999,
            1.4,
            0.000000000000000000000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_round_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            8.,
            9.5,
            10.,
            11.5,
            12.,
            13.5,
            14.,
            0.00000000000000000000007,
        );
        let b = _mm512_set1_ps(0.1);
        let r = _mm512_mask_mul_round_ps(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_mul_round_ps(
            a,
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            0.8,
            0.95,
            1.0,
            1.15,
            1.2,
            1.35,
            1.4,
            0.000000000000000000000007000001,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_round_ps() {
        let a = _mm512_setr_ps(
            0.,
            1.5,
            2.,
            3.5,
            4.,
            5.5,
            6.,
            7.5,
            8.,
            9.5,
            10.,
            11.5,
            12.,
            13.5,
            14.,
            0.00000000000000000000007,
        );
        let b = _mm512_set1_ps(0.1);
        let r = _mm512_maskz_mul_round_ps(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_mul_round_ps(
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.8,
            0.95,
            1.0,
            1.15,
            1.2,
            1.35,
            1.4,
            0.000000000000000000000007000001,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_div_round_ps() {
        let a = _mm512_set1_ps(1.);
        let b = _mm512_set1_ps(3.);
        let r = _mm512_div_round_ps(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.33333334);
        assert_eq_m512(r, e);
        let r = _mm512_div_round_ps(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.3333333);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_div_round_ps() {
        let a = _mm512_set1_ps(1.);
        let b = _mm512_set1_ps(3.);
        let r = _mm512_mask_div_round_ps(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_div_round_ps(
            a,
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            1., 1., 1., 1., 1., 1., 1., 1., 0.33333334, 0.33333334, 0.33333334, 0.33333334,
            0.33333334, 0.33333334, 0.33333334, 0.33333334,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_div_round_ps() {
        let a = _mm512_set1_ps(1.);
        let b = _mm512_set1_ps(3.);
        let r = _mm512_maskz_div_round_ps(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_div_round_ps(
            0b11111111_00000000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 0.33333334, 0.33333334, 0.33333334, 0.33333334,
            0.33333334, 0.33333334, 0.33333334, 0.33333334,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sqrt_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_sqrt_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(1.7320508);
        assert_eq_m512(r, e);
        let r = _mm512_sqrt_round_ps(a, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(1.7320509);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sqrt_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_mask_sqrt_round_ps(a, 0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_sqrt_round_ps(
            a,
            0b11111111_00000000,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            3., 3., 3., 3., 3., 3., 3., 3., 1.7320508, 1.7320508, 1.7320508, 1.7320508, 1.7320508,
            1.7320508, 1.7320508, 1.7320508,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sqrt_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_maskz_sqrt_round_ps(0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_sqrt_round_ps(
            0b11111111_00000000,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 1.7320508, 1.7320508, 1.7320508, 1.7320508, 1.7320508,
            1.7320508, 1.7320508, 1.7320508,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_fmadd_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(-0.99999994);
        assert_eq_m512(r, e);
        let r = _mm512_fmadd_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(-0.9999999);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_mask_fmadd_round_ps(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmadd_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_maskz_fmadd_round_ps(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmadd_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_mask3_fmadd_round_ps(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmadd_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r = _mm512_fmsub_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(-0.99999994);
        assert_eq_m512(r, e);
        let r = _mm512_fmsub_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(-0.9999999);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_mask_fmsub_round_ps(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmsub_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_maskz_fmsub_round_ps(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmsub_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_mask3_fmsub_round_ps(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmsub_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            -0.99999994,
            1.,
            1.,
            1.,
            1.,
            1.,
            1.,
            1.,
            1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmaddsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_fmaddsub_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
        );
        assert_eq_m512(r, e);
        let r = _mm512_fmaddsub_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            1., -0.9999999, 1., -0.9999999, 1., -0.9999999, 1., -0.9999999, 1., -0.9999999, 1.,
            -0.9999999, 1., -0.9999999, 1., -0.9999999,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmaddsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_mask_fmaddsub_round_ps(
            a,
            0,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmaddsub_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmaddsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_maskz_fmaddsub_round_ps(
            0,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmaddsub_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmaddsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_mask3_fmaddsub_round_ps(
            a,
            b,
            c,
            0,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmaddsub_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsubadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_fmsubadd_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
        );
        assert_eq_m512(r, e);
        let r = _mm512_fmsubadd_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_ps(
            -0.9999999, 1., -0.9999999, 1., -0.9999999, 1., -0.9999999, 1., -0.9999999, 1.,
            -0.9999999, 1., -0.9999999, 1., -0.9999999, 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsubadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_mask_fmsubadd_round_ps(
            a,
            0,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, a);
        let r = _mm512_mask_fmsubadd_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
            0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsubadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_maskz_fmsubadd_round_ps(
            0,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fmsubadd_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
            0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsubadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_mask3_fmsubadd_round_ps(
            a,
            b,
            c,
            0,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fmsubadd_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -0.99999994,
            1.0000001,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
            -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r = _mm512_fnmadd_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.99999994);
        assert_eq_m512(r, e);
        let r = _mm512_fnmadd_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.9999999);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_mask_fnmadd_round_ps(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fnmadd_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, 0.00000007, 0.00000007, 0.00000007, 0.00000007, 0.00000007, 0.00000007,
            0.00000007, 0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_maskz_fnmadd_round_ps(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fnmadd_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmadd_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(1.);
        let r =
            _mm512_mask3_fnmadd_round_ps(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fnmadd_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r = _mm512_fnmsub_round_ps(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.99999994);
        assert_eq_m512(r, e);
        let r = _mm512_fnmsub_round_ps(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(0.9999999);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_mask_fnmsub_round_ps(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, a);
        let r = _mm512_mask_fnmsub_round_ps(
            a,
            0b00000000_11111111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, 0.00000007, 0.00000007, 0.00000007, 0.00000007, 0.00000007, 0.00000007,
            0.00000007, 0.00000007,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_maskz_fnmsub_round_ps(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_fnmsub_round_ps(
            0b00000000_11111111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmsub_round_ps() {
        let a = _mm512_set1_ps(0.00000007);
        let b = _mm512_set1_ps(1.);
        let c = _mm512_set1_ps(-1.);
        let r =
            _mm512_mask3_fnmsub_round_ps(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512(r, c);
        let r = _mm512_mask3_fnmsub_round_ps(
            a,
            b,
            c,
            0b00000000_11111111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_ps(
            0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994, 0.99999994,
            0.99999994, -1., -1., -1., -1., -1., -1., -1., -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_max_round_ps(a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_mask_max_round_ps(a, 0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, a);
        let r = _mm512_mask_max_round_ps(a, 0b00000000_11111111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_maskz_max_round_ps(0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_max_round_ps(0b00000000_11111111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_min_round_ps(a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_mask_min_round_ps(a, 0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, a);
        let r = _mm512_mask_min_round_ps(a, 0b00000000_11111111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_round_ps() {
        let a = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_setr_ps(
            15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
        );
        let r = _mm512_maskz_min_round_ps(0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_min_round_ps(0b00000000_11111111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getexp_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_getexp_round_ps(a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_set1_ps(1.);
        assert_eq_m512(r, e);
        let r = _mm512_getexp_round_ps(a, _MM_FROUND_NO_EXC);
        let e = _mm512_set1_ps(1.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getexp_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_mask_getexp_round_ps(a, 0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, a);
        let r = _mm512_mask_getexp_round_ps(a, 0b11111111_00000000, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            3., 3., 3., 3., 3., 3., 3., 3., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getexp_round_ps() {
        let a = _mm512_set1_ps(3.);
        let r = _mm512_maskz_getexp_round_ps(0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_getexp_round_ps(0b11111111_00000000, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getmant_round_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_getmant_round_ps(
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_set1_ps(1.25);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getmant_round_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_mask_getmant_round_ps(
            a,
            0,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        assert_eq_m512(r, a);
        let r = _mm512_mask_getmant_round_ps(
            a,
            0b11111111_00000000,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_setr_ps(
            10., 10., 10., 10., 10., 10., 10., 10., 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getmant_round_ps() {
        let a = _mm512_set1_ps(10.);
        let r = _mm512_maskz_getmant_round_ps(
            0,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_getmant_round_ps(
            0b11111111_00000000,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_setr_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvtps_epi32(a);
        let e = _mm512_setr_epi32(0, -1, 2, -4, 4, -6, 6, -8, 8, 10, 10, 12, 12, 14, 14, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvtps_epi32(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvtps_epi32(src, 0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -4, 4, -6, 6, -8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvtps_epi32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvtps_epi32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -4, 4, -6, 6, -8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvtps_epu32(a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 8, 10, 10, 12, 12, 14, 14, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvtps_epu32(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvtps_epu32(src, 0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.4, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvtps_epu32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvtps_epu32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvt_roundps_epi32(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -2, 2, -4, 4, -6, 6, -8, 8, 10, 10, 12, 12, 14, 14, 16);
        assert_eq_m512i(r, e);
        let r = _mm512_cvt_roundps_epi32(a, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -2, 2, -4, 4, -6, 6, -8, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r =
            _mm512_mask_cvt_roundps_epi32(src, 0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvt_roundps_epi32(
            src,
            0b00000000_11111111,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_epi32(0, -2, 2, -4, 4, -6, 6, -8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvt_roundps_epi32(0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvt_roundps_epi32(
            0b00000000_11111111,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_epi32(0, -2, 2, -4, 4, -6, 6, -8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvt_roundps_epu32(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 8, 10, 10, 12, 12, 14, 14, 16);
        assert_eq_m512i(r, e);
        let r = _mm512_cvt_roundps_epu32(a, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r =
            _mm512_mask_cvt_roundps_epu32(src, 0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvt_roundps_epu32(
            src,
            0b00000000_11111111,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvt_roundps_epu32(0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvt_roundps_epu32(
            0b00000000_11111111,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvtt_roundps_epi32(a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvtt_roundps_epi32(src, 0, a, _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvtt_roundps_epi32(src, 0b00000000_11111111, a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtt_roundps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvtt_roundps_epi32(0, a, _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvtt_roundps_epi32(0b00000000_11111111, a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvtt_roundps_epu32(a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvtt_roundps_epu32(src, 0, a, _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvtt_roundps_epu32(src, 0b00000000_11111111, a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtt_roundps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvtt_roundps_epu32(0, a, _MM_FROUND_NO_EXC);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvtt_roundps_epu32(0b00000000_11111111, a, _MM_FROUND_NO_EXC);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvttps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvttps_epi32(a);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvttps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvttps_epi32(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvttps_epi32(src, 0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvttps_epi32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvttps_epi32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvttps_epi32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvttps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_cvttps_epu32(a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvttps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let src = _mm512_set1_epi32(0);
        let r = _mm512_mask_cvttps_epu32(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_cvttps_epu32(src, 0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvttps_epu32() {
        let a = _mm512_setr_ps(
            0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5, 8., 9.5, 10., 11.5, 12., 13.5, 14., 15.5,
        );
        let r = _mm512_maskz_cvttps_epu32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_cvttps_epu32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32gather_ps() {
        let mut arr = [0f32; 256];
        for i in 0..256 {
            arr[i] = i as f32;
        }
        // A multiplier of 4 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      120, 128, 136, 144, 152, 160, 168, 176);
        let r = _mm512_i32gather_ps(index, arr.as_ptr() as *const u8, 4);
        #[rustfmt::skip]
        assert_eq_m512(r, _mm512_setr_ps(0., 16., 32., 48., 64., 80., 96., 112.,
                                         120., 128., 136., 144., 152., 160., 168., 176.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32gather_ps() {
        let mut arr = [0f32; 256];
        for i in 0..256 {
            arr[i] = i as f32;
        }
        let src = _mm512_set1_ps(2.);
        let mask = 0b10101010_10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      120, 128, 136, 144, 152, 160, 168, 176);
        // A multiplier of 4 is word-addressing
        let r = _mm512_mask_i32gather_ps(src, mask, index, arr.as_ptr() as *const u8, 4);
        #[rustfmt::skip]
        assert_eq_m512(r, _mm512_setr_ps(2., 16., 2., 48., 2., 80., 2., 112.,
                                         2., 128., 2., 144., 2., 160., 2., 176.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32gather_epi32() {
        let mut arr = [0i32; 256];
        for i in 0..256 {
            arr[i] = i as i32;
        }
        // A multiplier of 4 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      120, 128, 136, 144, 152, 160, 168, 176);
        let r = _mm512_i32gather_epi32(index, arr.as_ptr() as *const u8, 4);
        #[rustfmt::skip]
        assert_eq_m512i(r, _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                             120, 128, 136, 144, 152, 160, 168, 176));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32gather_epi32() {
        let mut arr = [0i32; 256];
        for i in 0..256 {
            arr[i] = i as i32;
        }
        let src = _mm512_set1_epi32(2);
        let mask = 0b10101010_10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      128, 144, 160, 176, 192, 208, 224, 240);
        // A multiplier of 4 is word-addressing
        let r = _mm512_mask_i32gather_epi32(src, mask, index, arr.as_ptr() as *const u8, 4);
        #[rustfmt::skip]
        assert_eq_m512i(r, _mm512_setr_epi32(2, 16, 2, 48, 2, 80, 2, 112,
                                             2, 144, 2, 176, 2, 208, 2, 240));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32scatter_ps() {
        let mut arr = [0f32; 256];
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      128, 144, 160, 176, 192, 208, 224, 240);
        let src = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        // A multiplier of 4 is word-addressing
        _mm512_i32scatter_ps(arr.as_mut_ptr() as *mut u8, index, src, 4);
        let mut expected = [0f32; 256];
        for i in 0..16 {
            expected[i * 16] = (i + 1) as f32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32scatter_ps() {
        let mut arr = [0f32; 256];
        let mask = 0b10101010_10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      128, 144, 160, 176, 192, 208, 224, 240);
        let src = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        // A multiplier of 4 is word-addressing
        _mm512_mask_i32scatter_ps(arr.as_mut_ptr() as *mut u8, mask, index, src, 4);
        let mut expected = [0f32; 256];
        for i in 0..8 {
            expected[i * 32 + 16] = 2. * (i + 1) as f32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32scatter_epi32() {
        let mut arr = [0i32; 256];
        #[rustfmt::skip]

        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      128, 144, 160, 176, 192, 208, 224, 240);
        let src = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        // A multiplier of 4 is word-addressing
        _mm512_i32scatter_epi32(arr.as_mut_ptr() as *mut u8, index, src, 4);
        let mut expected = [0i32; 256];
        for i in 0..16 {
            expected[i * 16] = (i + 1) as i32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32scatter_epi32() {
        let mut arr = [0i32; 256];
        let mask = 0b10101010_10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112,
                                      128, 144, 160, 176, 192, 208, 224, 240);
        let src = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        // A multiplier of 4 is word-addressing
        _mm512_mask_i32scatter_epi32(arr.as_mut_ptr() as *mut u8, mask, index, src, 4);
        let mut expected = [0i32; 256];
        for i in 0..8 {
            expected[i * 32 + 16] = 2 * (i + 1) as i32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let m = _mm512_cmplt_ps_mask(a, b);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmplt_ps_mask(mask, a, b);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpnlt_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        assert_eq!(_mm512_cmpnlt_ps_mask(a, b), !_mm512_cmplt_ps_mask(a, b));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpnlt_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01111010_01111010;
        assert_eq!(_mm512_mask_cmpnlt_ps_mask(mask, a, b), 0b01111010_01111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpnle_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let m = _mm512_cmpnle_ps_mask(b, a);
        assert_eq!(m, 0b00001101_00001101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpnle_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmpnle_ps_mask(mask, b, a);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        assert_eq!(_mm512_cmple_ps_mask(a, b), 0b00100101_00100101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.,
                              0., 1., -1., f32::MAX, f32::NAN, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01111010_01111010;
        assert_eq!(_mm512_mask_cmple_ps_mask(mask, a, b), 0b00100000_00100000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.);
        let m = _mm512_cmpeq_ps_mask(b, a);
        assert_eq!(m, 0b11001101_11001101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpeq_ps_mask(mask, b, a);
        assert_eq!(r, 0b01001000_01001000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpneq_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.);
        let m = _mm512_cmpneq_ps_mask(b, a);
        assert_eq!(m, 0b00110010_00110010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, f32::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.,
                              0., 1., 13., 42., f32::MAX, f32::MIN, f32::NAN, -100.);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpneq_ps_mask(mask, b, a);
        assert_eq!(r, 0b00110010_00110010)
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let m = _mm512_cmp_ps_mask(a, b, _CMP_LT_OQ);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmp_ps_mask(mask, a, b, _CMP_LT_OQ);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_round_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let m = _mm512_cmp_round_ps_mask(a, b, _CMP_LT_OQ, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_round_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.,
                              0., 1., -1., 13., f32::MAX, f32::MIN, 100., -100.);
        let b = _mm512_set1_ps(-1.);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmp_round_ps_mask(mask, a, b, _CMP_LT_OQ, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpord_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, -1., f32::NAN, 0.,
                              f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, 1., f32::NAN, 2.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 0.,
                              f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 2.);
        let m = _mm512_cmpord_ps_mask(a, b);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpord_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, -1., f32::NAN, 0.,
                              f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, 1., f32::NAN, 2.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 0.,
                              f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 2.);
        let mask = 0b11000011_11000011;
        let m = _mm512_mask_cmpord_ps_mask(mask, a, b);
        assert_eq!(m, 0b00000001_00000001);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpunord_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, -1., f32::NAN, 0.,
                              f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, 1., f32::NAN, 2.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 0.,
                              f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 2.);
        let m = _mm512_cmpunord_ps_mask(a, b);

        assert_eq!(m, 0b11111010_11111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpunord_ps_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_ps(f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, -1., f32::NAN, 0.,
                              f32::NAN, f32::MAX, f32::NAN, f32::MIN, f32::NAN, 1., f32::NAN, 2.);
        #[rustfmt::skip]
        let b = _mm512_set_ps(f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 0.,
                              f32::NAN, f32::NAN, f32::NAN, f32::NAN, f32::MIN, f32::MAX, -1., 2.);
        let mask = 0b00001111_00001111;
        let m = _mm512_mask_cmpunord_ps_mask(mask, a, b);
        assert_eq!(m, 0b000001010_00001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_cmp_ss_mask() {
        let a = _mm_setr_ps(2., 1., 1., 1.);
        let b = _mm_setr_ps(1., 2., 2., 2.);
        let m = _mm_cmp_ss_mask(a, b, _CMP_GE_OS);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_mask_cmp_ss_mask() {
        let a = _mm_setr_ps(2., 1., 1., 1.);
        let b = _mm_setr_ps(1., 2., 2., 2.);
        let m = _mm_mask_cmp_ss_mask(0b10, a, b, _CMP_GE_OS);
        assert_eq!(m, 0);
        let m = _mm_mask_cmp_ss_mask(0b1, a, b, _CMP_GE_OS);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_cmp_round_ss_mask() {
        let a = _mm_setr_ps(2., 1., 1., 1.);
        let b = _mm_setr_ps(1., 2., 2., 2.);
        let m = _mm_cmp_round_ss_mask(a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_mask_cmp_round_ss_mask() {
        let a = _mm_setr_ps(2., 1., 1., 1.);
        let b = _mm_setr_ps(1., 2., 2., 2.);
        let m = _mm_mask_cmp_round_ss_mask(0b10, a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 0);
        let m = _mm_mask_cmp_round_ss_mask(0b1, a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_cmp_sd_mask() {
        let a = _mm_setr_pd(2., 1.);
        let b = _mm_setr_pd(1., 2.);
        let m = _mm_cmp_sd_mask(a, b, _CMP_GE_OS);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_mask_cmp_sd_mask() {
        let a = _mm_setr_pd(2., 1.);
        let b = _mm_setr_pd(1., 2.);
        let m = _mm_mask_cmp_sd_mask(0b10, a, b, _CMP_GE_OS);
        assert_eq!(m, 0);
        let m = _mm_mask_cmp_sd_mask(0b1, a, b, _CMP_GE_OS);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_cmp_round_sd_mask() {
        let a = _mm_setr_pd(2., 1.);
        let b = _mm_setr_pd(1., 2.);
        let m = _mm_cmp_round_sd_mask(a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm_mask_cmp_round_sd_mask() {
        let a = _mm_setr_pd(2., 1.);
        let b = _mm_setr_pd(1., 2.);
        let m = _mm_mask_cmp_round_sd_mask(0b10, a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 0);
        let m = _mm_mask_cmp_round_sd_mask(0b1, a, b, _CMP_GE_OS, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 1);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmplt_epu32_mask(a, b);
        assert_eq!(m, 0b11001111_11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmplt_epu32_mask(mask, a, b);
        assert_eq!(r, 0b01001010_01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpgt_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmpgt_epu32_mask(b, a);
        assert_eq!(m, 0b11001111_11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpgt_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpgt_epu32_mask(mask, b, a);
        assert_eq!(r, 0b01001010_01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        assert_eq!(
            _mm512_cmple_epu32_mask(a, b),
            !_mm512_cmpgt_epu32_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        assert_eq!(
            _mm512_mask_cmple_epu32_mask(mask, a, b),
            0b01111010_01111010
        );
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpge_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        assert_eq!(
            _mm512_cmpge_epu32_mask(a, b),
            !_mm512_cmplt_epu32_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpge_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        assert_eq!(_mm512_mask_cmpge_epu32_mask(mask, a, b), 0b01100000_0110000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let m = _mm512_cmpeq_epu32_mask(b, a);
        assert_eq!(m, 0b11001111_11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpeq_epu32_mask(mask, b, a);
        assert_eq!(r, 0b01001010_01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpneq_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let m = _mm512_cmpneq_epu32_mask(b, a);
        assert_eq!(m, !_mm512_cmpeq_epu32_mask(b, a));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, -100, 100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, -100, 100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpneq_epu32_mask(mask, b, a);
        assert_eq!(r, 0b00110010_00110010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmp_epu32_mask(a, b, _MM_CMPINT_LT);
        assert_eq!(m, 0b11001111_11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_epu32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmp_epu32_mask(mask, a, b, _MM_CMPINT_LT);
        assert_eq!(r, 0b01001010_01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmplt_epi32_mask(a, b);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmplt_epi32_mask(mask, a, b);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpgt_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmpgt_epi32_mask(b, a);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpgt_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmpgt_epi32_mask(mask, b, a);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        assert_eq!(
            _mm512_cmple_epi32_mask(a, b),
            !_mm512_cmpgt_epi32_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        assert_eq!(_mm512_mask_cmple_epi32_mask(mask, a, b), 0b01100000_0110000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpge_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        assert_eq!(
            _mm512_cmpge_epi32_mask(a, b),
            !_mm512_cmplt_epi32_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpge_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, u32::MAX as i32, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01111010_01111010;
        assert_eq!(
            _mm512_mask_cmpge_epi32_mask(mask, a, b),
            0b01111010_01111010
        );
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let m = _mm512_cmpeq_epi32_mask(b, a);
        assert_eq!(m, 0b11001111_11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpeq_epi32_mask(mask, b, a);
        assert_eq!(r, 0b01001010_01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpneq_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let m = _mm512_cmpneq_epi32_mask(b, a);
        assert_eq!(m, !_mm512_cmpeq_epi32_mask(b, a));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, -100, 100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, -100, 100);
        #[rustfmt::skip]
        let b = _mm512_set_epi32(0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, 13, 42, i32::MAX, i32::MIN, 100, -100);
        let mask = 0b01111010_01111010;
        let r = _mm512_mask_cmpneq_epi32_mask(mask, b, a);
        assert_eq!(r, 0b00110010_00110010)
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let m = _mm512_cmp_epi32_mask(a, b, _MM_CMPINT_LT);
        assert_eq!(m, 0b00000101_00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_epi32_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_epi32(0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100,
                                 0, 1, -1, 13, i32::MAX, i32::MIN, 100, -100);
        let b = _mm512_set1_epi32(-1);
        let mask = 0b01100110_01100110;
        let r = _mm512_mask_cmp_epi32_mask(mask, a, b, _MM_CMPINT_LT);
        assert_eq!(r, 0b00000100_00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set_epi32() {
        let r = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(
            r,
            _mm512_set_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0),
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setr_epi32() {
        let r = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        assert_eq_m512i(
            r,
            _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0),
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set1_epi32() {
        let r = _mm512_set_epi32(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, _mm512_set1_epi32(2));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setzero_si512() {
        assert_eq_m512i(_mm512_set1_epi32(0), _mm512_setzero_si512());
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set_ps() {
        let r = _mm512_setr_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(
            r,
            _mm512_set_ps(
                15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
            ),
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setr_ps() {
        let r = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        assert_eq_m512(
            r,
            _mm512_setr_ps(
                15., 14., 13., 12., 11., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1., 0.,
            ),
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set1_ps() {
        #[rustfmt::skip]
        let expected = _mm512_set_ps(2., 2., 2., 2., 2., 2., 2., 2.,
                                     2., 2., 2., 2., 2., 2., 2., 2.);
        assert_eq_m512(expected, _mm512_set1_ps(2.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setzero_ps() {
        assert_eq_m512(_mm512_setzero_ps(), _mm512_set1_ps(0.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_loadu_pd() {
        let a = &[4., 3., 2., 5., 8., 9., 64., 50.];
        let p = a.as_ptr();
        let r = _mm512_loadu_pd(black_box(p));
        let e = _mm512_setr_pd(4., 3., 2., 5., 8., 9., 64., 50.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_storeu_pd() {
        let a = _mm512_set1_pd(9.);
        let mut r = _mm512_undefined_pd();
        _mm512_storeu_pd(&mut r as *mut _ as *mut f64, a);
        assert_eq_m512d(r, a);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_loadu_ps() {
        let a = &[
            4., 3., 2., 5., 8., 9., 64., 50., -4., -3., -2., -5., -8., -9., -64., -50.,
        ];
        let p = a.as_ptr();
        let r = _mm512_loadu_ps(black_box(p));
        let e = _mm512_setr_ps(
            4., 3., 2., 5., 8., 9., 64., 50., -4., -3., -2., -5., -8., -9., -64., -50.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_storeu_ps() {
        let a = _mm512_set1_ps(9.);
        let mut r = _mm512_undefined_ps();
        _mm512_storeu_ps(&mut r as *mut _ as *mut f32, a);
        assert_eq_m512(r, a);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setr_pd() {
        let r = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        assert_eq_m512d(r, _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set_pd() {
        let r = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        assert_eq_m512d(r, _mm512_set_pd(7., 6., 5., 4., 3., 2., 1., 0.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rol_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let r = _mm512_rol_epi32(a, 1);
        let e = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rol_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let r = _mm512_mask_rol_epi32(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rol_epi32(a, 0b11111111_11111111, a, 1);
        let e = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rol_epi32() {
        let a = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        let r = _mm512_maskz_rol_epi32(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rol_epi32(0b00000000_11111111, a, 1);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1 << 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_ror_epi32() {
        let a = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let r = _mm512_ror_epi32(a, 1);
        let e = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_ror_epi32() {
        let a = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let r = _mm512_mask_ror_epi32(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_ror_epi32(a, 0b11111111_11111111, a, 1);
        let e = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_ror_epi32() {
        let a = _mm512_set_epi32(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 << 0);
        let r = _mm512_maskz_ror_epi32(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_ror_epi32(0b00000000_11111111, a, 1);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_slli_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let r = _mm512_slli_epi32(a, 1);
        let e = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_slli_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let r = _mm512_mask_slli_epi32(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_slli_epi32(a, 0b11111111_11111111, a, 1);
        let e = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_slli_epi32() {
        let a = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        let r = _mm512_maskz_slli_epi32(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_slli_epi32(0b00000000_11111111, a, 1);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srli_epi32() {
        let a = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let r = _mm512_srli_epi32(a, 1);
        let e = _mm512_set_epi32(0 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srli_epi32() {
        let a = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let r = _mm512_mask_srli_epi32(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srli_epi32(a, 0b11111111_11111111, a, 1);
        let e = _mm512_set_epi32(0 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srli_epi32() {
        let a = _mm512_set_epi32(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0);
        let r = _mm512_maskz_srli_epi32(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srli_epi32(0b00000000_11111111, a, 1);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 << 31);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rolv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_rolv_epi32(a, b);

        let e = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rolv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_mask_rolv_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rolv_epi32(a, 0b11111111_11111111, a, b);

        let e = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rolv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        let b = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_maskz_rolv_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rolv_epi32(0b00000000_11111111, a, b);

        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1 << 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rorv_epi32() {
        let a = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let b = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_rorv_epi32(a, b);

        let e = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rorv_epi32() {
        let a = _mm512_set_epi32(1 << 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let b = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_mask_rorv_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rorv_epi32(a, 0b11111111_11111111, a, b);

        let e = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rorv_epi32() {
        let a = _mm512_set_epi32(3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 << 0);
        let b = _mm512_set_epi32(2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_maskz_rorv_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rorv_epi32(0b00000000_11111111, a, b);

        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sllv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let count = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_sllv_epi32(a, count);

        let e = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sllv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let count = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_mask_sllv_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sllv_epi32(a, 0b11111111_11111111, a, count);

        let e = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sllv_epi32() {
        let a = _mm512_set_epi32(1 << 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 << 31);
        let count = _mm512_set_epi32(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_maskz_sllv_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sllv_epi32(0b00000000_11111111, a, count);

        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srlv_epi32() {
        let a = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let count = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_srlv_epi32(a, count);

        let e = _mm512_set_epi32(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srlv_epi32() {
        let a = _mm512_set_epi32(0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2);
        let count = _mm512_set_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_mask_srlv_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srlv_epi32(a, 0b11111111_11111111, a, count);

        let e = _mm512_set_epi32(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srlv_epi32() {
        let a = _mm512_set_epi32(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0);
        let count = _mm512_set_epi32(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        let r = _mm512_maskz_srlv_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srlv_epi32(0b00000000_11111111, a, count);

        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sll_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        let count = _mm_set_epi32(0, 0, 0, 2);
        let r = _mm512_sll_epi32(a, count);
        let e = _mm512_set_epi32(
            0,
            1 << 2,
            1 << 3,
            1 << 4,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sll_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        let count = _mm_set_epi32(0, 0, 0, 2);
        let r = _mm512_mask_sll_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sll_epi32(a, 0b11111111_11111111, a, count);
        let e = _mm512_set_epi32(
            0,
            1 << 2,
            1 << 3,
            1 << 4,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sll_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 31,
        );
        let count = _mm_set_epi32(2, 0, 0, 2);
        let r = _mm512_maskz_sll_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sll_epi32(0b00000000_11111111, a, count);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srl_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        let count = _mm_set_epi32(0, 0, 0, 2);
        let r = _mm512_srl_epi32(a, count);
        let e = _mm512_set_epi32(1 << 29, 0, 0, 1 << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srl_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
        );
        let count = _mm_set_epi32(0, 0, 0, 2);
        let r = _mm512_mask_srl_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srl_epi32(a, 0b11111111_11111111, a, count);
        let e = _mm512_set_epi32(1 << 29, 0, 0, 1 << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srl_epi32() {
        let a = _mm512_set_epi32(
            1 << 31,
            1 << 0,
            1 << 1,
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 31,
        );
        let count = _mm_set_epi32(2, 0, 0, 2);
        let r = _mm512_maskz_srl_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srl_epi32(0b00000000_11111111, a, count);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 << 29);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sra_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        let count = _mm_set_epi32(1, 0, 0, 2);
        let r = _mm512_sra_epi32(a, count);
        let e = _mm512_set_epi32(2, -2, 4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sra_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16);
        let count = _mm_set_epi32(0, 0, 0, 2);
        let r = _mm512_mask_sra_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sra_epi32(a, 0b11111111_11111111, a, count);
        let e = _mm512_set_epi32(2, -2, 4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sra_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -14);
        let count = _mm_set_epi32(2, 0, 0, 2);
        let r = _mm512_maskz_sra_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sra_epi32(0b00000000_11111111, a, count);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srav_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        let count = _mm512_set_epi32(2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        let r = _mm512_srav_epi32(a, count);
        let e = _mm512_set_epi32(2, -2, 4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srav_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16);
        let count = _mm512_set_epi32(2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        let r = _mm512_mask_srav_epi32(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srav_epi32(a, 0b11111111_11111111, a, count);
        let e = _mm512_set_epi32(2, -2, 4, -4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srav_epi32() {
        let a = _mm512_set_epi32(8, -8, 16, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -15, -14);
        let count = _mm512_set_epi32(2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2);
        let r = _mm512_maskz_srav_epi32(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srav_epi32(0b00000000_11111111, a, count);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srai_epi32() {
        let a = _mm512_set_epi32(8, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, -15);
        let r = _mm512_srai_epi32(a, 2);
        let e = _mm512_set_epi32(2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srai_epi32() {
        let a = _mm512_set_epi32(8, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, -15);
        let r = _mm512_mask_srai_epi32(a, 0, a, 2);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srai_epi32(a, 0b11111111_11111111, a, 2);
        let e = _mm512_set_epi32(2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srai_epi32() {
        let a = _mm512_set_epi32(8, -8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, -15);
        let r = _mm512_maskz_srai_epi32(0, a, 2);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srai_epi32(0b00000000_11111111, a, 2);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permute_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_permute_ps(a, 1);
        let e = _mm512_set_ps(
            2., 2., 2., 2., 6., 6., 6., 6., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permute_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_mask_permute_ps(a, 0b00000000_00000000, a, 1);
        assert_eq_m512(r, a);
        let r = _mm512_mask_permute_ps(a, 0b11111111_11111111, a, 1);
        let e = _mm512_set_ps(
            2., 2., 2., 2., 6., 6., 6., 6., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permute_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_maskz_permute_ps(0, a, 1);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_permute_ps(0b00000000_11111111, a, 1);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutevar_epi32() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let r = _mm512_permutevar_epi32(idx, a);
        let e = _mm512_set1_epi32(14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutevar_epi32() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let r = _mm512_mask_permutevar_epi32(a, 0, idx, a);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutevar_epi32(a, 0b11111111_11111111, idx, a);
        let e = _mm512_set1_epi32(14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutevar_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_permutevar_ps(a, b);
        let e = _mm512_set_ps(
            2., 2., 2., 2., 6., 6., 6., 6., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutevar_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_mask_permutevar_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_permutevar_ps(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_ps(
            2., 2., 2., 2., 6., 6., 6., 6., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutevar_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let b = _mm512_set1_epi32(1);
        let r = _mm512_maskz_permutevar_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_permutevar_ps(0b00000000_11111111, a, b);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 10., 10., 10., 10., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutexvar_epi32() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let r = _mm512_permutexvar_epi32(idx, a);
        let e = _mm512_set1_epi32(14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutexvar_epi32() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let r = _mm512_mask_permutexvar_epi32(a, 0, idx, a);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutexvar_epi32(a, 0b11111111_11111111, idx, a);
        let e = _mm512_set1_epi32(14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutexvar_epi32() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let r = _mm512_maskz_permutexvar_epi32(0, idx, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_permutexvar_epi32(0b00000000_11111111, idx, a);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 14, 14, 14, 14, 14, 14, 14, 14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutexvar_ps() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_permutexvar_ps(idx, a);
        let e = _mm512_set1_ps(14.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutexvar_ps() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_mask_permutexvar_ps(a, 0, idx, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_permutexvar_ps(a, 0b11111111_11111111, idx, a);
        let e = _mm512_set1_ps(14.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutexvar_ps() {
        let idx = _mm512_set1_epi32(1);
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let r = _mm512_maskz_permutexvar_ps(0, idx, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_permutexvar_ps(0b00000000_11111111, idx, a);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 14., 14., 14., 14., 14., 14., 14., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex2var_epi32() {
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_epi32(100);
        let r = _mm512_permutex2var_epi32(a, idx, b);
        let e = _mm512_set_epi32(
            14, 100, 13, 100, 12, 100, 11, 100, 10, 100, 9, 100, 8, 100, 7, 100,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex2var_epi32() {
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_epi32(100);
        let r = _mm512_mask_permutex2var_epi32(a, 0, idx, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutex2var_epi32(a, 0b11111111_11111111, idx, b);
        let e = _mm512_set_epi32(
            14, 100, 13, 100, 12, 100, 11, 100, 10, 100, 9, 100, 8, 100, 7, 100,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex2var_epi32() {
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_epi32(100);
        let r = _mm512_maskz_permutex2var_epi32(0, a, idx, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_permutex2var_epi32(0b00000000_11111111, a, idx, b);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 10, 100, 9, 100, 8, 100, 7, 100);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask2_permutex2var_epi32() {
        let a = _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        let idx = _mm512_set_epi32(
            1000,
            1 << 4,
            2000,
            1 << 4,
            3000,
            1 << 4,
            4000,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_epi32(100);
        let r = _mm512_mask2_permutex2var_epi32(a, idx, 0, b);
        assert_eq_m512i(r, idx);
        let r = _mm512_mask2_permutex2var_epi32(a, idx, 0b00000000_11111111, b);
        let e = _mm512_set_epi32(
            1000,
            1 << 4,
            2000,
            1 << 4,
            3000,
            1 << 4,
            4000,
            1 << 4,
            10,
            100,
            9,
            100,
            8,
            100,
            7,
            100,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex2var_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_ps(100.);
        let r = _mm512_permutex2var_ps(a, idx, b);
        let e = _mm512_set_ps(
            14., 100., 13., 100., 12., 100., 11., 100., 10., 100., 9., 100., 8., 100., 7., 100.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex2var_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_ps(100.);
        let r = _mm512_mask_permutex2var_ps(a, 0, idx, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_permutex2var_ps(a, 0b11111111_11111111, idx, b);
        let e = _mm512_set_ps(
            14., 100., 13., 100., 12., 100., 11., 100., 10., 100., 9., 100., 8., 100., 7., 100.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex2var_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_ps(100.);
        let r = _mm512_maskz_permutex2var_ps(0, a, idx, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_permutex2var_ps(0b00000000_11111111, a, idx, b);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 10., 100., 9., 100., 8., 100., 7., 100.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask2_permutex2var_ps() {
        let a = _mm512_set_ps(
            0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15.,
        );
        let idx = _mm512_set_epi32(
            1,
            1 << 4,
            2,
            1 << 4,
            3,
            1 << 4,
            4,
            1 << 4,
            5,
            1 << 4,
            6,
            1 << 4,
            7,
            1 << 4,
            8,
            1 << 4,
        );
        let b = _mm512_set1_ps(100.);
        let r = _mm512_mask2_permutex2var_ps(a, idx, 0, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_mask2_permutex2var_ps(a, idx, 0b00000000_11111111, b);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 10., 100., 9., 100., 8., 100., 7., 100.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_epi32() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let r = _mm512_shuffle_epi32(a, _MM_PERM_AADD);
        let e = _mm512_setr_epi32(8, 8, 1, 1, 16, 16, 9, 9, 8, 8, 1, 1, 16, 16, 9, 9);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_epi32() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let r = _mm512_mask_shuffle_epi32(a, 0, a, _MM_PERM_AADD);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_shuffle_epi32(a, 0b11111111_11111111, a, _MM_PERM_AADD);
        let e = _mm512_setr_epi32(8, 8, 1, 1, 16, 16, 9, 9, 8, 8, 1, 1, 16, 16, 9, 9);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_epi32() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let r = _mm512_maskz_shuffle_epi32(0, a, _MM_PERM_AADD);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_shuffle_epi32(0b00000000_11111111, a, _MM_PERM_AADD);
        let e = _mm512_setr_epi32(8, 8, 1, 1, 16, 16, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_ps() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_shuffle_ps(a, b, 0x0F);
        let e = _mm512_setr_ps(
            8., 8., 2., 2., 16., 16., 10., 10., 8., 8., 2., 2., 16., 16., 10., 10.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_ps() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_mask_shuffle_ps(a, 0, a, b, 0x0F);
        assert_eq_m512(r, a);
        let r = _mm512_mask_shuffle_ps(a, 0b11111111_11111111, a, b, 0x0F);
        let e = _mm512_setr_ps(
            8., 8., 2., 2., 16., 16., 10., 10., 8., 8., 2., 2., 16., 16., 10., 10.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_ps() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_maskz_shuffle_ps(0, a, b, 0x0F);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_shuffle_ps(0b00000000_11111111, a, b, 0x0F);
        let e = _mm512_setr_ps(
            8., 8., 2., 2., 16., 16., 10., 10., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_i32x4() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi32(2, 3, 6, 7, 10, 11, 14, 15, 2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_shuffle_i32x4(a, b, 0b00000000);
        let e = _mm512_setr_epi32(1, 4, 5, 8, 1, 4, 5, 8, 2, 3, 6, 7, 2, 3, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_i32x4() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi32(2, 3, 6, 7, 10, 11, 14, 15, 2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_mask_shuffle_i32x4(a, 0, a, b, 0b00000000);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_shuffle_i32x4(a, 0b11111111_11111111, a, b, 0b00000000);
        let e = _mm512_setr_epi32(1, 4, 5, 8, 1, 4, 5, 8, 2, 3, 6, 7, 2, 3, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_i32x4() {
        let a = _mm512_setr_epi32(1, 4, 5, 8, 9, 12, 13, 16, 1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi32(2, 3, 6, 7, 10, 11, 14, 15, 2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_maskz_shuffle_i32x4(0, a, b, 0b00000000);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_shuffle_i32x4(0b00000000_11111111, a, b, 0b00000000);
        let e = _mm512_setr_epi32(1, 4, 5, 8, 1, 4, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_f32x4() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_shuffle_f32x4(a, b, 0b00000000);
        let e = _mm512_setr_ps(
            1., 4., 5., 8., 1., 4., 5., 8., 2., 3., 6., 7., 2., 3., 6., 7.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_f32x4() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_mask_shuffle_f32x4(a, 0, a, b, 0b00000000);
        assert_eq_m512(r, a);
        let r = _mm512_mask_shuffle_f32x4(a, 0b11111111_11111111, a, b, 0b00000000);
        let e = _mm512_setr_ps(
            1., 4., 5., 8., 1., 4., 5., 8., 2., 3., 6., 7., 2., 3., 6., 7.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_f32x4() {
        let a = _mm512_setr_ps(
            1., 4., 5., 8., 9., 12., 13., 16., 1., 4., 5., 8., 9., 12., 13., 16.,
        );
        let b = _mm512_setr_ps(
            2., 3., 6., 7., 10., 11., 14., 15., 2., 3., 6., 7., 10., 11., 14., 15.,
        );
        let r = _mm512_maskz_shuffle_f32x4(0, a, b, 0b00000000);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_shuffle_f32x4(0b00000000_11111111, a, b, 0b00000000);
        let e = _mm512_setr_ps(
            1., 4., 5., 8., 1., 4., 5., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_extractf32x4_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_extractf32x4_ps(a, 0x1);
        let e = _mm_setr_ps(5., 6., 7., 8.);
        assert_eq_m128(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_moveldup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_moveldup_ps(a);
        let e = _mm512_setr_ps(
            1., 1., 3., 3., 5., 5., 7., 7., 9., 9., 11., 11., 13., 13., 15., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_moveldup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_mask_moveldup_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_moveldup_ps(a, 0b11111111_11111111, a);
        let e = _mm512_setr_ps(
            1., 1., 3., 3., 5., 5., 7., 7., 9., 9., 11., 11., 13., 13., 15., 15.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_moveldup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_maskz_moveldup_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_moveldup_ps(0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            1., 1., 3., 3., 5., 5., 7., 7., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_movehdup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_movehdup_ps(a);
        let e = _mm512_setr_ps(
            2., 2., 4., 4., 6., 6., 8., 8., 10., 10., 12., 12., 14., 14., 16., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_movehdup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_mask_movehdup_ps(a, 0, a);
        assert_eq_m512(r, a);
        let r = _mm512_mask_movehdup_ps(a, 0b11111111_11111111, a);
        let e = _mm512_setr_ps(
            2., 2., 4., 4., 6., 6., 8., 8., 10., 10., 12., 12., 14., 14., 16., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_movehdup_ps() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let r = _mm512_maskz_movehdup_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_movehdup_ps(0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            2., 2., 4., 4., 6., 6., 8., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_inserti32x4() {
        let a = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm_setr_epi32(17, 18, 19, 20);
        let r = _mm512_inserti32x4(a, b, 0);
        let e = _mm512_setr_epi32(17, 18, 19, 20, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_inserti32x4() {
        let a = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm_setr_epi32(17, 18, 19, 20);
        let r = _mm512_mask_inserti32x4(a, 0, a, b, 0);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_inserti32x4(a, 0b11111111_11111111, a, b, 0);
        let e = _mm512_setr_epi32(17, 18, 19, 20, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_inserti32x4() {
        let a = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm_setr_epi32(17, 18, 19, 20);
        let r = _mm512_maskz_inserti32x4(0, a, b, 0);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_inserti32x4(0b00000000_11111111, a, b, 0);
        let e = _mm512_setr_epi32(17, 18, 19, 20, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_insertf32x4() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm_setr_ps(17., 18., 19., 20.);
        let r = _mm512_insertf32x4(a, b, 0);
        let e = _mm512_setr_ps(
            17., 18., 19., 20., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_insertf32x4() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm_setr_ps(17., 18., 19., 20.);
        let r = _mm512_mask_insertf32x4(a, 0, a, b, 0);
        assert_eq_m512(r, a);
        let r = _mm512_mask_insertf32x4(a, 0b11111111_11111111, a, b, 0);
        let e = _mm512_setr_ps(
            17., 18., 19., 20., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_insertf32x4() {
        let a = _mm512_setr_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm_setr_ps(17., 18., 19., 20.);
        let r = _mm512_maskz_insertf32x4(0, a, b, 0);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_insertf32x4(0b00000000_11111111, a, b, 0);
        let e = _mm512_setr_ps(
            17., 18., 19., 20., 5., 6., 7., 8., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps128_ps512() {
        let a = _mm_setr_ps(17., 18., 19., 20.);
        let r = _mm512_castps128_ps512(a);
        let e = _mm512_setr_ps(
            17., 18., 19., 20., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps256_ps512() {
        let a = _mm256_setr_ps(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_castps256_ps512(a);
        let e = _mm512_setr_ps(
            17., 18., 19., 20., 21., 22., 23., 24., -1., -1., -1., -1., -1., -1., -1., -1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps512_ps128() {
        let a = _mm512_setr_ps(
            17., 18., 19., 20., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1., -1.,
        );
        let r = _mm512_castps512_ps128(a);
        let e = _mm_setr_ps(17., 18., 19., 20.);
        assert_eq_m128(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps512_ps256() {
        let a = _mm512_setr_ps(
            17., 18., 19., 20., 21., 22., 23., 24., -1., -1., -1., -1., -1., -1., -1., -1.,
        );
        let r = _mm512_castps512_ps256(a);
        let e = _mm256_setr_ps(17., 18., 19., 20., 21., 22., 23., 24.);
        assert_eq_m256(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps_pd() {
        let a = _mm512_set1_ps(1.);
        let r = _mm512_castps_pd(a);
        let e = _mm512_set1_pd(0.007812501848093234);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castps_si512() {
        let a = _mm512_set1_ps(1.);
        let r = _mm512_castps_si512(a);
        let e = _mm512_set1_epi32(1065353216);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcastd_epi32() {
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_broadcastd_epi32(a);
        let e = _mm512_set1_epi32(20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcastd_epi32() {
        let src = _mm512_set1_epi32(20);
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_mask_broadcastd_epi32(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_broadcastd_epi32(src, 0b11111111_11111111, a);
        let e = _mm512_set1_epi32(20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcastd_epi32() {
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_maskz_broadcastd_epi32(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_broadcastd_epi32(0b00000000_11111111, a);
        let e = _mm512_setr_epi32(20, 20, 20, 20, 20, 20, 20, 20, 0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcastss_ps() {
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_broadcastss_ps(a);
        let e = _mm512_set1_ps(20.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcastss_ps() {
        let src = _mm512_set1_ps(20.);
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_mask_broadcastss_ps(src, 0, a);
        assert_eq_m512(r, src);
        let r = _mm512_mask_broadcastss_ps(src, 0b11111111_11111111, a);
        let e = _mm512_set1_ps(20.);
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcastss_ps() {
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_maskz_broadcastss_ps(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_broadcastss_ps(0b00000000_11111111, a);
        let e = _mm512_setr_ps(
            20., 20., 20., 20., 20., 20., 20., 20., 0., 0., 0., 0., 0., 0., 0., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcast_i32x4() {
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_broadcast_i32x4(a);
        let e = _mm512_set_epi32(
            17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcast_i32x4() {
        let src = _mm512_set1_epi32(20);
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_mask_broadcast_i32x4(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_broadcast_i32x4(src, 0b11111111_11111111, a);
        let e = _mm512_set_epi32(
            17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcast_i32x4() {
        let a = _mm_set_epi32(17, 18, 19, 20);
        let r = _mm512_maskz_broadcast_i32x4(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_broadcast_i32x4(0b00000000_11111111, a);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 17, 18, 19, 20, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcast_f32x4() {
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_broadcast_f32x4(a);
        let e = _mm512_set_ps(
            17., 18., 19., 20., 17., 18., 19., 20., 17., 18., 19., 20., 17., 18., 19., 20.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcast_f32x4() {
        let src = _mm512_set1_ps(20.);
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_mask_broadcast_f32x4(src, 0, a);
        assert_eq_m512(r, src);
        let r = _mm512_mask_broadcast_f32x4(src, 0b11111111_11111111, a);
        let e = _mm512_set_ps(
            17., 18., 19., 20., 17., 18., 19., 20., 17., 18., 19., 20., 17., 18., 19., 20.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcast_f32x4() {
        let a = _mm_set_ps(17., 18., 19., 20.);
        let r = _mm512_maskz_broadcast_f32x4(0, a);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_broadcast_f32x4(0b00000000_11111111, a);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 17., 18., 19., 20., 17., 18., 19., 20.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_blend_epi32() {
        let a = _mm512_set1_epi32(1);
        let b = _mm512_set1_epi32(2);
        let r = _mm512_mask_blend_epi32(0b11111111_00000000, a, b);
        let e = _mm512_set_epi32(2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_blend_ps() {
        let a = _mm512_set1_ps(1.);
        let b = _mm512_set1_ps(2.);
        let r = _mm512_mask_blend_ps(0b11111111_00000000, a, b);
        let e = _mm512_set_ps(
            2., 2., 2., 2., 2., 2., 2., 2., 1., 1., 1., 1., 1., 1., 1., 1.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpackhi_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_unpackhi_epi32(a, b);
        let e = _mm512_set_epi32(17, 1, 18, 2, 21, 5, 22, 6, 25, 9, 26, 10, 29, 13, 30, 14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpackhi_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_mask_unpackhi_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_unpackhi_epi32(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_epi32(17, 1, 18, 2, 21, 5, 22, 6, 25, 9, 26, 10, 29, 13, 30, 14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpackhi_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_maskz_unpackhi_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_unpackhi_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 25, 9, 26, 10, 29, 13, 30, 14);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpackhi_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_unpackhi_ps(a, b);
        let e = _mm512_set_ps(
            17., 1., 18., 2., 21., 5., 22., 6., 25., 9., 26., 10., 29., 13., 30., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpackhi_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_mask_unpackhi_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_unpackhi_ps(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_ps(
            17., 1., 18., 2., 21., 5., 22., 6., 25., 9., 26., 10., 29., 13., 30., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpackhi_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_maskz_unpackhi_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_unpackhi_ps(0b00000000_11111111, a, b);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 25., 9., 26., 10., 29., 13., 30., 14.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpacklo_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_unpacklo_epi32(a, b);
        let e = _mm512_set_epi32(19, 3, 20, 4, 23, 7, 24, 8, 27, 11, 28, 12, 31, 15, 32, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpacklo_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_mask_unpacklo_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_unpacklo_epi32(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_epi32(19, 3, 20, 4, 23, 7, 24, 8, 27, 11, 28, 12, 31, 15, 32, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpacklo_epi32() {
        let a = _mm512_set_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let b = _mm512_set_epi32(
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        );
        let r = _mm512_maskz_unpacklo_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_unpacklo_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 27, 11, 28, 12, 31, 15, 32, 16);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpacklo_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_unpacklo_ps(a, b);
        let e = _mm512_set_ps(
            19., 3., 20., 4., 23., 7., 24., 8., 27., 11., 28., 12., 31., 15., 32., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpacklo_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_mask_unpacklo_ps(a, 0, a, b);
        assert_eq_m512(r, a);
        let r = _mm512_mask_unpacklo_ps(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_ps(
            19., 3., 20., 4., 23., 7., 24., 8., 27., 11., 28., 12., 31., 15., 32., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpacklo_ps() {
        let a = _mm512_set_ps(
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        );
        let b = _mm512_set_ps(
            17., 18., 19., 20., 21., 22., 23., 24., 25., 26., 27., 28., 29., 30., 31., 32.,
        );
        let r = _mm512_maskz_unpacklo_ps(0, a, b);
        assert_eq_m512(r, _mm512_setzero_ps());
        let r = _mm512_maskz_unpacklo_ps(0b00000000_11111111, a, b);
        let e = _mm512_set_ps(
            0., 0., 0., 0., 0., 0., 0., 0., 27., 11., 28., 12., 31., 15., 32., 16.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_and_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_and_epi32(a, b);
        let e = _mm512_set_epi32(1 << 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_and_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_mask_and_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_and_epi32(a, 0b01111111_11111111, a, b);
        let e = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_and_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_maskz_and_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_and_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_and_si512() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_and_epi32(a, b);
        let e = _mm512_set_epi32(1 << 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_or_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_or_epi32(a, b);
        let e = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_or_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_mask_or_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_or_epi32(a, 0b11111111_11111111, a, b);
        let e = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_or_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_maskz_or_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_or_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_or_si512() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_or_epi32(a, b);
        let e = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_xor_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_xor_epi32(a, b);
        let e = _mm512_set_epi32(
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_xor_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_mask_xor_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_xor_epi32(a, 0b01111111_11111111, a, b);
        let e = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_xor_epi32() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_maskz_xor_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_xor_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_xor_si512() {
        let a = _mm512_set_epi32(
            1 << 1 | 1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 3,
        );
        let b = _mm512_set_epi32(
            1 << 1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
        );
        let r = _mm512_xor_epi32(a, b);
        let e = _mm512_set_epi32(
            1 << 2,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_andnot_epi32() {
        let a = _mm512_set1_epi32(0);
        let b = _mm512_set1_epi32(1 << 3 | 1 << 4);
        let r = _mm512_andnot_epi32(a, b);
        let e = _mm512_set1_epi32(1 << 3 | 1 << 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_andnot_epi32() {
        let a = _mm512_set1_epi32(1 << 1 | 1 << 2);
        let b = _mm512_set1_epi32(1 << 3 | 1 << 4);
        let r = _mm512_mask_andnot_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_andnot_epi32(a, 0b11111111_11111111, a, b);
        let e = _mm512_set1_epi32(1 << 3 | 1 << 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_andnot_epi32() {
        let a = _mm512_set1_epi32(1 << 1 | 1 << 2);
        let b = _mm512_set1_epi32(1 << 3 | 1 << 4);
        let r = _mm512_maskz_andnot_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_andnot_epi32(0b00000000_11111111, a, b);
        let e = _mm512_set_epi32(
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kand() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b11001100_00110011;
        let r = _mm512_kand(a, b);
        let e: u16 = 0b11001100_00110011;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_kand_mask16() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b11001100_00110011;
        let r = _kand_mask16(a, b);
        let e: u16 = 0b11001100_00110011;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kor() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _mm512_kor(a, b);
        let e: u16 = 0b11101110_00111011;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_kor_mask16() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _kor_mask16(a, b);
        let e: u16 = 0b11101110_00111011;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kxor() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _mm512_kxor(a, b);
        let e: u16 = 0b11100010_00111000;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_kxor_mask16() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _kxor_mask16(a, b);
        let e: u16 = 0b11100010_00111000;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_knot() {
        let a: u16 = 0b11001100_00110011;
        let r = _mm512_knot(a);
        let e: u16 = 0b00110011_11001100;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_knot_mask16() {
        let a: u16 = 0b11001100_00110011;
        let r = _knot_mask16(a);
        let e: u16 = 0b00110011_11001100;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kandn() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _mm512_kandn(a, b);
        let e: u16 = 0b00100010_00001000;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_kandn_mask16() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _kandn_mask16(a, b);
        let e: u16 = 0b00100010_00001000;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kxnor() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _mm512_kxnor(a, b);
        let e: u16 = 0b00011101_11000111;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_kxnor_mask16() {
        let a: u16 = 0b11001100_00110011;
        let b: u16 = 0b00101110_00001011;
        let r = _kxnor_mask16(a, b);
        let e: u16 = 0b00011101_11000111;
        assert_eq!(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_kmov() {
        let a: u16 = 0b11001100_00110011;
        let r = _mm512_kmov(a);
        let e: u16 = 0b11001100_00110011;
        assert_eq!(r, e);
    }
}

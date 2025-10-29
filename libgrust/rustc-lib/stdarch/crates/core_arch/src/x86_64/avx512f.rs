use crate::{
    core_arch::{simd::*, x86::*},
    mem::transmute,
};

/// Sets packed 64-bit integers in `dst` with the supplied values.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_set_epi64(
    e0: i64,
    e1: i64,
    e2: i64,
    e3: i64,
    e4: i64,
    e5: i64,
    e6: i64,
    e7: i64,
) -> __m512i {
    _mm512_setr_epi64(e7, e6, e5, e4, e3, e2, e1, e0)
}

/// Sets packed 64-bit integers in `dst` with the supplied values in
/// reverse order.
///
/// [Intel's documentation]( https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=727,1063,4909,1062,1062,4909&text=_mm512_set_epi64)
#[inline]
#[target_feature(enable = "avx512f")]
pub unsafe fn _mm512_setr_epi64(
    e0: i64,
    e1: i64,
    e2: i64,
    e3: i64,
    e4: i64,
    e5: i64,
    e6: i64,
    e7: i64,
) -> __m512i {
    let r = i64x8::new(e0, e1, e2, e3, e4, e5, e6, e7);
    transmute(r)
}

#[cfg(test)]
mod tests {
    use std;
    use stdarch_test::simd_test;

    use crate::core_arch::x86::*;
    use crate::core_arch::x86_64::*;

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_abs_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let r = _mm512_abs_epi64(a);
        let e = _mm512_setr_epi64(0, 1, 1, i64::MAX, i64::MAX.wrapping_add(1), 100, 100, 32);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_abs_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let r = _mm512_mask_abs_epi64(a, 0, a);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_abs_epi64(a, 0b00001111, a);
        let e = _mm512_setr_epi64(0, 1, 1, i64::MAX, i64::MIN, 100, -100, -32);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_abs_epi64() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let r = _mm512_maskz_abs_epi64(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_abs_epi64(0b00001111, a);
        let e = _mm512_setr_epi64(0, 1, 1, i64::MAX, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_abs_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let r = _mm512_abs_pd(a);
        let e = _mm512_setr_pd(0., 1., 1., f64::MAX, f64::MAX, 100., 100., 32.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_abs_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let r = _mm512_mask_abs_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_abs_pd(a, 0b00001111, a);
        let e = _mm512_setr_pd(0., 1., 1., f64::MAX, f64::MIN, 100., -100., -32.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_add_epi64(a, b);
        let e = _mm512_setr_epi64(1, 2, 0, i64::MIN, i64::MIN + 1, 101, -99, -31);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_mask_add_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_add_epi64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(1, 2, 0, i64::MIN, i64::MIN, 100, -100, -32);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_epi64() {
        #[rustfmt::skip]
        let a = _mm512_setr_epi64(
            0, 1, -1, i64::MAX,
            i64::MIN, 100, -100, -32
        );
        let b = _mm512_set1_epi64(1);
        let r = _mm512_maskz_add_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_add_epi64(0b00001111, a, b);
        let e = _mm512_setr_epi64(1, 2, 0, i64::MIN, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_add_pd(a, b);
        let e = _mm512_setr_pd(1., 2., 0., f64::MAX, f64::MIN + 1., 101., -99., -31.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_mask_add_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_add_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(1., 2., 0., f64::MAX, f64::MIN, 100., -100., -32.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_maskz_add_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_add_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(1., 2., 0., f64::MAX, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_sub_epi64(a, b);
        let e = _mm512_setr_epi64(-1, 0, -2, i64::MAX - 1, i64::MAX, 99, -101, -33);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_mask_sub_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_sub_epi64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(-1, 0, -2, i64::MAX - 1, i64::MIN, 100, -100, -32);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_epi64() {
        let a = _mm512_setr_epi64(0, 1, -1, i64::MAX, i64::MIN, 100, -100, -32);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_maskz_sub_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_sub_epi64(0b00001111, a, b);
        let e = _mm512_setr_epi64(-1, 0, -2, i64::MAX - 1, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_sub_pd(a, b);
        let e = _mm512_setr_pd(-1., 0., -2., f64::MAX - 1., f64::MIN, 99., -101., -33.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_mask_sub_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_sub_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(-1., 0., -2., f64::MAX - 1., f64::MIN, 100., -100., -32.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_pd() {
        let a = _mm512_setr_pd(0., 1., -1., f64::MAX, f64::MIN, 100., -100., -32.);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_maskz_sub_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_sub_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(-1., 0., -2., f64::MAX - 1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_epi32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_mul_epi32(a, b);
        let e = _mm512_set_epi64(15, 13, 11, 9, 7, 5, 3, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_epi32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_mask_mul_epi32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_mul_epi32(a, 0b00001111, a, b);
        let e = _mm512_set_epi64(
            1 | 1 << 32,
            1 | 1 << 32,
            1 | 1 << 32,
            1 | 1 << 32,
            7,
            5,
            3,
            1,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_epi32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_maskz_mul_epi32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_mul_epi32(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 7, 5, 3, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_epu32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_mul_epu32(a, b);
        let e = _mm512_set_epi64(15, 13, 11, 9, 7, 5, 3, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_epu32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_mask_mul_epu32(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_mul_epu32(a, 0b00001111, a, b);
        let e = _mm512_set_epi64(
            1 | 1 << 32,
            1 | 1 << 32,
            1 | 1 << 32,
            1 | 1 << 32,
            7,
            5,
            3,
            1,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_epu32() {
        let a = _mm512_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
        let b = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
        let r = _mm512_maskz_mul_epu32(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_mul_epu32(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 7, 5, 3, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mullox_epi64() {
        let a = _mm512_setr_epi64(0, 1, i64::MAX, i64::MIN, i64::MAX, 100, -100, -32);
        let b = _mm512_set1_epi64(2);
        let r = _mm512_mullox_epi64(a, b);
        let e = _mm512_setr_epi64(0, 2, -2, 0, -2, 200, -200, -64);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mullox_epi64() {
        let a = _mm512_setr_epi64(0, 1, i64::MAX, i64::MIN, i64::MAX, 100, -100, -32);
        let b = _mm512_set1_epi64(2);
        let r = _mm512_mask_mullox_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_mullox_epi64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(0, 2, -2, 0, i64::MAX, 100, -100, -32);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_set1_pd(2.);
        let r = _mm512_mul_pd(a, b);
        let e = _mm512_setr_pd(
            0.,
            2.,
            f64::INFINITY,
            f64::NEG_INFINITY,
            f64::INFINITY,
            f64::NEG_INFINITY,
            -200.,
            -64.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_set1_pd(2.);
        let r = _mm512_mask_mul_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_mul_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(
            0.,
            2.,
            f64::INFINITY,
            f64::NEG_INFINITY,
            f64::MAX,
            f64::MIN,
            -100.,
            -32.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_set1_pd(2.);
        let r = _mm512_maskz_mul_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_mul_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(0., 2., f64::INFINITY, f64::NEG_INFINITY, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_div_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_setr_pd(2., 2., 0., 0., 0., 0., 2., 2.);
        let r = _mm512_div_pd(a, b);
        let e = _mm512_setr_pd(
            0.,
            0.5,
            f64::INFINITY,
            f64::NEG_INFINITY,
            f64::INFINITY,
            f64::NEG_INFINITY,
            -50.,
            -16.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_div_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_setr_pd(2., 2., 0., 0., 0., 0., 2., 2.);
        let r = _mm512_mask_div_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_div_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(
            0.,
            0.5,
            f64::INFINITY,
            f64::NEG_INFINITY,
            f64::MAX,
            f64::MIN,
            -100.,
            -32.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_div_pd() {
        let a = _mm512_setr_pd(0., 1., f64::MAX, f64::MIN, f64::MAX, f64::MIN, -100., -32.);
        let b = _mm512_setr_pd(2., 2., 0., 0., 0., 0., 2., 2.);
        let r = _mm512_maskz_div_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_div_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(0., 0.5, f64::INFINITY, f64::NEG_INFINITY, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_max_epi64(a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_max_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_max_epi64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_max_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_max_epi64(0b00001111, a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_max_pd(a, b);
        let e = _mm512_setr_pd(7., 6., 5., 4., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_mask_max_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_max_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(7., 6., 5., 4., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_maskz_max_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_max_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(7., 6., 5., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_max_epu64(a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_max_epu64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_max_epu64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_max_epu64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_max_epu64(0b00001111, a, b);
        let e = _mm512_setr_epi64(7, 6, 5, 4, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_min_epi64(a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 3, 2, 1, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_min_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_min_epi64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_epi64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_min_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_min_epi64(0b00001111, a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_min_pd(a, b);
        let e = _mm512_setr_pd(0., 1., 2., 3., 3., 2., 1., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_mask_min_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_min_pd(a, 0b00001111, a, b);
        let e = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_maskz_min_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_min_pd(0b00001111, a, b);
        let e = _mm512_setr_pd(0., 1., 2., 3., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_min_epu64(a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 3, 2, 1, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_mask_min_epu64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_min_epu64(a, 0b00001111, a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_epu64() {
        let a = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let b = _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0);
        let r = _mm512_maskz_min_epu64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_min_epu64(0b00001111, a, b);
        let e = _mm512_setr_epi64(0, 1, 2, 3, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sqrt_pd() {
        let a = _mm512_setr_pd(0., 1., 4., 9., 16., 25., 36., 49.);
        let r = _mm512_sqrt_pd(a);
        let e = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sqrt_pd() {
        let a = _mm512_setr_pd(0., 1., 4., 9., 16., 25., 36., 49.);
        let r = _mm512_mask_sqrt_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_sqrt_pd(a, 0b00001111, a);
        let e = _mm512_setr_pd(0., 1., 2., 3., 16., 25., 36., 49.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sqrt_pd() {
        let a = _mm512_setr_pd(0., 1., 4., 9., 16., 25., 36., 49.);
        let r = _mm512_maskz_sqrt_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_sqrt_pd(0b00001111, a);
        let e = _mm512_setr_pd(0., 1., 2., 3., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fmadd_pd(a, b, c);
        let e = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fmadd_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmadd_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(1., 2., 3., 4., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fmadd_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmadd_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(1., 2., 3., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fmadd_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmadd_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(1., 2., 3., 4., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fmsub_pd(a, b, c);
        let e = _mm512_setr_pd(-1., 0., 1., 2., 3., 4., 5., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fmsub_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmsub_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(-1., 0., 1., 2., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fmsub_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmsub_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(-1., 0., 1., 2., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fmsub_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmsub_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(-1., 0., 1., 2., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmaddsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fmaddsub_pd(a, b, c);
        let e = _mm512_setr_pd(-1., 2., 1., 4., 3., 6., 5., 8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmaddsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fmaddsub_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmaddsub_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(-1., 2., 1., 4., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmaddsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fmaddsub_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmaddsub_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(-1., 2., 1., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmaddsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fmaddsub_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmaddsub_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(-1., 2., 1., 4., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsubadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fmsubadd_pd(a, b, c);
        let e = _mm512_setr_pd(1., 0., 3., 2., 5., 4., 7., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsubadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fmsubadd_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmsubadd_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(1., 0., 3., 2., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsubadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fmsubadd_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmsubadd_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(1., 0., 3., 2., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsubadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fmsubadd_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmsubadd_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(1., 0., 3., 2., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fnmadd_pd(a, b, c);
        let e = _mm512_setr_pd(1., 0., -1., -2., -3., -4., -5., -6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fnmadd_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fnmadd_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(1., 0., -1., -2., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fnmadd_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fnmadd_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(1., 0., -1., -2., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmadd_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fnmadd_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fnmadd_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(1., 0., -1., -2., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_fnmsub_pd(a, b, c);
        let e = _mm512_setr_pd(-1., -2., -3., -4., -5., -6., -7., -8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_mask_fnmsub_pd(a, 0, b, c);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fnmsub_pd(a, 0b00001111, b, c);
        let e = _mm512_setr_pd(-1., -2., -3., -4., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let r = _mm512_maskz_fnmsub_pd(0, a, b, c);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fnmsub_pd(0b00001111, a, b, c);
        let e = _mm512_setr_pd(-1., -2., -3., -4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmsub_pd() {
        let a = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        let b = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let c = _mm512_setr_pd(1., 1., 1., 1., 2., 2., 2., 2.);
        let r = _mm512_mask3_fnmsub_pd(a, b, c, 0);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fnmsub_pd(a, b, c, 0b00001111);
        let e = _mm512_setr_pd(-1., -2., -3., -4., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rcp14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_rcp14_pd(a);
        let e = _mm512_set1_pd(0.3333320617675781);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rcp14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_mask_rcp14_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_rcp14_pd(a, 0b11110000, a);
        let e = _mm512_setr_pd(
            3.,
            3.,
            3.,
            3.,
            0.3333320617675781,
            0.3333320617675781,
            0.3333320617675781,
            0.3333320617675781,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rcp14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_maskz_rcp14_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_rcp14_pd(0b11110000, a);
        let e = _mm512_setr_pd(
            0.,
            0.,
            0.,
            0.,
            0.3333320617675781,
            0.3333320617675781,
            0.3333320617675781,
            0.3333320617675781,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rsqrt14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_rsqrt14_pd(a);
        let e = _mm512_set1_pd(0.5773391723632813);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rsqrt14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_mask_rsqrt14_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_rsqrt14_pd(a, 0b11110000, a);
        let e = _mm512_setr_pd(
            3.,
            3.,
            3.,
            3.,
            0.5773391723632813,
            0.5773391723632813,
            0.5773391723632813,
            0.5773391723632813,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rsqrt14_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_maskz_rsqrt14_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_rsqrt14_pd(0b11110000, a);
        let e = _mm512_setr_pd(
            0.,
            0.,
            0.,
            0.,
            0.5773391723632813,
            0.5773391723632813,
            0.5773391723632813,
            0.5773391723632813,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getexp_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_getexp_pd(a);
        let e = _mm512_set1_pd(1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getexp_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_mask_getexp_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_getexp_pd(a, 0b11110000, a);
        let e = _mm512_setr_pd(3., 3., 3., 3., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getexp_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_maskz_getexp_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_getexp_pd(0b11110000, a);
        let e = _mm512_setr_pd(0., 0., 0., 0., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getmant_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_getmant_pd(a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        let e = _mm512_set1_pd(1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getmant_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_mask_getmant_pd(a, 0, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_getmant_pd(a, 0b11110000, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        let e = _mm512_setr_pd(10., 10., 10., 10., 1.25, 1.25, 1.25, 1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getmant_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_maskz_getmant_pd(0, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_getmant_pd(0b11110000, a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_SRC);
        let e = _mm512_setr_pd(0., 0., 0., 0., 1.25, 1.25, 1.25, 1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvtps_pd(a);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm512_set1_pd(0.);
        let r = _mm512_mask_cvtps_pd(src, 0, a);
        assert_eq_m512d(r, src);
        let r = _mm512_mask_cvtps_pd(src, 0b00001111, a);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvtps_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_cvtps_pd(0b00001111, a);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtt_roundpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvtt_roundpd_epi32(a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtt_roundpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm256_set1_epi32(0);
        let r = _mm512_mask_cvtt_roundpd_epi32(src, 0, a, _MM_FROUND_NO_EXC);
        assert_eq_m256i(r, src);
        let r = _mm512_mask_cvtt_roundpd_epi32(src, 0b00001111, a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtt_roundpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvtt_roundpd_epi32(0, a, _MM_FROUND_NO_EXC);
        assert_eq_m256i(r, _mm256_setzero_si256());
        let r = _mm512_maskz_cvtt_roundpd_epi32(0b00001111, a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvtt_roundpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvtt_roundpd_epu32(a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvtt_roundpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm256_set1_epi32(0);
        let r = _mm512_mask_cvtt_roundpd_epu32(src, 0, a, _MM_FROUND_NO_EXC);
        assert_eq_m256i(r, src);
        let r = _mm512_mask_cvtt_roundpd_epu32(src, 0b00001111, a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvtt_roundpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvtt_roundpd_epu32(0, a, _MM_FROUND_NO_EXC);
        assert_eq_m256i(r, _mm256_setzero_si256());
        let r = _mm512_maskz_cvtt_roundpd_epu32(0b00001111, a, _MM_FROUND_NO_EXC);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvttpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvttpd_epi32(a);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 4, -5, 6, -7);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvttpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm256_set1_epi32(0);
        let r = _mm512_mask_cvttpd_epi32(src, 0, a);
        assert_eq_m256i(r, src);
        let r = _mm512_mask_cvttpd_epi32(src, 0b00001111, a);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvttpd_epi32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvttpd_epi32(0, a);
        assert_eq_m256i(r, _mm256_setzero_si256());
        let r = _mm512_maskz_cvttpd_epi32(0b00001111, a);
        let e = _mm256_setr_epi32(0, -1, 2, -3, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvttpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvttpd_epu32(a);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 4, -1, 6, -1);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvttpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm256_set1_epi32(0);
        let r = _mm512_mask_cvttpd_epu32(src, 0, a);
        assert_eq_m256i(r, src);
        let r = _mm512_mask_cvttpd_epu32(src, 0b00001111, a);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvttpd_epu32() {
        let a = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvttpd_epu32(0, a);
        assert_eq_m256i(r, _mm256_setzero_si256());
        let r = _mm512_maskz_cvttpd_epu32(0b00001111, a);
        let e = _mm256_setr_epi32(0, -1, 2, -1, 0, 0, 0, 0);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_add_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(-1.);
        let r = _mm512_add_round_pd(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(7., 8.5, 9., 10.5, 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
        let r = _mm512_add_round_pd(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(7., 8.5, 9., 10.5, 11., 12.5, 13., -0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_add_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(-1.);
        let r = _mm512_mask_add_round_pd(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_add_round_pd(
            a,
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(8., 9.5, 10., 11.5, 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_add_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(-1.);
        let r = _mm512_maskz_add_round_pd(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_add_round_pd(
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(0., 0., 0., 0., 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sub_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_sub_round_pd(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(7., 8.5, 9., 10.5, 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
        let r = _mm512_sub_round_pd(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(7., 8.5, 9., 10.5, 11., 12.5, 13., -0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sub_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_mask_sub_round_pd(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_sub_round_pd(
            a,
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(8., 9.5, 10., 11.5, 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sub_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let r = _mm512_maskz_sub_round_pd(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_sub_round_pd(
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(0., 0., 0., 0., 11., 12.5, 13., -1.0);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mul_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.);
        let b = _mm512_set1_pd(0.1);
        let r = _mm512_mul_round_pd(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(
            0.8,
            0.9500000000000001,
            1.,
            1.1500000000000001,
            1.2000000000000002,
            1.35,
            1.4000000000000001,
            0.,
        );
        assert_eq_m512d(r, e);
        let r = _mm512_mul_round_pd(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(0.8, 0.95, 1.0, 1.15, 1.2, 1.3499999999999999, 1.4, 0.0);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_mul_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.);
        let b = _mm512_set1_pd(0.1);
        let r = _mm512_mask_mul_round_pd(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_mul_round_pd(
            a,
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            8.,
            9.5,
            10.,
            11.5,
            1.2000000000000002,
            1.35,
            1.4000000000000001,
            0.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_mul_round_pd() {
        let a = _mm512_setr_pd(8., 9.5, 10., 11.5, 12., 13.5, 14., 0.);
        let b = _mm512_set1_pd(0.1);
        let r = _mm512_maskz_mul_round_pd(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_mul_round_pd(
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            0.,
            0.,
            0.,
            0.,
            1.2000000000000002,
            1.35,
            1.4000000000000001,
            0.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_div_round_pd() {
        let a = _mm512_set1_pd(1.);
        let b = _mm512_set1_pd(3.);
        let r = _mm512_div_round_pd(a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(0.3333333333333333);
        assert_eq_m512d(r, e);
        let r = _mm512_div_round_pd(a, b, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(0.33333333333333334);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_div_round_pd() {
        let a = _mm512_set1_pd(1.);
        let b = _mm512_set1_pd(3.);
        let r = _mm512_mask_div_round_pd(a, 0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_div_round_pd(
            a,
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            1.,
            1.,
            1.,
            1.,
            0.3333333333333333,
            0.3333333333333333,
            0.3333333333333333,
            0.3333333333333333,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_div_round_pd() {
        let a = _mm512_set1_pd(1.);
        let b = _mm512_set1_pd(3.);
        let r = _mm512_maskz_div_round_pd(0, a, b, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_div_round_pd(
            0b11110000,
            a,
            b,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            0.,
            0.,
            0.,
            0.,
            0.3333333333333333,
            0.3333333333333333,
            0.3333333333333333,
            0.3333333333333333,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sqrt_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_sqrt_round_pd(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(1.7320508075688772);
        assert_eq_m512d(r, e);
        let r = _mm512_sqrt_round_pd(a, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(1.7320508075688774);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sqrt_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_mask_sqrt_round_pd(a, 0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_sqrt_round_pd(
            a,
            0b11110000,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            3.,
            3.,
            3.,
            3.,
            1.7320508075688772,
            1.7320508075688772,
            1.7320508075688772,
            1.7320508075688772,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sqrt_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_maskz_sqrt_round_pd(0, a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_sqrt_round_pd(
            0b11110000,
            a,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            0.,
            0.,
            0.,
            0.,
            1.7320508075688772,
            1.7320508075688772,
            1.7320508075688772,
            1.7320508075688772,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_fmadd_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(-1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fmadd_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(-0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_mask_fmadd_round_pd(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmadd_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            -1.,
            -1.,
            -1.,
            -1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_maskz_fmadd_round_pd(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmadd_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., -1., -1., -1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_mask3_fmadd_round_pd(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmadd_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., -1., -1., -1., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r = _mm512_fmsub_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(-1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fmsub_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(-0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_mask_fmsub_round_pd(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmsub_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            -1.,
            -1.,
            -1.,
            -1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_maskz_fmsub_round_pd(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmsub_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., -1., -1., -1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_mask3_fmsub_round_pd(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmsub_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., -1., -1., -1., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmaddsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_fmaddsub_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(1., -1., 1., -1., 1., -1., 1., -1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fmaddsub_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(
            1.,
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmaddsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_mask_fmaddsub_round_pd(
            a,
            0,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmaddsub_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            1.,
            -1.,
            1.,
            -1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmaddsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_maskz_fmaddsub_round_pd(
            0,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmaddsub_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., -1., 1., -1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmaddsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_mask3_fmaddsub_round_pd(
            a,
            b,
            c,
            0,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmaddsub_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., -1., 1., -1., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fmsubadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_fmsubadd_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(-1., 1., -1., 1., -1., 1., -1., 1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fmsubadd_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_setr_pd(
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
            1.,
            -0.9999999999999999,
            1.,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fmsubadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_mask_fmsubadd_round_pd(
            a,
            0,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fmsubadd_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            -1.,
            1.,
            -1.,
            1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fmsubadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_maskz_fmsubadd_round_pd(
            0,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fmsubadd_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., 1., -1., 1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fmsubadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_mask3_fmsubadd_round_pd(
            a,
            b,
            c,
            0,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fmsubadd_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(-1., 1., -1., 1., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r = _mm512_fnmadd_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fnmadd_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_mask_fnmadd_round_pd(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fnmadd_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            1.,
            1.,
            1.,
            1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_maskz_fnmadd_round_pd(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fnmadd_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., 1., 1., 1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmadd_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(1.);
        let r =
            _mm512_mask3_fnmadd_round_pd(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fnmadd_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., 1., 1., 1., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_fnmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r = _mm512_fnmsub_round_pd(a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(1.);
        assert_eq_m512d(r, e);
        let r = _mm512_fnmsub_round_pd(a, b, c, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        let e = _mm512_set1_pd(0.9999999999999999);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_fnmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_mask_fnmsub_round_pd(a, 0, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_fnmsub_round_pd(
            a,
            0b00001111,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(
            1.,
            1.,
            1.,
            1.,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
            0.000000000000000007,
        );
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_fnmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_maskz_fnmsub_round_pd(0, a, b, c, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_fnmsub_round_pd(
            0b00001111,
            a,
            b,
            c,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., 1., 1., 1., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask3_fnmsub_round_pd() {
        let a = _mm512_set1_pd(0.000000000000000007);
        let b = _mm512_set1_pd(1.);
        let c = _mm512_set1_pd(-1.);
        let r =
            _mm512_mask3_fnmsub_round_pd(a, b, c, 0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        assert_eq_m512d(r, c);
        let r = _mm512_mask3_fnmsub_round_pd(
            a,
            b,
            c,
            0b00001111,
            _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC,
        );
        let e = _mm512_setr_pd(1., 1., 1., 1., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_max_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_max_round_pd(a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(7., 6., 5., 4., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_max_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_mask_max_round_pd(a, 0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_max_round_pd(a, 0b00001111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(7., 6., 5., 4., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_max_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_maskz_max_round_pd(0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_max_round_pd(0b00001111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(7., 6., 5., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_min_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_min_round_pd(a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., 1., 2., 3., 3., 2., 1., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_min_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_mask_min_round_pd(a, 0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_min_round_pd(a, 0b00001111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_min_round_pd() {
        let a = _mm512_setr_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_setr_pd(7., 6., 5., 4., 3., 2., 1., 0.);
        let r = _mm512_maskz_min_round_pd(0, a, b, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_min_round_pd(0b00001111, a, b, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., 1., 2., 3., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getexp_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_getexp_round_pd(a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_set1_pd(1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getexp_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_mask_getexp_round_pd(a, 0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_getexp_round_pd(a, 0b11110000, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(3., 3., 3., 3., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getexp_round_pd() {
        let a = _mm512_set1_pd(3.);
        let r = _mm512_maskz_getexp_round_pd(0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_getexp_round_pd(0b11110000, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., 0., 0., 0., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_getmant_round_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_getmant_round_pd(
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_set1_pd(1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_getmant_round_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_mask_getmant_round_pd(
            a,
            0,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        assert_eq_m512d(r, a);
        let r = _mm512_mask_getmant_round_pd(
            a,
            0b11110000,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_setr_pd(10., 10., 10., 10., 1.25, 1.25, 1.25, 1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_getmant_round_pd() {
        let a = _mm512_set1_pd(10.);
        let r = _mm512_maskz_getmant_round_pd(
            0,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_getmant_round_pd(
            0b11110000,
            a,
            _MM_MANT_NORM_1_2,
            _MM_MANT_SIGN_SRC,
            _MM_FROUND_CUR_DIRECTION,
        );
        let e = _mm512_setr_pd(0., 0., 0., 0., 1.25, 1.25, 1.25, 1.25);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cvt_roundps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_cvt_roundps_pd(a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cvt_roundps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let src = _mm512_set1_pd(0.);
        let r = _mm512_mask_cvt_roundps_pd(src, 0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, src);
        let r = _mm512_mask_cvt_roundps_pd(src, 0b00001111, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_cvt_roundps_pd() {
        let a = _mm256_setr_ps(0., -1.5, 2., -3.5, 4., -5.5, 6., -7.5);
        let r = _mm512_maskz_cvt_roundps_pd(0, a, _MM_FROUND_CUR_DIRECTION);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_cvt_roundps_pd(0b00001111, a, _MM_FROUND_CUR_DIRECTION);
        let e = _mm512_setr_pd(0., -1.5, 2., -3.5, 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setzero_pd() {
        assert_eq_m512d(_mm512_setzero_pd(), _mm512_set1_pd(0.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set1_pd() {
        let expected = _mm512_set_pd(2., 2., 2., 2., 2., 2., 2., 2.);
        assert_eq_m512d(expected, _mm512_set1_pd(2.));
    }

    unsafe fn test_mm512_set1_epi64() {
        let r = _mm512_set_epi64(2, 2, 2, 2, 2, 2, 2, 2);
        assert_eq_m512i(r, _mm512_set1_epi64(2));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let m = _mm512_cmplt_pd_mask(a, b);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01100110;
        let r = _mm512_mask_cmplt_pd_mask(mask, a, b);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpnlt_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        assert_eq!(_mm512_cmpnlt_pd_mask(a, b), !_mm512_cmplt_pd_mask(a, b));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpnlt_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmpnlt_pd_mask(mask, a, b), 0b01111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        assert_eq!(_mm512_cmple_pd_mask(a, b), 0b00100101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmple_pd_mask(mask, a, b), 0b00100000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpnle_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let m = _mm512_cmpnle_pd_mask(b, a);
        assert_eq!(m, 0b00001101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpnle_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., f64::MAX, f64::NAN, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01100110;
        let r = _mm512_mask_cmpnle_pd_mask(mask, b, a);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, f64::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(0., 1., 13., 42., f64::MAX, f64::MIN, f64::NAN, -100.);
        let m = _mm512_cmpeq_pd_mask(b, a);
        assert_eq!(m, 0b11001101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, f64::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(0., 1., 13., 42., f64::MAX, f64::MIN, f64::NAN, -100.);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpeq_pd_mask(mask, b, a);
        assert_eq!(r, 0b01001000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpneq_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, f64::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(0., 1., 13., 42., f64::MAX, f64::MIN, f64::NAN, -100.);
        let m = _mm512_cmpneq_pd_mask(b, a);
        assert_eq!(m, 0b00110010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, f64::NAN, -100.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(0., 1., 13., 42., f64::MAX, f64::MIN, f64::NAN, -100.);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpneq_pd_mask(mask, b, a);
        assert_eq!(r, 0b00110010)
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let m = _mm512_cmp_pd_mask(a, b, _CMP_LT_OQ);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01100110;
        let r = _mm512_mask_cmp_pd_mask(mask, a, b, _CMP_LT_OQ);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_round_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let m = _mm512_cmp_round_pd_mask(a, b, _CMP_LT_OQ, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_round_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(0., 1., -1., 13., f64::MAX, f64::MIN, 100., -100.);
        let b = _mm512_set1_pd(-1.);
        let mask = 0b01100110;
        let r = _mm512_mask_cmp_round_pd_mask(mask, a, b, _CMP_LT_OQ, _MM_FROUND_CUR_DIRECTION);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpord_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(f64::NAN, f64::MAX, f64::NAN, f64::MIN, f64::NAN, -1., f64::NAN, 0.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(f64::NAN, f64::NAN, f64::NAN, f64::NAN, f64::MIN, f64::MAX, -1., 0.);
        let m = _mm512_cmpord_pd_mask(a, b);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpord_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(f64::NAN, f64::MAX, f64::NAN, f64::MIN, f64::NAN, -1., f64::NAN, 0.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(f64::NAN, f64::NAN, f64::NAN, f64::NAN, f64::MIN, f64::MAX, -1., 0.);
        let mask = 0b11000011;
        let m = _mm512_mask_cmpord_pd_mask(mask, a, b);
        assert_eq!(m, 0b00000001);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpunord_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(f64::NAN, f64::MAX, f64::NAN, f64::MIN, f64::NAN, -1., f64::NAN, 0.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(f64::NAN, f64::NAN, f64::NAN, f64::NAN, f64::MIN, f64::MAX, -1., 0.);
        let m = _mm512_cmpunord_pd_mask(a, b);

        assert_eq!(m, 0b11111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpunord_pd_mask() {
        #[rustfmt::skip]
        let a = _mm512_set_pd(f64::NAN, f64::MAX, f64::NAN, f64::MIN, f64::NAN, -1., f64::NAN, 0.);
        #[rustfmt::skip]
        let b = _mm512_set_pd(f64::NAN, f64::NAN, f64::NAN, f64::NAN, f64::MIN, f64::MAX, -1., 0.);
        let mask = 0b00001111;
        let m = _mm512_mask_cmpunord_pd_mask(mask, a, b);
        assert_eq!(m, 0b000001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmplt_epu64_mask(a, b);
        assert_eq!(m, 0b11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        let r = _mm512_mask_cmplt_epu64_mask(mask, a, b);
        assert_eq!(r, 0b01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpgt_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmpgt_epu64_mask(b, a);
        assert_eq!(m, 0b11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpgt_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpgt_epu64_mask(mask, b, a);
        assert_eq!(r, 0b01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        assert_eq!(
            _mm512_cmple_epu64_mask(a, b),
            !_mm512_cmpgt_epu64_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmple_epu64_mask(mask, a, b), 0b01111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpge_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        assert_eq!(
            _mm512_cmpge_epu64_mask(a, b),
            !_mm512_cmplt_epu64_mask(a, b)
        );
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpge_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmpge_epu64_mask(mask, a, b), 0b01111010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let m = _mm512_cmpeq_epu64_mask(b, a);
        assert_eq!(m, 0b11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpeq_epu64_mask(mask, b, a);
        assert_eq!(r, 0b01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpneq_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let m = _mm512_cmpneq_epu64_mask(b, a);
        assert_eq!(m, !_mm512_cmpeq_epu64_mask(b, a));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, -100, 100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpneq_epu64_mask(mask, b, a);
        assert_eq!(r, 0b00110010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmp_epu64_mask(a, b, _MM_CMPINT_LT);
        assert_eq!(m, 0b11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_epu64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        let r = _mm512_mask_cmp_epu64_mask(mask, a, b, _MM_CMPINT_LT);
        assert_eq!(r, 0b01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmplt_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmplt_epi64_mask(a, b);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmplt_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01100110;
        let r = _mm512_mask_cmplt_epi64_mask(mask, a, b);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpgt_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmpgt_epi64_mask(b, a);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpgt_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01100110;
        let r = _mm512_mask_cmpgt_epi64_mask(mask, b, a);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmple_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        assert_eq!(
            _mm512_cmple_epi64_mask(a, b),
            !_mm512_cmpgt_epi64_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmple_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmple_epi64_mask(mask, a, b), 0b00110000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpge_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        assert_eq!(
            _mm512_cmpge_epi64_mask(a, b),
            !_mm512_cmplt_epi64_mask(a, b)
        )
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpge_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, u64::MAX as i64, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01111010;
        assert_eq!(_mm512_mask_cmpge_epi64_mask(mask, a, b), 0b0110000);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmpeq_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let m = _mm512_cmpeq_epi64_mask(b, a);
        assert_eq!(m, 0b11001111);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpeq_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpeq_epi64_mask(mask, b, a);
        assert_eq!(r, 0b01001010);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_set_epi64() {
        let r = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        assert_eq_m512i(r, _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0))
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_setr_epi64() {
        let r = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        assert_eq_m512i(r, _mm512_setr_epi64(7, 6, 5, 4, 3, 2, 1, 0))
    }

    unsafe fn test_mm512_cmpneq_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let m = _mm512_cmpneq_epi64_mask(b, a);
        assert_eq!(m, !_mm512_cmpeq_epi64_mask(b, a));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmpneq_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, -100, 100);
        let b = _mm512_set_epi64(0, 1, 13, 42, i64::MAX, i64::MIN, 100, -100);
        let mask = 0b01111010;
        let r = _mm512_mask_cmpneq_epi64_mask(mask, b, a);
        assert_eq!(r, 0b00110010)
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_cmp_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let m = _mm512_cmp_epi64_mask(a, b, _MM_CMPINT_LT);
        assert_eq!(m, 0b00000101);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_cmp_epi64_mask() {
        let a = _mm512_set_epi64(0, 1, -1, 13, i64::MAX, i64::MIN, 100, -100);
        let b = _mm512_set1_epi64(-1);
        let mask = 0b01100110;
        let r = _mm512_mask_cmp_epi64_mask(mask, a, b, _MM_CMPINT_LT);
        assert_eq!(r, 0b00000100);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32gather_pd() {
        let mut arr = [0f64; 128];
        for i in 0..128 {
            arr[i] = i as f64;
        }
        // A multiplier of 8 is word-addressing
        #[rustfmt::skip]
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i32gather_pd(index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512d(r, _mm512_setr_pd(0., 16., 32., 48., 64., 80., 96., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32gather_pd() {
        let mut arr = [0f64; 128];
        for i in 0..128 {
            arr[i] = i as f64;
        }
        let src = _mm512_set1_pd(2.);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 8 is word-addressing
        let r = _mm512_mask_i32gather_pd(src, mask, index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512d(r, _mm512_setr_pd(2., 16., 2., 48., 2., 80., 2., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64gather_pd() {
        let mut arr = [0f64; 128];
        for i in 0..128 {
            arr[i] = i as f64;
        }
        // A multiplier of 8 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i64gather_pd(index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512d(r, _mm512_setr_pd(0., 16., 32., 48., 64., 80., 96., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64gather_pd() {
        let mut arr = [0f64; 128];
        for i in 0..128 {
            arr[i] = i as f64;
        }
        let src = _mm512_set1_pd(2.);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 8 is word-addressing
        let r = _mm512_mask_i64gather_pd(src, mask, index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512d(r, _mm512_setr_pd(2., 16., 2., 48., 2., 80., 2., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64gather_ps() {
        let mut arr = [0f32; 128];
        for i in 0..128 {
            arr[i] = i as f32;
        }
        // A multiplier of 4 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i64gather_ps(index, arr.as_ptr() as *const u8, 4);
        assert_eq_m256(r, _mm256_setr_ps(0., 16., 32., 48., 64., 80., 96., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64gather_ps() {
        let mut arr = [0f32; 128];
        for i in 0..128 {
            arr[i] = i as f32;
        }
        let src = _mm256_set1_ps(2.);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 4 is word-addressing
        let r = _mm512_mask_i64gather_ps(src, mask, index, arr.as_ptr() as *const u8, 4);
        assert_eq_m256(r, _mm256_setr_ps(2., 16., 2., 48., 2., 80., 2., 112.));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32gather_epi64() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        // A multiplier of 8 is word-addressing
        #[rustfmt::skip]
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i32gather_epi64(index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512i(r, _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32gather_epi64() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        let src = _mm512_set1_epi64(2);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 8 is word-addressing
        let r = _mm512_mask_i32gather_epi64(src, mask, index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512i(r, _mm512_setr_epi64(2, 16, 2, 48, 2, 80, 2, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64gather_epi64() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        // A multiplier of 8 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i64gather_epi64(index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512i(r, _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64gather_epi64() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        let src = _mm512_set1_epi64(2);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 8 is word-addressing
        let r = _mm512_mask_i64gather_epi64(src, mask, index, arr.as_ptr() as *const u8, 8);
        assert_eq_m512i(r, _mm512_setr_epi64(2, 16, 2, 48, 2, 80, 2, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64gather_epi32() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        // A multiplier of 8 is word-addressing
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let r = _mm512_i64gather_epi32(index, arr.as_ptr() as *const u8, 8);
        assert_eq_m256i(r, _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64gather_epi32() {
        let mut arr = [0i64; 128];
        for i in 0..128i64 {
            arr[i as usize] = i;
        }
        let src = _mm256_set1_epi32(2);
        let mask = 0b10101010;
        #[rustfmt::skip]
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        // A multiplier of 8 is word-addressing
        let r = _mm512_mask_i64gather_epi32(src, mask, index, arr.as_ptr() as *const u8, 8);
        assert_eq_m256i(r, _mm256_setr_epi32(2, 16, 2, 48, 2, 80, 2, 112));
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32scatter_pd() {
        let mut arr = [0f64; 128];
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 8 is word-addressing
        _mm512_i32scatter_pd(arr.as_mut_ptr() as *mut u8, index, src, 8);
        let mut expected = [0f64; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as f64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32scatter_pd() {
        let mut arr = [0f64; 128];
        let mask = 0b10101010;
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 8 is word-addressing
        _mm512_mask_i32scatter_pd(arr.as_mut_ptr() as *mut u8, mask, index, src, 8);
        let mut expected = [0f64; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2. * (i + 1) as f64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64scatter_pd() {
        let mut arr = [0f64; 128];
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 8 is word-addressing
        _mm512_i64scatter_pd(arr.as_mut_ptr() as *mut u8, index, src, 8);
        let mut expected = [0f64; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as f64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64scatter_pd() {
        let mut arr = [0f64; 128];
        let mask = 0b10101010;
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 8 is word-addressing
        _mm512_mask_i64scatter_pd(arr.as_mut_ptr() as *mut u8, mask, index, src, 8);
        let mut expected = [0f64; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2. * (i + 1) as f64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64scatter_ps() {
        let mut arr = [0f32; 128];
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm256_setr_ps(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 4 is word-addressing
        _mm512_i64scatter_ps(arr.as_mut_ptr() as *mut u8, index, src, 4);
        let mut expected = [0f32; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as f32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64scatter_ps() {
        let mut arr = [0f32; 128];
        let mask = 0b10101010;
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm256_setr_ps(1., 2., 3., 4., 5., 6., 7., 8.);
        // A multiplier of 4 is word-addressing
        _mm512_mask_i64scatter_ps(arr.as_mut_ptr() as *mut u8, mask, index, src, 4);
        let mut expected = [0f32; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2. * (i + 1) as f32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i32scatter_epi64() {
        let mut arr = [0i64; 128];
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 8 is word-addressing
        _mm512_i32scatter_epi64(arr.as_mut_ptr() as *mut u8, index, src, 8);
        let mut expected = [0i64; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as i64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i32scatter_epi64() {
        let mut arr = [0i64; 128];
        let mask = 0b10101010;
        let index = _mm256_setr_epi32(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 8 is word-addressing
        _mm512_mask_i32scatter_epi64(arr.as_mut_ptr() as *mut u8, mask, index, src, 8);
        let mut expected = [0i64; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2 * (i + 1) as i64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64scatter_epi64() {
        let mut arr = [0i64; 128];
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 8 is word-addressing
        _mm512_i64scatter_epi64(arr.as_mut_ptr() as *mut u8, index, src, 8);
        let mut expected = [0i64; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as i64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64scatter_epi64() {
        let mut arr = [0i64; 128];
        let mask = 0b10101010;
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 8 is word-addressing
        _mm512_mask_i64scatter_epi64(arr.as_mut_ptr() as *mut u8, mask, index, src, 8);
        let mut expected = [0i64; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2 * (i + 1) as i64;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_i64scatter_epi32() {
        let mut arr = [0i32; 128];
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 4 is word-addressing
        _mm512_i64scatter_epi32(arr.as_mut_ptr() as *mut u8, index, src, 4);
        let mut expected = [0i32; 128];
        for i in 0..8 {
            expected[i * 16] = (i + 1) as i32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_i64scatter_epi32() {
        let mut arr = [0i32; 128];
        let mask = 0b10101010;
        let index = _mm512_setr_epi64(0, 16, 32, 48, 64, 80, 96, 112);
        let src = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8);
        // A multiplier of 4 is word-addressing
        _mm512_mask_i64scatter_epi32(arr.as_mut_ptr() as *mut u8, mask, index, src, 4);
        let mut expected = [0i32; 128];
        for i in 0..4 {
            expected[i * 32 + 16] = 2 * (i + 1) as i32;
        }
        assert_eq!(&arr[..], &expected[..],);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rol_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_rol_epi64(a, 1);
        let e = _mm512_set_epi64(
            1 << 0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rol_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_mask_rol_epi64(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rol_epi64(a, 0b11111111, a, 1);
        let e = _mm512_set_epi64(
            1 << 0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rol_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 63,
        );
        let r = _mm512_maskz_rol_epi64(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rol_epi64(0b00001111, a, 1);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 33, 1 << 33, 1 << 33, 1 << 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_ror_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_ror_epi64(a, 1);
        let e = _mm512_set_epi64(
            1 << 63,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_ror_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_mask_ror_epi64(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_ror_epi64(a, 0b11111111, a, 1);
        let e = _mm512_set_epi64(
            1 << 63,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_ror_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 0,
        );
        let r = _mm512_maskz_ror_epi64(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_ror_epi64(0b00001111, a, 1);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 31, 1 << 31, 1 << 31, 1 << 63);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_slli_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_slli_epi64(a, 1);
        let e = _mm512_set_epi64(
            0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_slli_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_mask_slli_epi64(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_slli_epi64(a, 0b11111111, a, 1);
        let e = _mm512_set_epi64(
            0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_slli_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 63,
        );
        let r = _mm512_maskz_slli_epi64(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_slli_epi64(0b00001111, a, 1);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 33, 1 << 33, 1 << 33, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srli_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_srli_epi64(a, 1);
        let e = _mm512_set_epi64(
            0,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srli_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let r = _mm512_mask_srli_epi64(a, 0, a, 1);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srli_epi64(a, 0b11111111, a, 1);
        let e = _mm512_set_epi64(
            0,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srli_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 0,
        );
        let r = _mm512_maskz_srli_epi64(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srli_epi64(0b00001111, a, 1);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 31, 1 << 31, 1 << 31, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rolv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_rolv_epi64(a, b);
        let e = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 34,
            1 << 35,
            1 << 36,
            1 << 37,
            1 << 38,
            1 << 39,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rolv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_rolv_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rolv_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 34,
            1 << 35,
            1 << 36,
            1 << 37,
            1 << 38,
            1 << 39,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rolv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 62,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 2);
        let r = _mm512_maskz_rolv_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rolv_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 36, 1 << 37, 1 << 38, 1 << 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_rorv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_rorv_epi64(a, b);
        let e = _mm512_set_epi64(
            1 << 32,
            1 << 63,
            1 << 30,
            1 << 29,
            1 << 28,
            1 << 27,
            1 << 26,
            1 << 25,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_rorv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_rorv_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_rorv_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(
            1 << 32,
            1 << 63,
            1 << 30,
            1 << 29,
            1 << 28,
            1 << 27,
            1 << 26,
            1 << 25,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_rorv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 0,
        );
        let b = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 2);
        let r = _mm512_maskz_rorv_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_rorv_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 28, 1 << 27, 1 << 26, 1 << 62);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sllv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm512_set_epi64(0, 2, 2, 3, 4, 5, 6, 7);
        let r = _mm512_sllv_epi64(a, count);
        let e = _mm512_set_epi64(
            1 << 32,
            0,
            1 << 34,
            1 << 35,
            1 << 36,
            1 << 37,
            1 << 38,
            1 << 39,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sllv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_sllv_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sllv_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(
            1 << 32,
            1 << 33,
            0,
            1 << 35,
            1 << 36,
            1 << 37,
            1 << 38,
            1 << 39,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sllv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 63,
        );
        let count = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 1);
        let r = _mm512_maskz_sllv_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sllv_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 36, 1 << 37, 1 << 38, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srlv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_srlv_epi64(a, count);
        let e = _mm512_set_epi64(
            1 << 32,
            0,
            1 << 30,
            1 << 29,
            1 << 28,
            1 << 27,
            1 << 26,
            1 << 25,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srlv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_srlv_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srlv_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(
            1 << 32,
            0,
            1 << 30,
            1 << 29,
            1 << 28,
            1 << 27,
            1 << 26,
            1 << 25,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srlv_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 0,
        );
        let count = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_maskz_srlv_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srlv_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 28, 1 << 27, 1 << 26, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sll_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_sll_epi64(a, count);
        let e = _mm512_set_epi64(
            0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);

        let count = _mm_set_epi64x(1, 0);
        let r = _mm512_sll_epi64(a, count);
        assert_eq_m512i(r, a);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sll_epi64() {
        let a = _mm512_set_epi64(
            1 << 63,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_mask_sll_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sll_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(
            0,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
            1 << 33,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sll_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 63,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_maskz_sll_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sll_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 33, 1 << 33, 1 << 33, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srl_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_srl_epi64(a, count);
        let e = _mm512_set_epi64(
            0,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srl_epi64() {
        let a = _mm512_set_epi64(
            1 << 0,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_mask_srl_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srl_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(
            0,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
            1 << 31,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srl_epi64() {
        let a = _mm512_set_epi64(
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 32,
            1 << 0,
        );
        let count = _mm_set_epi64x(0, 1);
        let r = _mm512_maskz_srl_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srl_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 1 << 31, 1 << 31, 1 << 31, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_sra_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm_set_epi64x(0, 2);
        let r = _mm512_sra_epi64(a, count);
        let e = _mm512_set_epi64(0, -2, 0, 0, 0, 0, 3, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_sra_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm_set_epi64x(0, 2);
        let r = _mm512_mask_sra_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_sra_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(0, -2, 0, 0, 0, 0, 3, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_sra_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm_set_epi64x(0, 2);
        let r = _mm512_maskz_sra_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_sra_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 3, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srav_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm512_set_epi64(2, 2, 0, 0, 0, 0, 2, 1);
        let r = _mm512_srav_epi64(a, count);
        let e = _mm512_set_epi64(0, -2, 0, 0, 0, 0, 3, -8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srav_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm512_set_epi64(2, 2, 0, 0, 0, 0, 2, 1);
        let r = _mm512_mask_srav_epi64(a, 0, a, count);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srav_epi64(a, 0b11111111, a, count);
        let e = _mm512_set_epi64(0, -2, 0, 0, 0, 0, 3, -8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srav_epi64() {
        let a = _mm512_set_epi64(1, -8, 0, 0, 0, 0, 15, -16);
        let count = _mm512_set_epi64(2, 2, 0, 0, 0, 0, 2, 1);
        let r = _mm512_maskz_srav_epi64(0, a, count);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srav_epi64(0b00001111, a, count);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 3, -8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_srai_epi64() {
        let a = _mm512_set_epi64(1, -4, 15, 0, 0, 0, 0, -16);
        let r = _mm512_srai_epi64(a, 2);
        let e = _mm512_set_epi64(0, -1, 3, 0, 0, 0, 0, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_srai_epi64() {
        let a = _mm512_set_epi64(1, -4, 15, 0, 0, 0, 0, -16);
        let r = _mm512_mask_srai_epi64(a, 0, a, 2);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_srai_epi64(a, 0b11111111, a, 2);
        let e = _mm512_set_epi64(0, -1, 3, 0, 0, 0, 0, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_srai_epi64() {
        let a = _mm512_set_epi64(1, -4, 15, 0, 0, 0, 0, -16);
        let r = _mm512_maskz_srai_epi64(0, a, 2);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_srai_epi64(0b00001111, a, 2);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, -4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permute_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_permute_pd(a, 1);
        let e = _mm512_set_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permute_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_mask_permute_pd(a, 0, a, 1);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_permute_pd(a, 0b11111111, a, 1);
        let e = _mm512_set_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permute_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_maskz_permute_pd(0, a, 1);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_permute_pd(0b00001111, a, 1);
        let e = _mm512_set_pd(0., 0., 0., 0., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_permutex_epi64(a, 1);
        let e = _mm512_set_epi64(6, 6, 6, 6, 6, 6, 6, 6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_permutex_epi64(a, 0, a, 1);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutex_epi64(a, 0b11111111, a, 1);
        let e = _mm512_set_epi64(6, 6, 6, 6, 6, 6, 6, 6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_maskz_permutex_epi64(0, a, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_permutex_epi64(0b00001111, a, 1);
        let e = _mm512_set_epi64(0, 0, 0, 0, 6, 6, 6, 6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_permutex_pd(a, 1);
        let e = _mm512_set_pd(6., 6., 6., 6., 6., 6., 6., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_mask_permutex_pd(a, 0, a, 1);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_permutex_pd(a, 0b11111111, a, 1);
        let e = _mm512_set_pd(6., 6., 6., 6., 6., 6., 6., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_maskz_permutex_pd(0, a, 1);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_permutex_pd(0b00001111, a, 1);
        let e = _mm512_set_pd(0., 0., 0., 0., 6., 6., 6., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutevar_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_permutevar_pd(a, b);
        let e = _mm512_set_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutevar_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_mask_permutevar_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_permutevar_pd(a, 0b11111111, a, b);
        let e = _mm512_set_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutevar_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let b = _mm512_set1_epi64(1);
        let r = _mm512_maskz_permutevar_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_permutevar_pd(0b00001111, a, b);
        let e = _mm512_set_pd(0., 0., 0., 0., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutexvar_epi64() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_permutexvar_epi64(idx, a);
        let e = _mm512_set1_epi64(6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutexvar_epi64() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_mask_permutexvar_epi64(a, 0, idx, a);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutexvar_epi64(a, 0b11111111, idx, a);
        let e = _mm512_set1_epi64(6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutexvar_epi64() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let r = _mm512_maskz_permutexvar_epi64(0, idx, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_permutexvar_epi64(0b00001111, idx, a);
        let e = _mm512_set_epi64(0, 0, 0, 0, 6, 6, 6, 6);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutexvar_pd() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_permutexvar_pd(idx, a);
        let e = _mm512_set1_pd(6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutexvar_pd() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_mask_permutexvar_pd(a, 0, idx, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_permutexvar_pd(a, 0b11111111, idx, a);
        let e = _mm512_set1_pd(6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutexvar_pd() {
        let idx = _mm512_set1_epi64(1);
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let r = _mm512_maskz_permutexvar_pd(0, idx, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_permutexvar_pd(0b00001111, idx, a);
        let e = _mm512_set_pd(0., 0., 0., 0., 6., 6., 6., 6.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex2var_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_epi64(100);
        let r = _mm512_permutex2var_epi64(a, idx, b);
        let e = _mm512_set_epi64(6, 100, 5, 100, 4, 100, 3, 100);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex2var_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_epi64(100);
        let r = _mm512_mask_permutex2var_epi64(a, 0, idx, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_permutex2var_epi64(a, 0b11111111, idx, b);
        let e = _mm512_set_epi64(6, 100, 5, 100, 4, 100, 3, 100);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex2var_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_epi64(100);
        let r = _mm512_maskz_permutex2var_epi64(0, a, idx, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_permutex2var_epi64(0b00001111, a, idx, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 4, 100, 3, 100);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask2_permutex2var_epi64() {
        let a = _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7);
        let idx = _mm512_set_epi64(1000, 1 << 3, 2000, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_epi64(100);
        let r = _mm512_mask2_permutex2var_epi64(a, idx, 0, b);
        assert_eq_m512i(r, idx);
        let r = _mm512_mask2_permutex2var_epi64(a, idx, 0b00001111, b);
        let e = _mm512_set_epi64(1000, 1 << 3, 2000, 1 << 3, 4, 100, 3, 100);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_permutex2var_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_pd(100.);
        let r = _mm512_permutex2var_pd(a, idx, b);
        let e = _mm512_set_pd(6., 100., 5., 100., 4., 100., 3., 100.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_permutex2var_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_pd(100.);
        let r = _mm512_mask_permutex2var_pd(a, 0, idx, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_permutex2var_pd(a, 0b11111111, idx, b);
        let e = _mm512_set_pd(6., 100., 5., 100., 4., 100., 3., 100.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_permutex2var_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_pd(100.);
        let r = _mm512_maskz_permutex2var_pd(0, a, idx, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_permutex2var_pd(0b00001111, a, idx, b);
        let e = _mm512_set_pd(0., 0., 0., 0., 4., 100., 3., 100.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask2_permutex2var_pd() {
        let a = _mm512_set_pd(0., 1., 2., 3., 4., 5., 6., 7.);
        let idx = _mm512_set_epi64(1, 1 << 3, 2, 1 << 3, 3, 1 << 3, 4, 1 << 3);
        let b = _mm512_set1_pd(100.);
        let r = _mm512_mask2_permutex2var_pd(a, idx, 0, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_mask2_permutex2var_pd(a, idx, 0b00001111, b);
        let e = _mm512_set_pd(0., 0., 0., 0., 4., 100., 3., 100.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_pd() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 1., 4., 5., 8.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 2., 3., 6., 7.);
        let r = _mm512_shuffle_pd(
            a,
            b,
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7,
        );
        let e = _mm512_setr_pd(4., 3., 8., 7., 4., 3., 8., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_pd() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 1., 4., 5., 8.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 2., 3., 6., 7.);
        let r = _mm512_mask_shuffle_pd(
            a,
            0,
            a,
            b,
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7,
        );
        assert_eq_m512d(r, a);
        let r = _mm512_mask_shuffle_pd(
            a,
            0b11111111,
            a,
            b,
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7,
        );
        let e = _mm512_setr_pd(4., 3., 8., 7., 4., 3., 8., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_pd() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 1., 4., 5., 8.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 2., 3., 6., 7.);
        let r = _mm512_maskz_shuffle_pd(
            0,
            a,
            b,
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7,
        );
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_shuffle_pd(
            0b00001111,
            a,
            b,
            1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7,
        );
        let e = _mm512_setr_pd(4., 3., 8., 7., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_i64x2() {
        let a = _mm512_setr_epi64(1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi64(2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_shuffle_i64x2(a, b, 0b00000000);
        let e = _mm512_setr_epi64(1, 4, 1, 4, 2, 3, 2, 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_i64x2() {
        let a = _mm512_setr_epi64(1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi64(2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_mask_shuffle_i64x2(a, 0, a, b, 0b00000000);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_shuffle_i64x2(a, 0b11111111, a, b, 0b00000000);
        let e = _mm512_setr_epi64(1, 4, 1, 4, 2, 3, 2, 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_i64x2() {
        let a = _mm512_setr_epi64(1, 4, 5, 8, 9, 12, 13, 16);
        let b = _mm512_setr_epi64(2, 3, 6, 7, 10, 11, 14, 15);
        let r = _mm512_maskz_shuffle_i64x2(0, a, b, 0b00000000);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_shuffle_i64x2(0b00001111, a, b, 0b00000000);
        let e = _mm512_setr_epi64(1, 4, 1, 4, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_shuffle_f64x2() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 9., 12., 13., 16.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 10., 11., 14., 15.);
        let r = _mm512_shuffle_f64x2(a, b, 0b00000000);
        let e = _mm512_setr_pd(1., 4., 1., 4., 2., 3., 2., 3.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_shuffle_f64x2() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 9., 12., 13., 16.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 10., 11., 14., 15.);
        let r = _mm512_mask_shuffle_f64x2(a, 0, a, b, 0b00000000);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_shuffle_f64x2(a, 0b11111111, a, b, 0b00000000);
        let e = _mm512_setr_pd(1., 4., 1., 4., 2., 3., 2., 3.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_shuffle_f64x2() {
        let a = _mm512_setr_pd(1., 4., 5., 8., 9., 12., 13., 16.);
        let b = _mm512_setr_pd(2., 3., 6., 7., 10., 11., 14., 15.);
        let r = _mm512_maskz_shuffle_f64x2(0, a, b, 0b00000000);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_shuffle_f64x2(0b00001111, a, b, 0b00000000);
        let e = _mm512_setr_pd(1., 4., 1., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_movedup_pd() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let r = _mm512_movedup_pd(a);
        let e = _mm512_setr_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_movedup_pd() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let r = _mm512_mask_movedup_pd(a, 0, a);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_movedup_pd(a, 0b11111111, a);
        let e = _mm512_setr_pd(1., 1., 3., 3., 5., 5., 7., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_movedup_pd() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let r = _mm512_maskz_movedup_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_movedup_pd(0b00001111, a);
        let e = _mm512_setr_pd(1., 1., 3., 3., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_inserti64x4() {
        let a = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm256_setr_epi64x(17, 18, 19, 20);
        let r = _mm512_inserti64x4(a, b, 1);
        let e = _mm512_setr_epi64(1, 2, 3, 4, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_inserti64x4() {
        let a = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm256_setr_epi64x(17, 18, 19, 20);
        let r = _mm512_mask_inserti64x4(a, 0, a, b, 1);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_inserti64x4(a, 0b11111111, a, b, 1);
        let e = _mm512_setr_epi64(1, 2, 3, 4, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_inserti64x4() {
        let a = _mm512_setr_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm256_setr_epi64x(17, 18, 19, 20);
        let r = _mm512_maskz_inserti64x4(0, a, b, 1);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_inserti64x4(0b00001111, a, b, 1);
        let e = _mm512_setr_epi64(1, 2, 3, 4, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_insertf64x4() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm256_setr_pd(17., 18., 19., 20.);
        let r = _mm512_insertf64x4(a, b, 1);
        let e = _mm512_setr_pd(1., 2., 3., 4., 17., 18., 19., 20.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_insertf64x4() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm256_setr_pd(17., 18., 19., 20.);
        let r = _mm512_mask_insertf64x4(a, 0, a, b, 1);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_insertf64x4(a, 0b11111111, a, b, 1);
        let e = _mm512_setr_pd(1., 2., 3., 4., 17., 18., 19., 20.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_insertf64x4() {
        let a = _mm512_setr_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm256_setr_pd(17., 18., 19., 20.);
        let r = _mm512_maskz_insertf64x4(0, a, b, 1);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_insertf64x4(0b00001111, a, b, 1);
        let e = _mm512_setr_pd(1., 2., 3., 4., 0., 0., 0., 0.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd128_pd512() {
        let a = _mm_setr_pd(17., 18.);
        let r = _mm512_castpd128_pd512(a);
        let e = _mm512_setr_pd(17., 18., -1., -1., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd256_pd512() {
        let a = _mm256_setr_pd(17., 18., 19., 20.);
        let r = _mm512_castpd256_pd512(a);
        let e = _mm512_setr_pd(17., 18., 19., 20., -1., -1., -1., -1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd512_pd128() {
        let a = _mm512_setr_pd(17., 18., -1., -1., -1., -1., -1., -1.);
        let r = _mm512_castpd512_pd128(a);
        let e = _mm_setr_pd(17., 18.);
        assert_eq_m128d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd512_pd256() {
        let a = _mm512_setr_pd(17., 18., 19., 20., -1., -1., -1., -1.);
        let r = _mm512_castpd512_pd256(a);
        let e = _mm256_setr_pd(17., 18., 19., 20.);
        assert_eq_m256d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd_ps() {
        let a = _mm512_set1_pd(1.);
        let r = _mm512_castpd_ps(a);
        let e = _mm512_set_ps(
            1.875, 0.0, 1.875, 0.0, 1.875, 0.0, 1.875, 0.0, 1.875, 0.0, 1.875, 0.0, 1.875, 0.0,
            1.875, 0.0,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castpd_si512() {
        let a = _mm512_set1_pd(1.);
        let r = _mm512_castpd_si512(a);
        let e = _mm512_set_epi32(
            1072693248, 0, 1072693248, 0, 1072693248, 0, 1072693248, 0, 1072693248, 0, 1072693248,
            0, 1072693248, 0, 1072693248, 0,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi128_si512() {
        let a = _mm_setr_epi64x(17, 18);
        let r = _mm512_castsi128_si512(a);
        let e = _mm512_setr_epi64(17, 18, -1, -1, -1, -1, -1, -1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi256_si512() {
        let a = _mm256_setr_epi64x(17, 18, 19, 20);
        let r = _mm512_castsi256_si512(a);
        let e = _mm512_setr_epi64(17, 18, 19, 20, -1, -1, -1, -1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi512_si128() {
        let a = _mm512_setr_epi64(17, 18, -1, -1, -1, -1, -1, -1);
        let r = _mm512_castsi512_si128(a);
        let e = _mm_setr_epi64x(17, 18);
        assert_eq_m128i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi512_si256() {
        let a = _mm512_setr_epi64(17, 18, 19, 20, -1, -1, -1, -1);
        let r = _mm512_castsi512_si256(a);
        let e = _mm256_setr_epi64x(17, 18, 19, 20);
        assert_eq_m256i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi512_ps() {
        let a = _mm512_set1_epi64(1 << 62);
        let r = _mm512_castsi512_ps(a);
        let e = _mm512_set_ps(
            2., 0., 2., 0., 2., 0., 2., 0., 2., 0., 2., 0., 2., 0., 2., 0.,
        );
        assert_eq_m512(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_castsi512_pd() {
        let a = _mm512_set1_epi64(1 << 62);
        let r = _mm512_castsi512_pd(a);
        let e = _mm512_set_pd(2., 2., 2., 2., 2., 2., 2., 2.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcastq_epi64() {
        let a = _mm_setr_epi64x(17, 18);
        let r = _mm512_broadcastq_epi64(a);
        let e = _mm512_set1_epi64(17);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcastq_epi64() {
        let src = _mm512_set1_epi64(18);
        let a = _mm_setr_epi64x(17, 18);
        let r = _mm512_mask_broadcastq_epi64(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_broadcastq_epi64(src, 0b11111111, a);
        let e = _mm512_set1_epi64(17);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcastq_epi64() {
        let a = _mm_setr_epi64x(17, 18);
        let r = _mm512_maskz_broadcastq_epi64(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_broadcastq_epi64(0b00001111, a);
        let e = _mm512_set_epi64(0, 0, 0, 0, 17, 17, 17, 17);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcastsd_pd() {
        let a = _mm_setr_pd(17., 18.);
        let r = _mm512_broadcastsd_pd(a);
        let e = _mm512_set1_pd(18.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcastsd_pd() {
        let src = _mm512_set1_pd(18.);
        let a = _mm_setr_pd(17., 18.);
        let r = _mm512_mask_broadcastsd_pd(src, 0, a);
        assert_eq_m512d(r, src);
        let r = _mm512_mask_broadcastsd_pd(src, 0b01111111, a);
        let e = _mm512_set1_pd(18.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcastsd_pd() {
        let a = _mm_setr_pd(17., 18.);
        let r = _mm512_maskz_broadcastsd_pd(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_broadcastsd_pd(0b00001111, a);
        let e = _mm512_set_pd(0., 0., 0., 0., 18., 18., 18., 18.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcast_i64x4() {
        let a = _mm256_set_epi64x(17, 18, 19, 20);
        let r = _mm512_broadcast_i64x4(a);
        let e = _mm512_set_epi64(17, 18, 19, 20, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcast_i64x4() {
        let src = _mm512_set1_epi64(18);
        let a = _mm256_set_epi64x(17, 18, 19, 20);
        let r = _mm512_mask_broadcast_i64x4(src, 0, a);
        assert_eq_m512i(r, src);
        let r = _mm512_mask_broadcast_i64x4(src, 0b11111111, a);
        let e = _mm512_set_epi64(17, 18, 19, 20, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcast_i64x4() {
        let a = _mm256_set_epi64x(17, 18, 19, 20);
        let r = _mm512_maskz_broadcast_i64x4(0, a);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_broadcast_i64x4(0b00001111, a);
        let e = _mm512_set_epi64(0, 0, 0, 0, 17, 18, 19, 20);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_broadcast_f64x4() {
        let a = _mm256_set_pd(17., 18., 19., 20.);
        let r = _mm512_broadcast_f64x4(a);
        let e = _mm512_set_pd(17., 18., 19., 20., 17., 18., 19., 20.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_broadcast_f64x4() {
        let src = _mm512_set1_pd(18.);
        let a = _mm256_set_pd(17., 18., 19., 20.);
        let r = _mm512_mask_broadcast_f64x4(src, 0, a);
        assert_eq_m512d(r, src);
        let r = _mm512_mask_broadcast_f64x4(src, 0b11111111, a);
        let e = _mm512_set_pd(17., 18., 19., 20., 17., 18., 19., 20.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_broadcast_f64x4() {
        let a = _mm256_set_pd(17., 18., 19., 20.);
        let r = _mm512_maskz_broadcast_f64x4(0, a);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_broadcast_f64x4(0b00001111, a);
        let e = _mm512_set_pd(0., 0., 0., 0., 17., 18., 19., 20.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_blend_epi64() {
        let a = _mm512_set1_epi64(1);
        let b = _mm512_set1_epi64(2);
        let r = _mm512_mask_blend_epi64(0b11110000, a, b);
        let e = _mm512_set_epi64(2, 2, 2, 2, 1, 1, 1, 1);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_blend_pd() {
        let a = _mm512_set1_pd(1.);
        let b = _mm512_set1_pd(2.);
        let r = _mm512_mask_blend_pd(0b11110000, a, b);
        let e = _mm512_set_pd(2., 2., 2., 2., 1., 1., 1., 1.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpackhi_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_unpackhi_epi64(a, b);
        let e = _mm512_set_epi64(17, 1, 19, 3, 21, 5, 23, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpackhi_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_mask_unpackhi_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_unpackhi_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(17, 1, 19, 3, 21, 5, 23, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpackhi_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_maskz_unpackhi_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_unpackhi_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 21, 5, 23, 7);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpackhi_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_unpackhi_pd(a, b);
        let e = _mm512_set_pd(17., 1., 19., 3., 21., 5., 23., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpackhi_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_mask_unpackhi_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_unpackhi_pd(a, 0b11111111, a, b);
        let e = _mm512_set_pd(17., 1., 19., 3., 21., 5., 23., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpackhi_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_maskz_unpackhi_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_unpackhi_pd(0b00001111, a, b);
        let e = _mm512_set_pd(0., 0., 0., 0., 21., 5., 23., 7.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpacklo_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_unpacklo_epi64(a, b);
        let e = _mm512_set_epi64(18, 2, 20, 4, 22, 6, 24, 8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpacklo_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_mask_unpacklo_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);
        let r = _mm512_mask_unpacklo_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(18, 2, 20, 4, 22, 6, 24, 8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpacklo_epi64() {
        let a = _mm512_set_epi64(1, 2, 3, 4, 5, 6, 7, 8);
        let b = _mm512_set_epi64(17, 18, 19, 20, 21, 22, 23, 24);
        let r = _mm512_maskz_unpacklo_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());
        let r = _mm512_maskz_unpacklo_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 22, 6, 24, 8);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_unpacklo_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_unpacklo_pd(a, b);
        let e = _mm512_set_pd(18., 2., 20., 4., 22., 6., 24., 8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_unpacklo_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_mask_unpacklo_pd(a, 0, a, b);
        assert_eq_m512d(r, a);
        let r = _mm512_mask_unpacklo_pd(a, 0b11111111, a, b);
        let e = _mm512_set_pd(18., 2., 20., 4., 22., 6., 24., 8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_unpacklo_pd() {
        let a = _mm512_set_pd(1., 2., 3., 4., 5., 6., 7., 8.);
        let b = _mm512_set_pd(17., 18., 19., 20., 21., 22., 23., 24.);
        let r = _mm512_maskz_unpacklo_pd(0, a, b);
        assert_eq_m512d(r, _mm512_setzero_pd());
        let r = _mm512_maskz_unpacklo_pd(0b00001111, a, b);
        let e = _mm512_set_pd(0., 0., 0., 0., 22., 6., 24., 8.);
        assert_eq_m512d(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_and_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_and_epi64(a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_and_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_mask_and_epi64(a, 0, a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);

        let r = _mm512_mask_and_epi64(a, 0b01111111, a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_and_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_maskz_and_epi64(0, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);

        let r = _mm512_maskz_and_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_and_si512() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_and_epi64(a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_or_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_or_epi64(a, b);
        let e = _mm512_set_epi64(
            1 << 0 | 1 << 13 | 1 << 15,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 2 | 1 << 3,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_or_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_mask_or_epi64(a, 0, a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);

        let r = _mm512_mask_or_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(
            1 << 0 | 1 << 13 | 1 << 15,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 2 | 1 << 3,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_or_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_maskz_or_epi64(0, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);

        let r = _mm512_maskz_or_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_or_si512() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_or_epi64(a, b);
        let e = _mm512_set_epi64(
            1 << 0 | 1 << 13 | 1 << 15,
            0,
            0,
            0,
            0,
            0,
            0,
            1 << 1 | 1 << 2 | 1 << 3,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_xor_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_xor_epi64(a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 13 | 1 << 15, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_xor_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_mask_xor_epi64(a, 0, a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        assert_eq_m512i(r, e);

        let r = _mm512_mask_xor_epi64(a, 0b11111111, a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 13 | 1 << 15, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_xor_epi64() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_maskz_xor_epi64(0, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);

        let r = _mm512_maskz_xor_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_xor_si512() {
        let a = _mm512_set_epi64(1 << 0 | 1 << 15, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let b = _mm512_set_epi64(1 << 13, 0, 0, 0, 0, 0, 0, 1 << 1 | 1 << 2 | 1 << 3);
        let r = _mm512_xor_epi64(a, b);
        let e = _mm512_set_epi64(1 << 0 | 1 << 13 | 1 << 15, 0, 0, 0, 0, 0, 0, 0);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_andnot_epi64() {
        let a = _mm512_set1_epi64(0);
        let b = _mm512_set1_epi64(1 << 3 | 1 << 4);
        let r = _mm512_andnot_epi64(a, b);
        let e = _mm512_set1_epi64(1 << 3 | 1 << 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_mask_andnot_epi64() {
        let a = _mm512_set1_epi64(1 << 1 | 1 << 2);
        let b = _mm512_set1_epi64(1 << 3 | 1 << 4);
        let r = _mm512_mask_andnot_epi64(a, 0, a, b);
        assert_eq_m512i(r, a);

        let r = _mm512_mask_andnot_epi64(a, 0b11111111, a, b);
        let e = _mm512_set1_epi64(1 << 3 | 1 << 4);
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_maskz_andnot_epi64() {
        let a = _mm512_set1_epi64(1 << 1 | 1 << 2);
        let b = _mm512_set1_epi64(1 << 3 | 1 << 4);
        let r = _mm512_maskz_andnot_epi64(0, a, b);
        assert_eq_m512i(r, _mm512_setzero_si512());

        let r = _mm512_maskz_andnot_epi64(0b00001111, a, b);
        let e = _mm512_set_epi64(
            0,
            0,
            0,
            0,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
            1 << 3 | 1 << 4,
        );
        assert_eq_m512i(r, e);
    }

    #[simd_test(enable = "avx512f")]
    unsafe fn test_mm512_andnot_si512() {
        let a = _mm512_set1_epi64(0);
        let b = _mm512_set1_epi64(1 << 3 | 1 << 4);
        let r = _mm512_andnot_si512(a, b);
        let e = _mm512_set1_epi64(1 << 3 | 1 << 4);
        assert_eq_m512i(r, e);
    }
}

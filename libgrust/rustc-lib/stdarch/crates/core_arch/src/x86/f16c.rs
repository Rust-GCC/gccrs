//! [F16C intrinsics].
//!
//! [F16C intrinsics]: https://software.intel.com/sites/landingpage/IntrinsicsGuide/#text=fp16&expand=1769

use crate::{
    core_arch::{simd::*, x86::*},
    hint::unreachable_unchecked,
    mem::transmute,
};

#[cfg(test)]
use stdarch_test::assert_instr;

#[allow(improper_ctypes)]
extern "unadjusted" {
    #[link_name = "llvm.x86.vcvtph2ps.128"]
    fn llvm_vcvtph2ps_128(a: i16x8) -> f32x4;
    #[link_name = "llvm.x86.vcvtph2ps.256"]
    fn llvm_vcvtph2ps_256(a: i16x8) -> f32x8;
    #[link_name = "llvm.x86.vcvtps2ph.128"]
    fn llvm_vcvtps2ph_128(a: f32x4, rounding: i32) -> i16x8;
    #[link_name = "llvm.x86.vcvtps2ph.256"]
    fn llvm_vcvtps2ph_256(a: f32x8, rounding: i32) -> i16x8;
}

/// Converts the 4 x 16-bit half-precision float values in the lowest 64-bit of
/// the 128-bit vector `a` into 4 x 32-bit float values stored in a 128-bit wide
/// vector.
#[inline]
#[target_feature(enable = "f16c")]
#[cfg_attr(test, assert_instr("vcvtph2ps"))]
pub unsafe fn _mm_cvtph_ps(a: __m128i) -> __m128 {
    transmute(llvm_vcvtph2ps_128(transmute(a)))
}

/// Converts the 8 x 16-bit half-precision float values in the 128-bit vector
/// `a` into 8 x 32-bit float values stored in a 256-bit wide vector.
#[inline]
#[target_feature(enable = "f16c")]
#[cfg_attr(test, assert_instr("vcvtph2ps"))]
pub unsafe fn _mm256_cvtph_ps(a: __m128i) -> __m256 {
    transmute(llvm_vcvtph2ps_256(transmute(a)))
}

macro_rules! dispatch_rounding {
    ($rounding:ident, $call:ident) => {{
        match $rounding {
            0 => call!(0),
            1 => call!(1),
            2 => call!(2),
            3 => call!(3),
            4 => call!(4),
            5 => call!(5),
            6 => call!(6),
            7 => call!(7),
            _ => unreachable_unchecked(),
        }
    }};
}

/// Converts the 4 x 32-bit float values in the 128-bit vector `a` into 4 x
/// 16-bit half-precision float values stored in the lowest 64-bit of a 128-bit
/// vector.
///
/// Rounding is done according to the `imm_rounding` parameter, which can be one of:
///
/// * `_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC`: round to nearest and suppress exceptions,
/// * `_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC`: round down and suppress exceptions,
/// * `_MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC`: round up and suppress exceptions,
/// * `_MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC`: truncate and suppress exceptions,
/// * `_MM_FROUND_CUR_DIRECTION`: use `MXCSR.RC` - see [`_MM_SET_ROUNDING_MODE`].
#[inline]
#[target_feature(enable = "f16c")]
#[rustc_args_required_const(1)]
#[cfg_attr(test, assert_instr("vcvtps2ph", imm_rounding = 0))]
pub unsafe fn _mm_cvtps_ph(a: __m128, imm_rounding: i32) -> __m128i {
    let a = transmute(a);
    macro_rules! call {
        ($rounding:expr) => {
            llvm_vcvtps2ph_128(a, $rounding)
        };
    }
    transmute(dispatch_rounding!(imm_rounding, call))
}

/// Converts the 8 x 32-bit float values in the 256-bit vector `a` into 8 x
/// 16-bit half-precision float values stored in a 128-bit wide vector.
///
/// Rounding is done according to the `imm_rounding` parameter, which can be one of:
///
/// * `_MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC`: round to nearest and suppress exceptions,
/// * `_MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC`: round down and suppress exceptions,
/// * `_MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC`: round up and suppress exceptions,
/// * `_MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC`: truncate and suppress exceptions,
/// * `_MM_FROUND_CUR_DIRECTION`: use `MXCSR.RC` - see [`_MM_SET_ROUNDING_MODE`].
#[inline]
#[target_feature(enable = "f16c")]
#[rustc_args_required_const(1)]
#[cfg_attr(test, assert_instr("vcvtps2ph", imm_rounding = 0))]
pub unsafe fn _mm256_cvtps_ph(a: __m256, imm_rounding: i32) -> __m128i {
    let a = transmute(a);
    macro_rules! call {
        ($rounding:expr) => {
            llvm_vcvtps2ph_256(a, $rounding)
        };
    }
    transmute(dispatch_rounding!(imm_rounding, call))
}

#[cfg(test)]
mod tests {
    use crate::{core_arch::x86::*, mem::transmute};
    use stdarch_test::simd_test;

    #[simd_test(enable = "f16c")]
    unsafe fn test_mm_cvtph_ps() {
        let array = [1_f32, 2_f32, 3_f32, 4_f32];
        let float_vec: __m128 = transmute(array);
        let halfs: __m128i = _mm_cvtps_ph(float_vec, 0);
        let floats: __m128 = _mm_cvtph_ps(halfs);
        let result: [f32; 4] = transmute(floats);
        assert_eq!(result, array);
    }

    #[simd_test(enable = "f16c")]
    unsafe fn test_mm256_cvtph_ps() {
        let array = [1_f32, 2_f32, 3_f32, 4_f32, 5_f32, 6_f32, 7_f32, 8_f32];
        let float_vec: __m256 = transmute(array);
        let halfs: __m128i = _mm256_cvtps_ph(float_vec, 0);
        let floats: __m256 = _mm256_cvtph_ps(halfs);
        let result: [f32; 8] = transmute(floats);
        assert_eq!(result, array);
    }
}

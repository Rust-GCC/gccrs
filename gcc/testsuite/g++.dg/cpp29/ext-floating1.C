// P3899R3 - Clarify the behavior of floating-point overflow
// { dg-do compile { target c++23 } }
// { dg-options "-fexcess-precision=16" }
// { dg-add-options float16 }
// { dg-add-options float32 }
// { dg-add-options float64 }
// { dg-add-options float128 }
// { dg-add-options bfloat16 }

#include <stdfloat>
#include <limits>

#ifdef __STDCPP_FLOAT16_T__
constexpr std::float16_t min16 = std::numeric_limits<std::float16_t>::min ();
constexpr std::float16_t max16 = std::numeric_limits<std::float16_t>::max ();
constexpr std::float16_t inf16 = std::numeric_limits<std::float16_t>::infinity ();
constexpr std::float16_t nan16 = std::numeric_limits<std::float16_t>::quiet_NaN ();
constexpr std::float16_t snan16 = std::numeric_limits<std::float16_t>::signaling_NaN ();

// Furthermore, if arithmetic expressions with operands of type std::float16_t
// behave as specified in ISO/IEC 60559 for binary16:
constexpr std::float16_t inf216 = inf16 * 2.0f16; // OK, also positive infinity
constexpr std::float16_t zero16 = min16 / max16; // OK, result cannot be represented, and is rounded to zero
constexpr std::float16_t oflo16 = max16 * 2.0f16; // { dg-error "is not a constant expression" "" { target float16 } }
constexpr std::float16_t nan216 = nan16 * 2.0f16; // OK, propagating a NaN
constexpr std::float16_t udef16 = inf16 * 0.0f16; // { dg-error "is not a constant expression" "" { target float16 } }
constexpr std::float16_t div016 = max16 / 0.0f16; // { dg-error "is not a constant expression" "" { target float16 } }
constexpr std::float16_t zero216 = min16 * min16;
constexpr bool nancmp16 = nan16 < 0.0f16;
constexpr std::float16_t snan216 = snan16 * 2.0f16;
constexpr std::float16_t div0216 = 1.0f16 / 0.0f16; // { dg-error "is not a constant expression" "" { target float16 } }
constexpr std::float16_t div0316 = 0.0f16 / 0.0f16; // { dg-error "is not a constant expression" "" { target float16 } }
#endif

#ifdef __STDCPP_FLOAT32_T__
constexpr std::float32_t min32 = std::numeric_limits<std::float32_t>::min ();
constexpr std::float32_t max32 = std::numeric_limits<std::float32_t>::max ();
constexpr std::float32_t inf32 = std::numeric_limits<std::float32_t>::infinity ();
constexpr std::float32_t nan32 = std::numeric_limits<std::float32_t>::quiet_NaN ();
constexpr std::float32_t snan32 = std::numeric_limits<std::float32_t>::signaling_NaN ();

// Furthermore, if arithmetic expressions with operands of type std::float32_t
// behave as specified in ISO/IEC 60559 for binary32:
constexpr std::float32_t inf232 = inf32 * 2.0f32; // OK, also positive infinity
constexpr std::float32_t zero32 = min32 / max32; // OK, result cannot be represented, and is rounded to zero
constexpr std::float32_t oflo32 = max32 * 2.0f32; // { dg-error "is not a constant expression" "" { target float32 } }
constexpr std::float32_t nan232 = nan32 * 2.0f32; // OK, propagating a NaN
constexpr std::float32_t udef32 = inf32 * 0.0f32; // { dg-error "is not a constant expression" "" { target float32 } }
constexpr std::float32_t div032 = max32 / 0.0f32; // { dg-error "is not a constant expression" "" { target float32 } }
constexpr std::float32_t zero232 = min32 * min32;
constexpr bool nancmp32 = nan32 < 0.0f32;
constexpr std::float32_t snan232 = snan32 * 2.0f32;
constexpr std::float32_t div0232 = 1.0f32 / 0.0f32; // { dg-error "is not a constant expression" "" { target float32 } }
constexpr std::float32_t div0332 = 0.0f32 / 0.0f32; // { dg-error "is not a constant expression" "" { target float32 } }
#endif

#ifdef __STDCPP_FLOAT64_T__
constexpr std::float64_t min64 = std::numeric_limits<std::float64_t>::min ();
constexpr std::float64_t max64 = std::numeric_limits<std::float64_t>::max ();
constexpr std::float64_t inf64 = std::numeric_limits<std::float64_t>::infinity ();
constexpr std::float64_t nan64 = std::numeric_limits<std::float64_t>::quiet_NaN ();
constexpr std::float64_t snan64 = std::numeric_limits<std::float64_t>::signaling_NaN ();

// Furthermore, if arithmetic expressions with operands of type std::float64_t
// behave as specified in ISO/IEC 60559 for binary64:
constexpr std::float64_t inf264 = inf64 * 2.0f64; // OK, also positive infinity
constexpr std::float64_t zero64 = min64 / max64; // OK, result cannot be represented, and is rounded to zero
constexpr std::float64_t oflo64 = max64 * 2.0f64; // { dg-error "is not a constant expression" "" { target float64 } }
constexpr std::float64_t nan264 = nan64 * 2.0f64; // OK, propagating a NaN
constexpr std::float64_t udef64 = inf64 * 0.0f64; // { dg-error "is not a constant expression" "" { target float64 } }
constexpr std::float64_t div064 = max64 / 0.0f64; // { dg-error "is not a constant expression" "" { target float64 } }
constexpr std::float64_t zero264 = min64 * min64;
constexpr bool nancmp64 = nan64 < 0.0f64;
constexpr std::float64_t snan264 = snan64 * 2.0f64;
constexpr std::float64_t div0264 = 1.0f64 / 0.0f64; // { dg-error "is not a constant expression" "" { target float64 } }
constexpr std::float64_t div0364 = 0.0f64 / 0.0f64; // { dg-error "is not a constant expression" "" { target float64 } }
#endif

#ifdef __STDCPP_FLOAT128_T__
constexpr std::float128_t min128 = std::numeric_limits<std::float128_t>::min ();
constexpr std::float128_t max128 = std::numeric_limits<std::float128_t>::max ();
constexpr std::float128_t inf128 = std::numeric_limits<std::float128_t>::infinity ();
constexpr std::float128_t nan128 = std::numeric_limits<std::float128_t>::quiet_NaN ();
constexpr std::float128_t snan128 = std::numeric_limits<std::float128_t>::signaling_NaN ();

// Furthermore, if arithmetic expressions with operands of type std::float128_t
// behave as specified in ISO/IEC 60559 for binary128:
constexpr std::float128_t inf2128 = inf128 * 2.0f128; // OK, also positive infinity
constexpr std::float128_t zero128 = min128 / max128; // OK, result cannot be represented, and is rounded to zero
constexpr std::float128_t oflo128 = max128 * 2.0f128; // { dg-error "is not a constant expression" "" { target float128 } }
constexpr std::float128_t nan2128 = nan128 * 2.0f128; // OK, propagating a NaN
constexpr std::float128_t udef128 = inf128 * 0.0f128; // { dg-error "is not a constant expression" "" { target float128 } }
constexpr std::float128_t div0128 = max128 / 0.0f128; // { dg-error "is not a constant expression" "" { target float128 } }
constexpr std::float128_t zero2128 = min128 * min128;
constexpr bool nancmp128 = nan128 < 0.0f128;
constexpr std::float128_t snan2128 = snan128 * 2.0f128;
constexpr std::float128_t div02128 = 1.0f128 / 0.0f128; // { dg-error "is not a constant expression" "" { target float128 } }
constexpr std::float128_t div03128 = 0.0f128 / 0.0f128; // { dg-error "is not a constant expression" "" { target float128 } }
#endif

#ifdef __STDCPP_BFLOAT16_T__
constexpr std::bfloat16_t minb16 = std::numeric_limits<std::bfloat16_t>::min ();
constexpr std::bfloat16_t maxb16 = std::numeric_limits<std::bfloat16_t>::max ();
constexpr std::bfloat16_t infb16 = std::numeric_limits<std::bfloat16_t>::infinity ();
constexpr std::bfloat16_t nanb16 = std::numeric_limits<std::bfloat16_t>::quiet_NaN ();
constexpr std::bfloat16_t snanb16 = std::numeric_limits<std::bfloat16_t>::signaling_NaN ();

constexpr std::bfloat16_t inf2b16 = infb16 * 2.0bf16; // OK, also positive infinity
constexpr std::bfloat16_t zerob16 = minb16 / maxb16; // OK, result cannot be represented, and is rounded to zero
constexpr std::bfloat16_t oflob16 = maxb16 * 2.0bf16; // { dg-error "is not a constant expression" "" { target bfloat16 } }
constexpr std::bfloat16_t nan2b16 = nanb16 * 2.0bf16; // OK, propagating a NaN
constexpr std::bfloat16_t udefb16 = infb16 * 0.0bf16; // { dg-error "is not a constant expression" "" { target bfloat16 } }
constexpr std::bfloat16_t div0b16 = maxb16 / 0.0bf16; // { dg-error "is not a constant expression" "" { target bfloat16 } }
constexpr std::bfloat16_t zero2b16 = minb16 * minb16;
constexpr bool nancmpb16 = nanb16 < 0.0bf16;
constexpr std::bfloat16_t snan2b16 = snanb16 * 2.0bf16;
constexpr std::bfloat16_t div02b16 = 1.0bf16 / 0.0bf16; // { dg-error "is not a constant expression" "" { target bfloat16 } }
constexpr std::bfloat16_t div03b16 = 0.0bf16 / 0.0bf16; // { dg-error "is not a constant expression" "" { target bfloat16 } }
#endif

constexpr int div0 = 0 / 0; // { dg-error "division by zero is not a constant expression" }
// { dg-warning "division by zero" "" { target *-*-* } .-1 }

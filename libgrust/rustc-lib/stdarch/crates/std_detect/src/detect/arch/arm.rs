//! Run-time feature detection on ARM Aarch32.

features! {
    @TARGET: arm;
    @MACRO_NAME: is_arm_feature_detected;
    @MACRO_ATTRS:
    /// Checks if `arm` feature is enabled.
    #[unstable(feature = "stdsimd", issue = "27731")]
    @NO_RUNTIME_DETECTION: "v7";
    @NO_RUNTIME_DETECTION: "vfp2";
    @NO_RUNTIME_DETECTION: "vfp3";
    @NO_RUNTIME_DETECTION: "vfp4";
    @FEATURE: #[unstable(feature = "stdsimd", issue = "27731")] neon: "neon";
    /// ARM Advanced SIMD (NEON) - Aarch32
    @FEATURE: #[unstable(feature = "stdsimd", issue = "27731")] pmull: "pmull";
    /// Polynomial Multiply
    @FEATURE: #[unstable(feature = "stdsimd", issue = "27731")] crc: "crc";
    /// CRC32 (Cyclic Redundancy Check)
    @FEATURE: #[unstable(feature = "stdsimd", issue = "27731")] crypto: "crypto";
    /// Crypto: AES + PMULL + SHA1 + SHA2
}

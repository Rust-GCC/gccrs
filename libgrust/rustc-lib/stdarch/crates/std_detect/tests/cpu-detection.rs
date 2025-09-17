#![feature(stdsimd)]
#![allow(clippy::option_unwrap_used, clippy::use_debug, clippy::print_stdout)]
#![cfg(any(
    target_arch = "arm",
    target_arch = "aarch64",
    target_arch = "x86",
    target_arch = "x86_64",
    target_arch = "powerpc",
    target_arch = "powerpc64"
))]

#[macro_use]
extern crate std_detect;

#[test]
fn all() {
    for (f, e) in std_detect::detect::features() {
        println!("{}: {}", f, e);
    }
}

#[test]
#[cfg(all(target_arch = "arm", any(target_os = "linux", target_os = "android")))]
fn arm_linux() {
    println!("neon: {}", is_arm_feature_detected!("neon"));
    println!("pmull: {}", is_arm_feature_detected!("pmull"));
    println!("crc: {}", is_arm_feature_detected!("crc"));
    println!("crypto: {}", is_arm_feature_detected!("crypto"));
}

#[test]
#[cfg(all(
    target_arch = "aarch64",
    any(target_os = "linux", target_os = "android")
))]
fn aarch64_linux() {
    println!("fp: {}", is_aarch64_feature_detected!("fp"));
    println!("fp16: {}", is_aarch64_feature_detected!("fp16"));
    println!("neon: {}", is_aarch64_feature_detected!("neon"));
    println!("asimd: {}", is_aarch64_feature_detected!("asimd"));
    println!("sve: {}", is_aarch64_feature_detected!("sve"));
    println!("crc: {}", is_aarch64_feature_detected!("crc"));
    println!("crypto: {}", is_aarch64_feature_detected!("crypto"));
    println!("lse: {}", is_aarch64_feature_detected!("lse"));
    println!("rdm: {}", is_aarch64_feature_detected!("rdm"));
    println!("rcpc: {}", is_aarch64_feature_detected!("rcpc"));
    println!("dotprod: {}", is_aarch64_feature_detected!("dotprod"));
    println!("tme: {}", is_aarch64_feature_detected!("tme"));
}

#[test]
#[cfg(all(target_arch = "powerpc", target_os = "linux"))]
fn powerpc_linux() {
    println!("altivec: {}", is_powerpc_feature_detected!("altivec"));
    println!("vsx: {}", is_powerpc_feature_detected!("vsx"));
    println!("power8: {}", is_powerpc_feature_detected!("power8"));
}

#[test]
#[cfg(all(target_arch = "powerpc64", target_os = "linux"))]
fn powerpc64_linux() {
    println!("altivec: {}", is_powerpc64_feature_detected!("altivec"));
    println!("vsx: {}", is_powerpc64_feature_detected!("vsx"));
    println!("power8: {}", is_powerpc64_feature_detected!("power8"));
}

#[test]
#[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
fn x86_all() {
    println!("aes: {:?}", is_x86_feature_detected!("aes"));
    println!("pcmulqdq: {:?}", is_x86_feature_detected!("pclmulqdq"));
    println!("rdrand: {:?}", is_x86_feature_detected!("rdrand"));
    println!("rdseed: {:?}", is_x86_feature_detected!("rdseed"));
    println!("tsc: {:?}", is_x86_feature_detected!("tsc"));
    println!("mmx: {:?}", is_x86_feature_detected!("mmx"));
    println!("sse: {:?}", is_x86_feature_detected!("sse"));
    println!("sse2: {:?}", is_x86_feature_detected!("sse2"));
    println!("sse3: {:?}", is_x86_feature_detected!("sse3"));
    println!("ssse3: {:?}", is_x86_feature_detected!("ssse3"));
    println!("sse4.1: {:?}", is_x86_feature_detected!("sse4.1"));
    println!("sse4.2: {:?}", is_x86_feature_detected!("sse4.2"));
    println!("sse4a: {:?}", is_x86_feature_detected!("sse4a"));
    println!("sha: {:?}", is_x86_feature_detected!("sha"));
    println!("avx: {:?}", is_x86_feature_detected!("avx"));
    println!("avx2: {:?}", is_x86_feature_detected!("avx2"));
    println!("avx512f: {:?}", is_x86_feature_detected!("avx512f"));
    println!("avx512cd: {:?}", is_x86_feature_detected!("avx512cd"));
    println!("avx512er: {:?}", is_x86_feature_detected!("avx512er"));
    println!("avx512pf: {:?}", is_x86_feature_detected!("avx512pf"));
    println!("avx512bw: {:?}", is_x86_feature_detected!("avx512bw"));
    println!("avx512dq: {:?}", is_x86_feature_detected!("avx512dq"));
    println!("avx512vl: {:?}", is_x86_feature_detected!("avx512vl"));
    println!("avx512ifma: {:?}", is_x86_feature_detected!("avx512ifma"));
    println!("avx512vbmi: {:?}", is_x86_feature_detected!("avx512vbmi"));
    println!(
        "avx512vpopcntdq: {:?}",
        is_x86_feature_detected!("avx512vpopcntdq")
    );
    println!("avx512vbmi2 {:?}", is_x86_feature_detected!("avx512vbmi2"));
    println!("avx512gfni {:?}", is_x86_feature_detected!("avx512gfni"));
    println!("avx512vaes {:?}", is_x86_feature_detected!("avx512vaes"));
    println!(
        "avx512vpclmulqdq {:?}",
        is_x86_feature_detected!("avx512vpclmulqdq")
    );
    println!("avx512vnni {:?}", is_x86_feature_detected!("avx512vnni"));
    println!(
        "avx512bitalg {:?}",
        is_x86_feature_detected!("avx512bitalg")
    );
    println!("avx512bf16 {:?}", is_x86_feature_detected!("avx512bf16"));
    println!(
        "avx512vp2intersect {:?}",
        is_x86_feature_detected!("avx512vp2intersect")
    );
    println!("f16c: {:?}", is_x86_feature_detected!("f16c"));
    println!("fma: {:?}", is_x86_feature_detected!("fma"));
    println!("bmi1: {:?}", is_x86_feature_detected!("bmi1"));
    println!("bmi2: {:?}", is_x86_feature_detected!("bmi2"));
    println!("abm: {:?}", is_x86_feature_detected!("abm"));
    println!("lzcnt: {:?}", is_x86_feature_detected!("lzcnt"));
    println!("tbm: {:?}", is_x86_feature_detected!("tbm"));
    println!("popcnt: {:?}", is_x86_feature_detected!("popcnt"));
    println!("fxsr: {:?}", is_x86_feature_detected!("fxsr"));
    println!("xsave: {:?}", is_x86_feature_detected!("xsave"));
    println!("xsaveopt: {:?}", is_x86_feature_detected!("xsaveopt"));
    println!("xsaves: {:?}", is_x86_feature_detected!("xsaves"));
    println!("xsavec: {:?}", is_x86_feature_detected!("xsavec"));
}

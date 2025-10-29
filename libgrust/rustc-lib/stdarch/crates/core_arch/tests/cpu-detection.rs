#![feature(stdsimd)]
#![allow(clippy::option_unwrap_used, clippy::print_stdout, clippy::use_debug)]

#[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
#[macro_use]
extern crate std_detect;

#[test]
#[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
fn x86_all() {
    println!("sse: {:?}", is_x86_feature_detected!("sse"));
    println!("sse2: {:?}", is_x86_feature_detected!("sse2"));
    println!("sse3: {:?}", is_x86_feature_detected!("sse3"));
    println!("ssse3: {:?}", is_x86_feature_detected!("ssse3"));
    println!("sse4.1: {:?}", is_x86_feature_detected!("sse4.1"));
    println!("sse4.2: {:?}", is_x86_feature_detected!("sse4.2"));
    println!("sse4a: {:?}", is_x86_feature_detected!("sse4a"));
    println!("avx: {:?}", is_x86_feature_detected!("avx"));
    println!("avx2: {:?}", is_x86_feature_detected!("avx2"));
    println!("avx512f {:?}", is_x86_feature_detected!("avx512f"));
    println!("avx512cd {:?}", is_x86_feature_detected!("avx512cd"));
    println!("avx512er {:?}", is_x86_feature_detected!("avx512er"));
    println!("avx512pf {:?}", is_x86_feature_detected!("avx512pf"));
    println!("avx512bw {:?}", is_x86_feature_detected!("avx512bw"));
    println!("avx512dq {:?}", is_x86_feature_detected!("avx512dq"));
    println!("avx512vl {:?}", is_x86_feature_detected!("avx512vl"));
    println!("avx512_ifma {:?}", is_x86_feature_detected!("avx512ifma"));
    println!("avx512_vbmi {:?}", is_x86_feature_detected!("avx512vbmi"));
    println!(
        "avx512_vpopcntdq {:?}",
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
    println!("abm: {:?}", is_x86_feature_detected!("abm"));
    println!("bmi: {:?}", is_x86_feature_detected!("bmi1"));
    println!("bmi2: {:?}", is_x86_feature_detected!("bmi2"));
    println!("tbm: {:?}", is_x86_feature_detected!("tbm"));
    println!("popcnt: {:?}", is_x86_feature_detected!("popcnt"));
    println!("lzcnt: {:?}", is_x86_feature_detected!("lzcnt"));
    println!("fxsr: {:?}", is_x86_feature_detected!("fxsr"));
    println!("xsave: {:?}", is_x86_feature_detected!("xsave"));
    println!("xsaveopt: {:?}", is_x86_feature_detected!("xsaveopt"));
    println!("xsaves: {:?}", is_x86_feature_detected!("xsaves"));
    println!("xsavec: {:?}", is_x86_feature_detected!("xsavec"));
}

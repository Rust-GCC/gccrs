#![feature(no_core)]
#![no_core]

pub fn f() {
    let _ = 0.;
    let _ = 159.;
    let _ = 0.0;
    let _ = 159.0;
    let _ = 0.0f32;
    let _ = 159.0f32;
    let _ = 0_0.0f32;
    let _ = 0_159.0f32;
}

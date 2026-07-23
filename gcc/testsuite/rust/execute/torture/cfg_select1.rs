// { dg-output "wildcard\r*\n" }
#![feature(no_core, rustc_attrs)]
#![no_core]

#[rustc_builtin_macro]
macro_rules! cfg_select {
    () => {{}};
}

extern "C" {
    fn printf(s: *const i8, ...);
}

fn main() -> i32 {
    cfg_select!
    {
        target_os = "none" => { // assuming gccrs tests will never be ran on an embedded target with `target_os = "none"` lol
            unsafe {
                let a = "none\n\0";
                printf(a as *const str as *const i8);
            }
        },
        _ => {
            unsafe {
                let a = "wildcard\n\0";
                printf(a as *const str as *const i8);
            }
        }
    }
    return 0;
}
#![feature(rustc_attrs)]

#[rustc_builtin_macro]
macro_rules! option_env {
    () => {}
}

#[lang = "sized"]
trait Sized {}

mod core {
    mod option {
        enum Option<T> {
            Some(T),
            None,
        }
    }
}

use core::option::Option;

fn main() {
    let _: Option<&str> = option_env!(42);
    // { dg-error "argument must be a string literal" "" { target *-*-* } .-1 }
}

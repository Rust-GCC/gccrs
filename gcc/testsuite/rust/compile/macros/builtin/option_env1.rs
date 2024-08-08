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
    // Both a guaranteed-to-exist variable and a failed find should compile
    let _: Option<&str> = option_env!("PWD");
    let _: Option<&str> = option_env!("PROBABLY_DOESNT_EXIST");
}

#![feature(lang_items)]
#![feature(no_core)]
#![no_core]

macro_rules! assert {
    ($cond:expr $(,)?) => {{ /* compiler built-in */ }};
    ($cond:expr, $($arg:tt)+) => {{ /* compiler built-in */ }};
}

#[lang = "sized"]
trait Sized {}

fn _test() {
    let _: () = assert!(true);
    let _: () = assert!(true);
}

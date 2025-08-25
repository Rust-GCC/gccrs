#![forbid(unsafe_code)]
#![feature(allow_internal_unsafe)]

#[allow_internal_unsafe]
// { dg-error "" "" { target *-*-* } .-1 }
macro_rules! evil {
    ($e:expr) => {
        unsafe {
            $e
        }
    }
}

fn main() {
    println!("{}", evil!(*(0 as *const u8)));
}


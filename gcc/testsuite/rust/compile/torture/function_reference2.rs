#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}

fn test(a: i32) -> i32 {
    a + 1
}

fn main() {
    let a: fn(i32) -> i32 = test;
    let b = a(1);
    // { dg-warning "unused name" "" { target *-*-* } .-1 }
}

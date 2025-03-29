#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}


struct Foo {
    a: fn(i32) -> i32,
    b: i32,
}

fn test(a: i32) -> i32 {
    a + 1
}

fn main() {
    let a = test(1);
    // { dg-warning "unused name" "" { target *-*-* } .-1 }

    let b: fn(i32) -> i32 = test;
    let c = b(1);

    let d = Foo { a: test, b: c };
    let e = (d.a)(d.b);
    // { dg-warning "unused name" "" { target *-*-* } .-1 }
}

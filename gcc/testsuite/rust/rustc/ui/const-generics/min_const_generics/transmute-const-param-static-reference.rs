#![feature(min_const_generics)]

struct Const<const P: &'static ()>;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    const A: &'static () = unsafe {
        std::mem::transmute(10 as *const ())
    };

    let _ = Const::<{A}>;
}


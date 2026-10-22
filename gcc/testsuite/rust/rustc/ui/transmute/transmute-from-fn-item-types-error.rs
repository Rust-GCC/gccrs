use std::mem;

unsafe fn foo() -> (i8, *const (), Option<fn()>) {
    let i = mem::transmute(bar);
// { dg-error ".E0512." "" { target *-*-* } .-1 }


    let p = mem::transmute(foo);
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    let of = mem::transmute(main);
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    (i, p, of)
}

unsafe fn bar() {
    // Error as usual if the resulting type is not pointer-sized.
    mem::transmute::<_, u8>(main);
// { dg-error ".E0512." "" { target *-*-* } .-1 }


    mem::transmute::<_, *mut ()>(foo);
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    mem::transmute::<_, fn()>(bar);
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    // No error if a coercion would otherwise occur.
    mem::transmute::<fn(), usize>(main);
}

unsafe fn baz() {
    mem::transmute::<_, *mut ()>(Some(foo));
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    mem::transmute::<_, fn()>(Some(bar));
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    mem::transmute::<_, Option<fn()>>(Some(baz));
// { dg-error ".E0591." "" { target *-*-* } .-1 }


    // No error if a coercion would otherwise occur.
    mem::transmute::<Option<fn()>, usize>(Some(main));
}

fn main() {
    unsafe {
        foo();
        bar();
        baz();
    }
}


#![feature(box_syntax)]

fn needs_fn<F>(x: F) where F: Fn(isize) -> isize {}

fn main() {
    let _: () = (box |_: isize| {}) as Box<dyn FnOnce(isize)>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    let _: () = (box |_: isize, isize| {}) as Box<dyn Fn(isize, isize)>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    let _: () = (box || -> isize { unimplemented!() }) as Box<dyn FnMut() -> isize>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

    needs_fn(1);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}


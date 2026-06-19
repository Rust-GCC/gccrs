// Test `@` patterns combined with `box` patterns.

#![feature(bindings_after_at)]
#![feature(box_patterns)]

#[derive(Copy, Clone)]
struct C;

fn c() -> C {
    C
}

struct NC;

fn nc() -> NC {
    NC
}

fn main() {
    let a @ box &b = Box::new(&C);

    let a @ box b = Box::new(C);

    fn f1(a @ box &b: Box<&C>) {}

    fn f2(a @ box b: Box<C>) {}

    match Box::new(C) {
        a @ box b => {}
    }

    let ref a @ box b = Box::new(NC); // { dg-error ".E0382." "" { target *-*-* } }
// { dg-error ".E0382." "" { target *-*-* } .-2 }

    let ref a @ box ref mut b = Box::new(nc());
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let ref a @ box ref mut b = Box::new(NC);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    let ref a @ box ref mut b = Box::new(NC);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *b = NC;
    let ref a @ box ref mut b = Box::new(NC);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *b = NC;
    drop(a);

    let ref mut a @ box ref b = Box::new(NC);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
    *a = Box::new(NC);
    drop(b);

    fn f5(ref mut a @ box ref b: Box<NC>) {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
        *a = Box::new(NC);
        drop(b);
    }

    match Box::new(nc()) {
        ref mut a @ box ref b => {
// { dg-error ".E0502." "" { target *-*-* } .-1 }
// { dg-error ".E0502." "" { target *-*-* } .-2 }
            *a = Box::new(NC);
            drop(b);
        }
    }
}


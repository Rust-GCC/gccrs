#![feature(unboxed_closures)]

// Tests that we can't assign to or mutably borrow upvars from `Fn`
// closures (issue #17780)

fn set(x: &mut usize) {
    *x = 5;
}

fn to_fn<A, F: Fn<A>>(f: F) -> F {
    f
}
fn to_fn_mut<A, F: FnMut<A>>(f: F) -> F {
    f
}

fn main() {
    // By-ref captures
    {
        let mut x = 0;
        let _f = to_fn(|| x = 42); // { dg-error ".E0594." "" { target *-*-* } }

        let mut y = 0;
        let _g = to_fn(|| set(&mut y)); // { dg-error ".E0596." "" { target *-*-* } }

        let mut z = 0;
        let _h = to_fn_mut(|| {
            set(&mut z);
            to_fn(|| z = 42); // { dg-error ".E0594." "" { target *-*-* } }
        });
    }

    // By-value captures
    {
        let mut x = 0;
        let _f = to_fn(move || x = 42); // { dg-error ".E0594." "" { target *-*-* } }

        let mut y = 0;
        let _g = to_fn(move || set(&mut y)); // { dg-error ".E0596." "" { target *-*-* } }

        let mut z = 0;
        let _h = to_fn_mut(move || {
            set(&mut z);
            to_fn(move || z = 42);
// { dg-error ".E0594." "" { target *-*-* } .-1 }
        });
    }
}

fn foo() -> Box<dyn Fn() -> usize> {
    let mut x = 0;
    Box::new(move || {
        x += 1; // { dg-error ".E0594." "" { target *-*-* } }
        x
    })
}


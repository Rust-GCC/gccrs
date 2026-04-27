// edition:2018
// check-pass
#![warn(unused)]
#![allow(unreachable_code)]

pub fn unintentional_copy_one() {
    let mut last = None;
    let mut f = move |s| {
        last = Some(s); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
    };
    f("a");
    f("b");
    f("c");
    dbg!(last.unwrap());
}

pub fn unintentional_copy_two() {
    let mut sum = 0;
    (1..10).for_each(move |x| {
        sum += x; // { dg-warning "" "" { target *-*-* } }
    });
    dbg!(sum);
}

pub fn f() {
    let mut c = 0;

    // Captured by value, but variable is dead on entry.
    let _ = move || {
        c = 1; // { dg-warning "" "" { target *-*-* } }
        println!("{}", c);
    };
    let _ = async move {
        c = 1; // { dg-warning "" "" { target *-*-* } }
        println!("{}", c);
    };

    // Read and written to, but never actually used.
    let _ = move || {
        c += 1; // { dg-warning "" "" { target *-*-* } }
    };
    let _ = async move {
        c += 1; // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
    };

    let _ = move || {
        println!("{}", c);
        // Value is read by closure itself on later invocations.
        c += 1;
    };
    let b = Box::new(42);
    let _ = move || {
        println!("{}", c);
        // Never read because this is FnOnce closure.
        c += 1; // { dg-warning "" "" { target *-*-* } }
        drop(b);
    };
    let _ = async move {
        println!("{}", c);
        // Never read because this is a generator.
        c += 1; // { dg-warning "" "" { target *-*-* } }
    };
}

pub fn nested() {
    let mut d = None;
    let mut e = None;
    let _ = || {
        let _ = || {
            d = Some("d1"); // { dg-warning "" "" { target *-*-* } }
            d = Some("d2");
        };
        let _ = move || {
            e = Some("e1"); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
            e = Some("e2"); // { dg-warning "" "" { target *-*-* } }
        };
    };
}

pub fn g<T: Default>(mut v: T) {
    let _ = |r| {
        if r {
            v = T::default(); // { dg-warning "" "" { target *-*-* } }
        } else {
            drop(v);
        }
    };
}

pub fn h<T: Copy + Default + std::fmt::Debug>() {
    let mut z = T::default();
    let _ = move |b| {
        loop {
            if b {
                z = T::default(); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }
            } else {
                return;
            }
        }
        dbg!(z);
    };
}

fn main() {}


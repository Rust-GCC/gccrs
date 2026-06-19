// Regression test for #68641

#![feature(generic_associated_types)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait UnsafeCopy {
    type Item<'a>: Copy;

    fn copy<'a>(item: &Self::Item<'a>) -> Self::Item<'a> {
        *item
    }
}

impl<T> UnsafeCopy for T {
    type Item<'a> = T;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    let mut s = String::from("Hello world!");

    let copy = String::copy(&s);

    // Do we indeed point to the samme memory?
    assert!(s.as_ptr() == copy.as_ptr());

    // Any use of `copy` is certeinly UB after this
    drop(s);

    // UB UB UB UB UB!!
    println!("{}", copy);
}


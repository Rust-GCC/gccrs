// Several variants of #64945.

// This struct is not important, we just use it to put `T` and `'a` in scope for our associated
// consts.
struct Generic<'a, T>(std::marker::PhantomData<&'a T>);

impl<'a, T: 'static> Generic<'a, T> {
    const EMPTY_SLICE: &'a [T] = {
        let x: &'a [T] = &[];
        x
    };

    const EMPTY_SLICE_REF: &'a &'static [T] = {
        let x: &'static [T] = &[];
        &x
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    };
}

static mut INTERIOR_MUT_AND_DROP: &'static [std::cell::RefCell<Vec<i32>>] = {
    let x: &[_] = &[];
    x
};

static mut INTERIOR_MUT_AND_DROP_REF: &'static &'static [std::cell::RefCell<Vec<i32>>] = {
    let x: &[_] = &[];
    &x
// { dg-error ".E0597." "" { target *-*-* } .-1 }
};

fn main() {}


//! Make sure we error on erroneous consts even if they are unused.
#![warn(const_err, unconditional_panic)]

struct PrintName<T>(T);
impl<T> PrintName<T> {
    const VOID: () = [()][2]; // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
}

const fn no_codegen<T>() {
    if false {
        let _ = PrintName::<T>::VOID; // { dg-error ".E0080." "" { target *-*-* } }
    }
}

pub static FOO: () = no_codegen::<i32>(); // { dg-error ".E0080." "" { target *-*-* } }

fn main() {
    FOO
}


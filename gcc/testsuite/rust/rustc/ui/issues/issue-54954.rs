#![feature(const_fn)]

const ARR_LEN: usize = Tt::const_val::<[i8; 123]>();
// { dg-error ".E0283." "" { target *-*-* } .-1 }

trait Tt {
    const fn const_val<T: Sized>() -> usize {
// { dg-error ".E0379." "" { target *-*-* } .-1 }
        core::mem::size_of::<T>()
    }
}

fn f(z: [f32; ARR_LEN]) -> [f32; ARR_LEN] {
    z
}

fn main() {
    let _ = f([1f32; ARR_LEN]);
}


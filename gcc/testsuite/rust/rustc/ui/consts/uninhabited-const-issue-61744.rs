// build-fail

pub const unsafe fn fake_type<T>() -> T {
    hint_unreachable()
}

pub const unsafe fn hint_unreachable() -> ! {
    fake_type() // { dg-error ".E0080." "" { target *-*-* } }
}

trait Const {
    const CONSTANT: i32 = unsafe { fake_type() }; // { dg-error "" "" { target *-*-* } }
}

impl<T> Const for T {}

pub fn main() -> () {
    dbg!(i32::CONSTANT); // { dg-error ".E0080." "" { target *-*-* } }
}


#![feature(const_fn_union)]

fn main() {
    let n: Int = 40;
    match n {
        0..=10 => {},
        10..=BAR => {}, // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => {},
    }
}

#[repr(C)]
union Foo {
    f: Int,
    r: &'static u32,
}

#[cfg(target_pointer_width="64")]
type Int = u64;

#[cfg(target_pointer_width="32")]
type Int = u32;

const BAR: Int = unsafe { Foo { r: &42 }.f }; // { dg-error ".E0080." "" { target *-*-* } }


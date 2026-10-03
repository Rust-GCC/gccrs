// revisions: full min
#![cfg_attr(full, feature(const_generics))] // { dg-warning "" "" { target *-*-* } }
#![cfg_attr(min, feature(min_const_generics))]

fn f<T: Copy, const N: usize>(x: T) -> [T; N] {
    [x; N]
}

fn g<T, const N: usize>(x: T) -> [T; N] {
    [x; N]
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    let x: [u32; 5] = f::<u32, 5>(3);
    assert_eq!(x, [3u32; 5]);
}


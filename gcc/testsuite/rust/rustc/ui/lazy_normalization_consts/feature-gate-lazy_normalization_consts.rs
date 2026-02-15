pub const fn sof<T>() -> usize {
    10
}

fn test<T>() {
    let _: [u8; sof::<T>()];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}


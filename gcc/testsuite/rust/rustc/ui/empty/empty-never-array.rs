#![feature(never_type)]

enum Helper<T, U> {
    T(T, [!; 0]),
    #[allow(dead_code)]
    U(U),
}

fn transmute<T, U>(t: T) -> U {
    let Helper::U(u) = Helper::T(t, []);
// { dg-error ".E0005." "" { target *-*-* } .-1 }
    u
}

fn main() {
    println!("{:?}", transmute::<&str, (*const u8, u64)>("type safety"));
}


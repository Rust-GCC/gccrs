enum T {
    T1(i32, i32),
    T2(i32, i32),
    T3(i32),
    T4(i32),
}

fn main() {
    match T::T1(123, 456) {
        T::T1(a, d) | T::T2(d, b) | T::T3(c) | T::T4(a) => { println!("{:?}", a); }
// { dg-error ".E0408." "" { target *-*-* } .-1 }
// { dg-error ".E0408." "" { target *-*-* } .-2 }
// { dg-error ".E0408." "" { target *-*-* } .-3 }
// { dg-error ".E0408." "" { target *-*-* } .-4 }
    }
}


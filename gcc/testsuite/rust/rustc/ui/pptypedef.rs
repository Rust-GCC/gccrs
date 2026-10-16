fn let_in<T, F>(x: T, f: F) where F: FnOnce(T) {}

fn main() {
    let_in(3u32, |i| { assert!(i == 3i32); });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    let_in(3i32, |i| { assert!(i == 3u32); });
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}


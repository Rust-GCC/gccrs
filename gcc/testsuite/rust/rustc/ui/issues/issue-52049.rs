fn foo(_: &'static u32) {}

fn unpromotable<T>(t: T) -> T { t }

fn main() {
    foo(&unpromotable(5u32));
}
// { dg-error ".E0716." "" { target *-*-* } .-2 }


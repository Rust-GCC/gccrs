fn foo<T>() {
    fn bar(b: T) { } // { dg-error ".E0401." "" { target *-*-* } }
}
fn main() { }


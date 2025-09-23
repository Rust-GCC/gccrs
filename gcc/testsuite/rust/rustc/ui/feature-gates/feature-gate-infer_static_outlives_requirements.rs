// Needs an explicit where clause stating outlives condition. (RFC 2093)

// Type T needs to outlive lifetime 'static.
struct Foo<U> {
    bar: Bar<U> // { dg-error ".E0310." "" { target *-*-* } }
}
struct Bar<T: 'static> {
    x: T,
}


fn main() { }


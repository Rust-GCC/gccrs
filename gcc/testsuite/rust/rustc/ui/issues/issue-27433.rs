fn main() {
    let foo = 42u32;
    const FOO : u32 = foo;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
}


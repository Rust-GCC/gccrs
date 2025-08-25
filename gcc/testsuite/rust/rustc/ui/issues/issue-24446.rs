fn main() {
    static foo: dyn Fn() -> u32 = || -> u32 {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        0
    };
}


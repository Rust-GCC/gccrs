// A few contrived examples where lifetime should (or should not) be parsed as an object type.
// Lifetimes parsed as types are still rejected later by semantic checks.

struct S<'a, T>(&'a u8, T);

fn main() {
    // `'static` is a lifetime argument, `'static +` is a type argument
    let _: S<'static, u8>;
    let _: S<'static, dyn 'static +>;
// { dg-error ".E0224." "" { target *-*-* } .-1 }
    let _: S<'static, 'static>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
    let _: S<dyn 'static +, 'static>;
// { dg-error ".E0747." "" { target *-*-* } .-1 }
// { dg-error ".E0747." "" { target *-*-* } .-2 }
}


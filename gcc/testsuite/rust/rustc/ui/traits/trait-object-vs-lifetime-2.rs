// A few contrived examples where lifetime should (or should not) be parsed as an object type.
// Lifetimes parsed as types are still rejected later by semantic checks.

// `'static` is a lifetime, `'static +` is a type, `'a` is a type
fn g() where
    'static: 'static,
    dyn 'static +: 'static + Copy,
// { dg-error ".E0224." "" { target *-*-* } .-1 }
{}

fn main() {}


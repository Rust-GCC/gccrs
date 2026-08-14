// Ensure that capturing closures are never coerced to fns
// Especially interesting as non-capturing closures can be.

fn main() {
    let b = 0u8;
    let bar: fn() -> u8 = || { b };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}


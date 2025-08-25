// Syntactically, a `const` item inside an `extern { ... }` block is not allowed.

fn main() {}

#[cfg(FALSE)]
extern {
    const A: isize; // { dg-error "" "" { target *-*-* } }
    const B: isize = 42; // { dg-error "" "" { target *-*-* } }
}


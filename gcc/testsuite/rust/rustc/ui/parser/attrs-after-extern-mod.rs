// Make sure there's an error when given `extern { ... #[attr] }`.

fn main() {}

extern {
    #[cfg(stage37)] // { dg-error "" "" { target *-*-* } }
}


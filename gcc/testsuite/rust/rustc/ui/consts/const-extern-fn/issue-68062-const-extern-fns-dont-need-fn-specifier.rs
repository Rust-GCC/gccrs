fn main() {}

#[cfg(FALSE)]
fn container() {
    const extern "Rust" PUT_ANYTHING_YOU_WANT_HERE bug() -> usize { 1 }
// { dg-error "" "" { target *-*-* } .-1 }
}


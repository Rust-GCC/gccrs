// revisions: a

// Counterpart to `expected-error-wrong-rev.rs`

#[cfg(a)]
fn foo() {
    let x: u32 = 22_usize; // { dg-error "" "" { target *-*-* } }
}

fn main() { }


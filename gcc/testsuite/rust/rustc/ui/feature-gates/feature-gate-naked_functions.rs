#[naked]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn naked() {}

#[naked]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn naked_2() -> isize {
    0
}

fn main() {}


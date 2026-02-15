// gate-test-raw_ref_op

macro_rules! is_expr {
    ($e:expr) => {}
}

is_expr!(&raw const a);         // { dg-error ".E0658." "" { target *-*-* } }
is_expr!(&raw mut a);           // { dg-error ".E0658." "" { target *-*-* } }

#[cfg(FALSE)]
fn cfgd_out() {
    let mut a = 0;
    &raw const a;               // { dg-error ".E0658." "" { target *-*-* } }
    &raw mut a;                 // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {
    let mut y = 123;
    let x = &raw const y;       // { dg-error ".E0658." "" { target *-*-* } }
    let x = &raw mut y;         // { dg-error ".E0658." "" { target *-*-* } }
}


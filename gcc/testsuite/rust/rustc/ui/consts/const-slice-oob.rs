#[deny(const_err)]

const FOO: &'static[u32] = &[1, 2, 3];
const BAR: u32 = FOO[5];
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    let _ = BAR;
}


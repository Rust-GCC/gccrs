#[cfg(FALSE)]
fn syntax() {
    bar::<Item = 42>(); // { dg-error "" "" { target *-*-* } }
    bar::<Item = { 42 }>(); // { dg-error "" "" { target *-*-* } }
}

fn main() {}


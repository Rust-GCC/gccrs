#[cfg(FALSE)]
fn syntax() {
    bar::<Item = 'a>(); // { dg-error "" "" { target *-*-* } }
}

fn main() {}


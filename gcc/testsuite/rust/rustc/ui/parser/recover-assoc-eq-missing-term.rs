#[cfg(FALSE)]
fn syntax() {
    bar::<Item =   >(); // { dg-error "" "" { target *-*-* } }
}

fn main() {}


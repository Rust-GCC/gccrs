fn deny_on_arm() {
    match 0 {
        #[deny(unused_variables)]
// { dg-note "" "" { target *-*-* } .-1 }
        y => (),
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

#[deny(unused_variables)]
fn allow_on_arm() {
    match 0 {
        #[allow(unused_variables)]
        y => (), // OK
    }
}

fn main() {}


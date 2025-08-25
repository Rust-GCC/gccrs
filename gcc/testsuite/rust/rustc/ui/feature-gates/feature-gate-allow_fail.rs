// check that #[allow_fail] is feature-gated

#[allow_fail] // { dg-error ".E0658." "" { target *-*-* } }
fn ok_to_fail() {
    assert!(false);
}

fn main() {}


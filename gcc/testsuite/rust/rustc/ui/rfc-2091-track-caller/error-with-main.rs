#[track_caller] // { dg-error "" "" { target *-*-* } }
fn main() {
    panic!("{}: oh no", std::panic::Location::caller());
}


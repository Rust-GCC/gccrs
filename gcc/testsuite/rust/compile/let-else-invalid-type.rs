enum FakeOption {
    Some(i32),
    None,
}

fn main() {
    let FakeOption::Some(a) = FakeOption::None else { FakeOption::Some(15) }; // { dg-error "does not diverge" }
}

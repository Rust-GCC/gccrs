// Regression test for #72911.

pub struct Lint {}

impl Lint {}

pub fn gather_all() -> impl Iterator<Item = Lint> {
// { dg-error ".E0720." "" { target *-*-* } .-1 }
    lint_files().flat_map(|f| gather_from_file(&f))
}

fn gather_from_file(dir_entry: &foo::MissingItem) -> impl Iterator<Item = Lint> {
// { dg-error ".E0433." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn lint_files() -> impl Iterator<Item = foo::MissingItem> {
// { dg-error ".E0433." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn main() {}


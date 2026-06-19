macro_rules! m {
    ( $( any_token $field_rust_type )* ) => {}; // { dg-error "" "" { target *-*-* } }
}

fn main() {
    m!();
}


use prelude::*; // { dg-error ".E0432." "" { target *-*-* } }

mod unresolved_env {
    use env; // { dg-error ".E0432." "" { target *-*-* } }

    include!(concat!(env!("NON_EXISTENT"), "/data.rs"));
// { dg-error "" "" { target *-*-* } .-1 }
}

mod nonexistent_env {
    include!(concat!(env!("NON_EXISTENT"), "/data.rs"));
// { dg-error "" "" { target *-*-* } .-1 }
}

mod erroneous_literal {
    include!(concat!("NON_EXISTENT"suffix, "/data.rs"));
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}


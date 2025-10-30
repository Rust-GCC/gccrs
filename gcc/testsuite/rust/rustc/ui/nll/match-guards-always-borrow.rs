// Here is arielb1's basic example from rust-lang/rust#27282
// that AST borrowck is flummoxed by:

fn should_reject_destructive_mutate_in_guard() {
    match Some(&4) {
        None => {},
        ref mut foo if {
            (|| { let bar = foo; bar.take() })();
// { dg-error ".E0507." "" { target *-*-* } .-1 }
            false } => { },
        Some(s) => std::process::exit(*s),
    }
}

// Here below is a case that needs to keep working: we only use the
// binding via immutable-borrow in the guard, and we mutate in the arm
// body.
fn allow_mutate_in_arm_body() {
    match Some(&4) {
        None => {},
        ref mut foo if foo.is_some() && false => { foo.take(); () }
        Some(s) => std::process::exit(*s),
    }
}

// Here below is a case that needs to keep working: we only use the
// binding via immutable-borrow in the guard, and we move into the arm
// body.
fn allow_move_into_arm_body() {
    match Some(&4) {
        None => {},
        mut foo if foo.is_some() && false => { foo.take(); () }
        Some(s) => std::process::exit(*s),
    }
}

fn main() {
    should_reject_destructive_mutate_in_guard();
    allow_mutate_in_arm_body();
    allow_move_into_arm_body();
}


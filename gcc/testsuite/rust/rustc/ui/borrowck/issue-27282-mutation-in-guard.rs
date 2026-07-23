fn main() {
    match Some(&4) {
        None => {},
        ref mut foo
            if {
                (|| { let bar = foo; bar.take() })();
// { dg-error ".E0507." "" { target *-*-* } .-1 }
                false
            } => {},
        Some(ref _s) => println!("Note this arm is bogus; the `Some` became `None` in the guard."),
        _ => println!("Here is some supposedly unreachable code."),
    }
}


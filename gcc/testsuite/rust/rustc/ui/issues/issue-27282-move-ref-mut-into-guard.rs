// Issue 27282: Example 1: This sidesteps the AST checks disallowing
// mutable borrows in match guards by hiding the mutable borrow in a
// guard behind a move (of the ref mut pattern id) within a closure.

fn main() {
    match Some(&4) {
        None => {},
        ref mut foo
            if { (|| { let bar = foo; bar.take() })(); false } => {},
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        Some(s) => std::process::exit(*s),
    }
}


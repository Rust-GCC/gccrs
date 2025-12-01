enum A {
    Value(())
}

fn main() {
    let a = A::Value(());
    a == A::Value;
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}


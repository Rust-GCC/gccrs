trait Foo {
    fn dummy(&self) { }
}

// This should emit the less confusing error, not the more confusing one.

fn foo(_x: Foo + Send) {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-warning ".E0277." "" { target *-*-* } .-2 }
}

fn main() { }


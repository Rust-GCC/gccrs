fn main() {
    let foo = thing::len(Vec::new());
// { dg-error ".E0433." "" { target *-*-* } .-1 }

    let foo = foo::bar::baz();
// { dg-error ".E0433." "" { target *-*-* } .-1 }
}


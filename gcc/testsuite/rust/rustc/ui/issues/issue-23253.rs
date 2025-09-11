enum Foo { Bar }

fn main() {
    Foo::Bar.a;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}


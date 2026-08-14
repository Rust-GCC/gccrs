mod foo { pub struct Bar; }

fn main() {
    {
        struct Bar;
        use foo::Bar;
// { dg-error ".E0255." "" { target *-*-* } .-1 }
    }
}


// #41719

fn main() {
    enum Foo {}
    let _ = Foo::bar!(); // { dg-error ".E0433." "" { target *-*-* } }
}


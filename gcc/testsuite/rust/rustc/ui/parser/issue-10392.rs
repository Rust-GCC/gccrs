struct A { foo: isize }

fn a() -> A { panic!() }

fn main() {
    let A { , } = a(); // { dg-error "" "" { target *-*-* } }
}


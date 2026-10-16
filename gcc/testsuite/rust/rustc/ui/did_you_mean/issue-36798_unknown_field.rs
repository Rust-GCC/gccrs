struct Foo {
    bar: u8
}

fn main() {
    let f = Foo { bar: 22 };
    f.zz; // { dg-error ".E0609." "" { target *-*-* } }
}


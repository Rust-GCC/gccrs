enum Foo {
    A(i32),
    B
}

fn match_enum() {
    let mut foo = Foo::B;
    let p = &mut foo;
    let _ = match foo {
        Foo::B => 1, // { dg-error ".E0503." "" { target *-*-* } }
        _ => 2,
        Foo::A(x) => x // { dg-error ".E0503." "" { target *-*-* } }
    };
    drop(p);
}


fn main() {
    let mut x = 1;
    let r = &mut x;
    let _ = match x {
        x => x + 1, // { dg-error ".E0503." "" { target *-*-* } }
        y => y + 2, // { dg-error ".E0503." "" { target *-*-* } }
    };
    drop(r);
}


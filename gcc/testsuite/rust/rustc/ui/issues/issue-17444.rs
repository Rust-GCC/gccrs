enum Test {
    Foo = 0
}

fn main() {
    let _x = Test::Foo as *const isize;
// { dg-error ".E0606." "" { target *-*-* } .-1 }
}


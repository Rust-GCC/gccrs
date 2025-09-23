struct Foo;
const INIT: Foo = Foo;
static FOO: Foo = INIT;

fn main() {
    let _a = FOO; // { dg-error ".E0507." "" { target *-*-* } }
}


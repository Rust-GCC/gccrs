struct Foo {
    val: (),
}

fn foo() -> Foo { // { dg-error "" "" { target *-*-* } }
    val: (),
}

fn main() {
    let x = foo();
    x.val == 42; // { dg-error ".E0308." "" { target *-*-* } }
    let x = { // { dg-error "" "" { target *-*-* } }
        val: (),
    };
}


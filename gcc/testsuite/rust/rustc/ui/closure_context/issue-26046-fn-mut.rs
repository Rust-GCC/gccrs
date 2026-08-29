fn foo() -> Box<dyn Fn()> {
    let num = 5;

    let closure = || { // { dg-error ".E0525." "" { target *-*-* } }
        num += 1;
    };

    Box::new(closure)
}

fn main() {}


// Regression test for #51117 in borrowck interaction with match
// default bindings. The borrow of `*bar` created by `baz` was failing
// to register as a conflict with `bar.take()`.

fn main() {
    let mut foo = Some("foo".to_string());
    let bar = &mut foo;
    match bar {
        Some(baz) => {
            bar.take(); // { dg-error ".E0499." "" { target *-*-* } }
            drop(baz);
        },
        None => unreachable!(),
    }
}


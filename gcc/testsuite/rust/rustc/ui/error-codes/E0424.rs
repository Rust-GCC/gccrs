struct Foo;

impl Foo {
    fn bar(self) {}

    fn foo() {
        self.bar(); // { dg-error ".E0424." "" { target *-*-* } }
    }

    fn baz(_: i32) {
        self.bar(); // { dg-error ".E0424." "" { target *-*-* } }
    }

    fn qux() {
        let _ = || self.bar(); // { dg-error ".E0424." "" { target *-*-* } }
    }
}

fn main () {
    let self = "self"; // { dg-error ".E0424." "" { target *-*-* } }
}


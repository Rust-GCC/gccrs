enum SomeEnum {
    E
}

fn main() {
    E { name: "foobar" }; // { dg-error ".E0422." "" { target *-*-* } }
}


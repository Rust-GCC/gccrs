fn /// document
foo() {}
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    foo();
}


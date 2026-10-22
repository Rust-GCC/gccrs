extern {
    fn foo(...);
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}


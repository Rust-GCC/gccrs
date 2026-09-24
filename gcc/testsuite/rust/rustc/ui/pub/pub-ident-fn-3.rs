// #60115

mod foo {
    pub bar();
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}


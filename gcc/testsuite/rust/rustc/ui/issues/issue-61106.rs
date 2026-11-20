fn main() {
    let x = String::new();
    foo(x.clone()); // { dg-error ".E0308." "" { target *-*-* } }
}

fn foo(_: &str) {}


fn main() {
    let foo = "bar";
    let x = foo("baz");
// { dg-error ".E0618." "" { target *-*-* } .-1 }
}

fn foo(file: &str) -> bool {
    true
}


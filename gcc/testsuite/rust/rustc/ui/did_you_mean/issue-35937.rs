struct Foo {
    pub v: Vec<String>
}

fn main() {
    let f = Foo { v: Vec::new() };
    f.v.push("cat".to_string()); // { dg-error ".E0596." "" { target *-*-* } }
}


struct S {
    x: i32,
}
fn foo() {
    let s = S { x: 42 };
    s.x += 1; // { dg-error ".E0594." "" { target *-*-* } }
}

fn bar(s: S) {
    s.x += 1; // { dg-error ".E0594." "" { target *-*-* } }
}


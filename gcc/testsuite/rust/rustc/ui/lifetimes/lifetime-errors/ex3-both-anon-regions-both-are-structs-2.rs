struct Ref<'a, 'b> {
    a: &'a u32,
    b: &'b u32,
}

fn foo(mut x: Ref, y: Ref) {
    x.b = y.b; // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {}


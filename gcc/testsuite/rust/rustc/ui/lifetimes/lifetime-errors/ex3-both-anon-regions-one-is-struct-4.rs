struct Ref<'a, 'b> { a: &'a u32, b: &'b u32 }

fn foo(mut y: Ref, x: &u32) {
    y.b = x; // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() { }


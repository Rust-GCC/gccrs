struct Ref<'a, 'b> { a: &'a u32, b: &'b u32 }

fn foo(mut x: Ref, y: &u32) {
    y = x.b; // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() { }


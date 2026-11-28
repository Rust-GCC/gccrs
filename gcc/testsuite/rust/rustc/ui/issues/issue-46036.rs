// Issue 46036: [NLL] false edges on infinite loops
// Infinite loops should create false edges to the cleanup block.

struct Foo { x: &'static u32 }

fn foo() {
    let a = 3;
    let foo = Foo { x: &a }; // { dg-error ".E0597." "" { target *-*-* } }
    loop { }
}

fn main() { }


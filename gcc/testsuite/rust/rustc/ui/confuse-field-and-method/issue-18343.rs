struct Obj<F> where F: FnMut() -> u32 {
    closure: F,
}

fn main() {
    let o = Obj { closure: || 42 };
    o.closure();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}


mod foo {
    pub const X: u32 = 1;
}

pub use foo as foo2;
// { dg-error ".E0365." "" { target *-*-* } .-1 }

fn main() {}


struct A {
    b: usize,
    c: usize
}

impl A {
    fn foo(&self) {
        let A { x, y } = self.d; // { dg-error ".E0027." "" { target *-*-* } }
// { dg-error ".E0027." "" { target *-*-* } .-1 }
// { dg-error ".E0027." "" { target *-*-* } .-2 }
    }
}

fn main() {}


impl<T> Option<T> {
// { dg-error ".E0116." "" { target *-*-* } .-1 }
    pub fn foo(&self) { }
}

fn main() { }


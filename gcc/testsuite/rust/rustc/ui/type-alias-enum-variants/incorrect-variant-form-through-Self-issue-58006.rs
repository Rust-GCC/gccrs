pub enum Enum {
    A(usize),
}

impl Enum {
    fn foo(&self) -> () {
        match self {
            Self::A => (),
// { dg-error ".E0533." "" { target *-*-* } .-1 }
        }
    }
}

fn main() {}


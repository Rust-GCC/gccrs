struct A {
    banana: u8,
}

impl A {
    fn new(peach: u8) -> A {
        A {
            banana: banana // { dg-error ".E0425." "" { target *-*-* } }
        }
    }

    fn foo(&self, peach: u8) -> A {
        A {
            banana: banana // { dg-error ".E0425." "" { target *-*-* } }
        }
    }
}

fn main() {}


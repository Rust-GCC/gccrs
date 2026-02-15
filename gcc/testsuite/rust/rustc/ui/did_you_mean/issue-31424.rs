// forbid-output: &mut mut self

struct Struct;

impl Struct {
    fn foo(&mut self) {
        (&mut self).bar(); // { dg-error ".E0596." "" { target *-*-* } }
    }

    // In this case we could keep the suggestion, but to distinguish the
    // two cases is pretty hard. It's an obscure case anyway.
    fn bar(self: &mut Self) {
// { dg-warning "" "" { target *-*-* } .-1 }
        (&mut self).bar(); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

fn main () {}


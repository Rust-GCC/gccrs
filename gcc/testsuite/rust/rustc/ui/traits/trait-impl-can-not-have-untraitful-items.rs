trait A { }

impl A for isize {
    const BAR: () = (); // { dg-error ".E0438." "" { target *-*-* } }
    type Baz = (); // { dg-error ".E0437." "" { target *-*-* } }
    fn foo(&self) { } // { dg-error ".E0407." "" { target *-*-* } }
}

fn main() { }


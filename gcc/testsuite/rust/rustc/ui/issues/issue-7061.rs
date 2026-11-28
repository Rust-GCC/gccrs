struct BarStruct;

impl<'a> BarStruct {
    fn foo(&'a mut self) -> Box<BarStruct> { self }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
}

fn main() {}


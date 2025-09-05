use m::unexported;
// { dg-error ".E0603." "" { target *-*-* } .-1 }

mod m {
    pub fn exported() { }

    fn unexported() { }
}


fn main() { unexported(); }


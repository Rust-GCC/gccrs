fn main() {}

extern {
    pub pub fn foo();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}


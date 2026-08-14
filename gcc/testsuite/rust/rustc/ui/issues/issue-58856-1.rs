impl A {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    fn b(self>
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}


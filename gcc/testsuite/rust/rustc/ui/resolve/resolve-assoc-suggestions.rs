// Make sure associated items are recommended only in appropriate contexts.

struct S {
    field: u8,
}

trait Tr {
    fn method(&self);
    type Type;
}

impl Tr for S {
    type Type = u8;

    fn method(&self) {
        let _: field;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
        let field(..);
// { dg-error ".E0531." "" { target *-*-* } .-1 }
        field;
// { dg-error ".E0425." "" { target *-*-* } .-1 }

        let _: Type;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
        let Type(..);
// { dg-error ".E0531." "" { target *-*-* } .-1 }
        Type;
// { dg-error ".E0425." "" { target *-*-* } .-1 }

        let _: method;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
        let method(..);
// { dg-error ".E0531." "" { target *-*-* } .-1 }
        method;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}


// { dg-excess-errors "warnings" }

struct S {
    field: [u8; {
        #[path = "outer/inner.rs"]
        // { dg-error "error handling module file for .inner." "#4145" { target *-*-* } .+1 }
        mod inner;
        // OK
        0
    }],
}

fn main() {}

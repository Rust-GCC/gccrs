#![crate_type="lib"]
#![deny(unreachable_patterns)]

mod test_struct {
    // Test the exact copy of the minimal example
    // posted in the issue.
    pub struct Punned {
        foo: [u8; 1],
        bar: [u8; 1],
    }

    pub fn test(punned: Punned) {
        match punned {
            Punned { foo: [_], .. } => println!("foo"),
            Punned { bar: [_], .. } => println!("bar"),
// { dg-error "" "" { target *-*-* } .-1 }
        }
    }
}

mod test_union {
    // Test the same thing using a union.
    pub union Punned {
        foo: [u8; 1],
        bar: [u8; 1],
    }

    pub fn test(punned: Punned) {
        match punned {
            Punned { foo: [_] } => println!("foo"),
            Punned { bar: [_] } => println!("bar"),
// { dg-error "" "" { target *-*-* } .-1 }
        }
    }
}


mod foo {
    pub const b: u8 = 2;
    pub const d: u8 = 2;
}

use foo::b as c;
use foo::d;

const a: u8 = 2;

fn main() {
    let a = 4; // { dg-error ".E0005." "" { target *-*-* } }
    let c = 4; // { dg-error ".E0005." "" { target *-*-* } }
    let d = 4; // { dg-error ".E0005." "" { target *-*-* } }
    fn f() {} // Check that the `NOTE`s still work with an item here (cf. issue #35115).
}


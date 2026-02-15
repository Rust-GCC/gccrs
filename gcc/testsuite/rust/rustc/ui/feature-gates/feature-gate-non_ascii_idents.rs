extern crate core as bäz; // { dg-error ".E0658." "" { target *-*-* } }

use föö::bar; // { dg-error ".E0658." "" { target *-*-* } }

mod föö { // { dg-error ".E0658." "" { target *-*-* } }
    pub fn bar() {}
}

fn bär( // { dg-error ".E0658." "" { target *-*-* } }
    bäz: isize // { dg-error ".E0658." "" { target *-*-* } }
    ) {
    let _ö: isize; // { dg-error ".E0658." "" { target *-*-* } }

    match (1, 2) {
        (_ä, _) => {} // { dg-error ".E0658." "" { target *-*-* } }
    }
}

struct Föö { // { dg-error ".E0658." "" { target *-*-* } }
    föö: isize // { dg-error ".E0658." "" { target *-*-* } }
}

enum Bär { // { dg-error ".E0658." "" { target *-*-* } }
    Bäz { // { dg-error ".E0658." "" { target *-*-* } }
        qüx: isize // { dg-error ".E0658." "" { target *-*-* } }
    }
}

extern {
    fn qüx();  // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}


use std::fmt::Debug;

const CONST_0: dyn Debug + Sync = *(&0 as &(dyn Debug + Sync));
// { dg-error ".E0277." "" { target *-*-* } .-1 }

const CONST_FOO: str = *"foo";
// { dg-error ".E0277." "" { target *-*-* } .-1 }

static STATIC_1: dyn Debug + Sync = *(&1 as &(dyn Debug + Sync));
// { dg-error ".E0277." "" { target *-*-* } .-1 }

static STATIC_BAR: str = *"bar";
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
    println!("{:?} {:?} {:?} {:?}", &CONST_0, &CONST_FOO, &STATIC_1, &STATIC_BAR);
}


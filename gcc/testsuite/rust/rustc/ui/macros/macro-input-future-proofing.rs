#![allow(unused_macros)]

macro_rules! errors_everywhere {
    ($ty:ty <) => (); // { dg-error "" "" { target *-*-* } }
    ($ty:ty < foo ,) => (); // { dg-error "" "" { target *-*-* } }
    ($ty:ty , ) => ();
    ( ( $ty:ty ) ) => ();
    ( { $ty:ty } ) => ();
    ( [ $ty:ty ] ) => ();
    ($bl:block < ) => ();
    ($pa:pat >) => (); // { dg-error "" "" { target *-*-* } }
    ($pa:pat , ) => ();
    ($pa:pat $pb:pat $ty:ty ,) => ();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    ($($ty:ty)* -) => (); // { dg-error "" "" { target *-*-* } }
    ($($a:ty, $b:ty)* -) => (); // { dg-error "" "" { target *-*-* } }
    ($($ty:ty)-+) => (); // { dg-error "" "" { target *-*-* } }
    ( $($a:expr)* $($b:tt)* ) => { };
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() { }


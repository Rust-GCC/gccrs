#![feature(marker_trait_attr)]
#![feature(associated_type_defaults)]

#[marker]
trait MarkerConst {
    const N: usize;
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[marker]
trait MarkerType {
    type Output;
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[marker]
trait MarkerFn {
    fn foo();
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[marker]
trait MarkerConstWithDefault {
    const N: usize = 43;
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[marker]
trait MarkerTypeWithDefault {
    type Output = ();
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

#[marker]
trait MarkerFnWithDefault {
    fn foo() {}
// { dg-error ".E0714." "" { target *-*-* } .-1 }
}

fn main() {}


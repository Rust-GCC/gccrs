// This test illustrates a case where full NLL (enabled by the feature
// switch below) produces superior diagnostics to the NLL-migrate
// mode.

#![feature(nll)]

fn doit(data: &'static mut ()) {
    || doit(data);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
}

fn main() { }


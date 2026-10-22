#![allow(dead_code, non_camel_case_types)]

// Test that bounds on higher-kinded lifetime binders are rejected.

fn bar1<'a, 'b>(
    x: &'a i32,
    y: &'b i32,
    f: for<'xa, 'xb: 'xa+'xa> fn(&'xa i32, &'xb i32) -> &'xa i32)
// { dg-error "" "" { target *-*-* } .-1 }
{
    // If the bound in f's type would matter, the call below would (have to)
    // be rejected.
    f(x, y);
}

fn bar2<'a, 'b, F: for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32>(
// { dg-error "" "" { target *-*-* } .-1 }
    x: &'a i32,
    y: &'b i32,
    f: F)
{
    // If the bound in f's type would matter, the call below would (have to)
    // be rejected.
    f(x, y);
}

fn bar3<'a, 'b, F>(
    x: &'a i32,
    y: &'b i32,
    f: F)
    where F: for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32
// { dg-error "" "" { target *-*-* } .-1 }
{
    // If the bound in f's type would matter, the call below would (have to)
    // be rejected.
    f(x, y);
}

fn bar4<'a, 'b, F>(
    x: &'a i32,
    y: &'b i32,
    f: F)
    where for<'xa, 'xb: 'xa> F: Fn(&'xa i32, &'xb i32) -> &'xa i32
// { dg-error "" "" { target *-*-* } .-1 }
{
    // If the bound in f's type would matter, the call below would (have to)
    // be rejected.
    f(x, y);
}

struct S1<F: for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32>(F);
// { dg-error "" "" { target *-*-* } .-1 }
struct S2<F>(F) where F: for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32;
// { dg-error "" "" { target *-*-* } .-1 }
struct S3<F>(F) where for<'xa, 'xb: 'xa> F: Fn(&'xa i32, &'xb i32) -> &'xa i32;
// { dg-error "" "" { target *-*-* } .-1 }

struct S_fnty(for<'xa, 'xb: 'xa> fn(&'xa i32, &'xb i32) -> &'xa i32);
// { dg-error "" "" { target *-*-* } .-1 }

type T1 = Box<dyn for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32>;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let _ : Option<for<'xa, 'xb: 'xa> fn(&'xa i32, &'xb i32) -> &'xa i32> = None;
// { dg-error "" "" { target *-*-* } .-1 }
    let _ : Option<Box<dyn for<'xa, 'xb: 'xa> Fn(&'xa i32, &'xb i32) -> &'xa i32>> = None;
// { dg-error "" "" { target *-*-* } .-1 }
}


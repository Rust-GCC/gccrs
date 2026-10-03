struct S1<F: Fn(&i32, &i32) -> &'a i32>(F); // { dg-error ".E0261." "" { target *-*-* } }
struct S2<F: Fn(&i32, &i32) -> &i32>(F); // { dg-error ".E0106." "" { target *-*-* } }
struct S3<F: for<'a> Fn(&i32, &i32) -> &'a i32>(F);
// { dg-error ".E0582." "" { target *-*-* } .-1 }
struct S4<F: for<'x> Fn(&'x i32, &'x i32) -> &'x i32>(F);
const C: Option<Box<dyn for<'a> Fn(&usize, &usize) -> &'a usize>> = None;
// { dg-error ".E0582." "" { target *-*-* } .-1 }
fn main() {}


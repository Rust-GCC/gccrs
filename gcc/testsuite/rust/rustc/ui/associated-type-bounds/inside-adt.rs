#![feature(associated_type_bounds)]
#![feature(untagged_unions)]

struct S1 { f: dyn Iterator<Item: Copy> }
// { dg-error "" "" { target *-*-* } .-1 }
struct S2 { f: Box<dyn Iterator<Item: Copy>> }
// { dg-error "" "" { target *-*-* } .-1 }
struct S3 { f: dyn Iterator<Item: 'static> }
// { dg-error "" "" { target *-*-* } .-1 }

enum E1 { V(dyn Iterator<Item: Copy>) }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
enum E2 { V(Box<dyn Iterator<Item: Copy>>) }
// { dg-error "" "" { target *-*-* } .-1 }
enum E3 { V(dyn Iterator<Item: 'static>) }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

union U1 { f: dyn Iterator<Item: Copy> }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
union U2 { f: Box<dyn Iterator<Item: Copy>> }
// { dg-error "" "" { target *-*-* } .-1 }
union U3 { f: dyn Iterator<Item: 'static> }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

fn main() {}


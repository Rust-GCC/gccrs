use std::ops::Add;
use std::ops::Sub;
use std::ops::Mul;
use std::ops::Div;
use std::ops::Rem;

type Add = bool; // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
struct Sub { x: f32 } // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
enum Mul { A, B } // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
mod Div { } // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
trait Rem {  } // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}


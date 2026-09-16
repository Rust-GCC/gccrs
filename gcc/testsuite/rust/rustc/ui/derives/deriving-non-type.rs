#![allow(dead_code)]

struct S;

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
trait T { }

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
impl S { }

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
impl T for S { }

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
static s: usize = 0;

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
const c: usize = 0;

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
mod m { }

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
extern "C" { }

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
type A = usize;

#[derive(PartialEq)] // { dg-error ".E0774." "" { target *-*-* } }
fn main() { }


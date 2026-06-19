#![feature(trait_alias)]

trait Svc<Req> { type Res; }

trait MkSvc<Target, Req> = Svc<Target> where Self::Res: Svc<Req>;
// { dg-error ".E0220." "" { target *-*-* } .-1 }
// { dg-error ".E0220." "" { target *-*-* } .-2 }

fn main() {}


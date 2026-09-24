// edition:2018

type A0 = dyn;
type A1 = dyn::dyn; // { dg-error "" "" { target *-*-* } }
type A2 = dyn<dyn, dyn>; // { dg-error "" "" { target *-*-* } }
type A3 = dyn<<dyn as dyn>::dyn>;

fn main() {}


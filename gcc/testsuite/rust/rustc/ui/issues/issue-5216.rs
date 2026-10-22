fn f() { }
struct S(Box<dyn FnMut()>);
pub static C: S = S(f); // { dg-error ".E0308." "" { target *-*-* } }


fn g() { }
type T = Box<dyn FnMut()>;
pub static D: T = g; // { dg-error ".E0308." "" { target *-*-* } }

fn main() {}


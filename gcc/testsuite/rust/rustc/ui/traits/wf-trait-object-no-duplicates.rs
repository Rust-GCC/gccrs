// The purpose of this test is to demonstrate that duplicating object safe traits
// that are not auto-traits is rejected even though one could reasonably accept this.

// Some arbitrary object-safe trait:
trait Obj {}

// Demonstrate that recursive expansion of trait aliases doesn't affect stable behavior:
type _0 = dyn Obj + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Some variations:

type _1 = dyn Send + Obj + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _2 = dyn Obj + Send + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _3 = dyn Obj + Send + Send; // But it is OK to duplicate auto traits.

// Take higher ranked types into account.

// Note that `'a` and `'b` are intentionally different to make sure we consider
// them semantically the same.
trait ObjL<'l> {}
type _4 = dyn for<'a> ObjL<'a> + for<'b> ObjL<'b>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

trait ObjT<T> {}
type _5 = dyn ObjT<for<'a> fn(&'a u8)> + ObjT<for<'b> fn(&'b u8)>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

fn main() {}


// Test that we report an error for unused type parameters in types.

struct SomeStruct<'a> { x: u32 } // { dg-error ".E0392." "" { target *-*-* } }
enum SomeEnum<'a> { Nothing } // { dg-error ".E0392." "" { target *-*-* } }
trait SomeTrait<'a> { fn foo(&self); } // OK on traits.

fn main() {}


#[used]
static FOO: u32 = 0; // OK

#[used] // { dg-error "" "" { target *-*-* } }
fn foo() {}

#[used] // { dg-error "" "" { target *-*-* } }
struct Foo {}

#[used] // { dg-error "" "" { target *-*-* } }
trait Bar {}

#[used] // { dg-error "" "" { target *-*-* } }
impl Bar for Foo {}

fn main() {}


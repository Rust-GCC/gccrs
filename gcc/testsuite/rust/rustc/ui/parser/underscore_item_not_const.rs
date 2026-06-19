// Test that various non-const items do not syntactically permit `_` as a name.

static _: () = (); // { dg-error "" "" { target *-*-* } }
struct _(); // { dg-error "" "" { target *-*-* } }
enum _ {} // { dg-error "" "" { target *-*-* } }
fn _() {} // { dg-error "" "" { target *-*-* } }
mod _ {} // { dg-error "" "" { target *-*-* } }
type _ = (); // { dg-error "" "" { target *-*-* } }
use _; // { dg-error "" "" { target *-*-* } }
use _ as g; // { dg-error "" "" { target *-*-* } }
trait _ {} // { dg-error "" "" { target *-*-* } }
trait _ = Copy; // { dg-error "" "" { target *-*-* } }
macro_rules! _ { () => {} } // { dg-error "" "" { target *-*-* } }
union _ { f: u8 } // { dg-error "" "" { target *-*-* } }

fn main() {}


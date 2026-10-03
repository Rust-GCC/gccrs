type Alias = ();
use Alias::*; // { dg-error ".E0432." "" { target *-*-* } }

use std::io::Result::*; // { dg-error ".E0432." "" { target *-*-* } }

trait T {}
use T::*; // { dg-error "" "" { target *-*-* } }

fn main() {}


enum E {}
trait Tr {}

pub(in E) struct S; // { dg-error ".E0577." "" { target *-*-* } }
pub(in Tr) struct Z; // { dg-error ".E0577." "" { target *-*-* } }
pub(in std::vec) struct F; // { dg-error ".E0742." "" { target *-*-* } }
pub(in nonexistent) struct G; // { dg-error ".E0433." "" { target *-*-* } }
pub(in too_soon) struct H; // { dg-error ".E0433." "" { target *-*-* } }

// Visibilities are resolved eagerly without waiting for modules becoming fully populated.
// Visibilities can only use ancestor modules legally which are always available in time,
// so the worst thing that can happen due to eager resolution is a suboptimal error message.
mod too_soon {}

fn main () {}


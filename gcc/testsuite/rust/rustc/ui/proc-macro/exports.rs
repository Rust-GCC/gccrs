// force-host
// no-prefer-dynamic

#![crate_type = "proc-macro"]
#![allow(warnings)]

pub fn a() {} // { dg-error "" "" { target *-*-* } }
pub struct B; // { dg-error "" "" { target *-*-* } }
pub enum C {} // { dg-error "" "" { target *-*-* } }
pub mod d {} // { dg-error "" "" { target *-*-* } }

mod e {}
struct F;
enum G {}
fn h() {}


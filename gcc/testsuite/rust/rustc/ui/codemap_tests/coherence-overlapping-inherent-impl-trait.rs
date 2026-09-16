#![allow(dead_code)]

trait C {}
impl dyn C { fn f() {} } // { dg-error ".E0592." "" { target *-*-* } }
impl dyn C { fn f() {} }
fn main() { }


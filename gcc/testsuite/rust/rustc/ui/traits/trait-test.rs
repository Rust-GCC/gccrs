#[allow(non_camel_case_types)]
trait foo { fn foo(&self); }

impl isize for usize { fn foo(&self) {} } // { dg-error ".E0404." "" { target *-*-* } }

fn main() {}


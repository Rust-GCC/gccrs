include!("two_files_data.rs");

struct Baz { }

impl Bar for Baz { } // { dg-error ".E0404." "" { target *-*-* } }

fn main() { }


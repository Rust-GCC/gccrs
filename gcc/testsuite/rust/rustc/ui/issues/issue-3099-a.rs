enum A { B, C }

enum A { D, E } // { dg-error ".E0428." "" { target *-*-* } }

fn main() {}


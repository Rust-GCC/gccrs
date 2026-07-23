mod options {
    pub struct ParseOptions {}
}

mod parser {
    pub use options::*;
    // Private single import shadows public glob import, but arrives too late for initial
    // resolution of `use parser::ParseOptions` because it depends on that resolution itself.
    use ParseOptions;
}

pub use parser::ParseOptions; // { dg-error ".E0603." "" { target *-*-* } }

fn main() {}


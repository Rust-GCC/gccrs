#![feature(register_attr)]
#![feature(register_tool)]

#![register_attr(attr)]
#![register_tool(tool)]

#[no_implicit_prelude]
mod m {
    #[attr] // { dg-error "" "" { target *-*-* } }
    #[tool::attr] // { dg-error ".E0433." "" { target *-*-* } }
    fn check() {}
}

fn main() {}


#![feature(register_attr)]
#![feature(register_tool)]

#![register_attr] // { dg-error "" "" { target *-*-* } }
#![register_tool] // { dg-error "" "" { target *-*-* } }

#![register_attr(a::b)] // { dg-error "" "" { target *-*-* } }
#![register_tool(a::b)] // { dg-error "" "" { target *-*-* } }

#![register_attr(attr, attr)] // { dg-error "" "" { target *-*-* } }
#![register_tool(tool, tool)] // { dg-error "" "" { target *-*-* } }

fn main() {}


use std::alloc::System;

#[global_allocator]
static A: System = System;
#[global_allocator]
static B: System = System;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}


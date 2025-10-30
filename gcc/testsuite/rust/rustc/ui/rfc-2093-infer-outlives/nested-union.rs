#![feature(rustc_attrs)]
#![feature(untagged_unions)]

#[rustc_outlives]
union Foo<'a, T: Copy> { // { dg-error "" "" { target *-*-* } }
    field1: Bar<'a, T>
}

// Type U needs to outlive lifetime 'b
union Bar<'b, U: Copy> {
    field2: &'b U
}

fn main() {}


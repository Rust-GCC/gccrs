#![feature(no_core)]
#![no_core]

pub mod a {
    pub mod b {
        pub const C: i32 = 12;
    }
}

pub fn foo() -> i32 {
    use a::b;
    b::C
}

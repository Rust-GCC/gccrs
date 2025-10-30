// build-pass
#![allow(dead_code)]
#![allow(non_camel_case_types)]
#![warn(clashing_extern_declarations)]

// pretty-expanded FIXME #23616

mod a {
    pub type rust_task = usize;
    pub mod rustrt {
        use super::rust_task;
        extern {
            pub fn rust_task_is_unwinding(rt: *const rust_task) -> bool;
        }
    }
}

mod b {
    pub type rust_task = bool;
    pub mod rustrt {
        use super::rust_task;
        extern {
            pub fn rust_task_is_unwinding(rt: *const rust_task) -> bool;
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
}

pub fn main() { }


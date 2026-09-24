// ignore-windows
// aux-build:cfg-target-thread-local.rs

#![feature(thread_local)]

extern crate cfg_target_thread_local;

extern {
    #[cfg_attr(target_thread_local, thread_local)]
// { dg-error ".E0658." "" { target *-*-* } .-1 }

    static FOO: u32;
}

fn main() {
    assert_eq!(FOO, 3);
}


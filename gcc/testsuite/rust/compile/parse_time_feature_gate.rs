#![feature(no_core)]
#![no_core]

// { dg-error "arbitrary expressions in key-value attributes are unstable" "" { target *-*-* } .+1 }
#[export_name = concat!(stringify!(non), stringify!(literal))]
pub extern "C" fn attribute_test_function() {}

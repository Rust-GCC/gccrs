#![feature(no_core)]
#![no_core]

pub mod foo {} // { dg-error ".foo. defined multiple times" }

use foo;

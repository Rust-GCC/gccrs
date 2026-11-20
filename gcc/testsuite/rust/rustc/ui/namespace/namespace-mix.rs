// aux-build:namespace-mix.rs

extern crate namespace_mix;
use namespace_mix::*;

mod c {
    pub struct S {}
    pub struct TS();
    pub struct US;
    pub enum E {
        V {},
        TV(),
        UV,
    }

    pub struct Item;
}

// Use something emitting the type argument name, e.g., unsatisfied bound.
trait Impossible {}
fn check<T: Impossible>(_: T) {}

mod m1 {
    pub use ::c::*;
    pub type S = ::c::Item;
}
mod m2 {
    pub use ::c::*;
    pub const S: ::c::Item = ::c::Item;
}

fn f12() {
    check(m1::S{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m1::S); // { dg-error ".E0423." "" { target *-*-* } }
    check(m2::S{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m2::S); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xf12() {
    check(xm1::S{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm1::S); // { dg-error ".E0423." "" { target *-*-* } }
    check(xm2::S{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm2::S); // { dg-error ".E0277." "" { target *-*-* } }
}

mod m3 {
    pub use ::c::*;
    pub type TS = ::c::Item;
}
mod m4 {
    pub use ::c::*;
    pub const TS: ::c::Item = ::c::Item;
}

fn f34() {
    check(m3::TS{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m3::TS); // { dg-error ".E0277." "" { target *-*-* } }
    check(m4::TS{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m4::TS); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xf34() {
    check(xm3::TS{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm3::TS); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm4::TS{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm4::TS); // { dg-error ".E0277." "" { target *-*-* } }
}

mod m5 {
    pub use ::c::*;
    pub type US = ::c::Item;
}
mod m6 {
    pub use ::c::*;
    pub const US: ::c::Item = ::c::Item;
}

fn f56() {
    check(m5::US{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m5::US); // { dg-error ".E0277." "" { target *-*-* } }
    check(m6::US{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m6::US); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xf56() {
    check(xm5::US{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm5::US); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm6::US{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm6::US); // { dg-error ".E0277." "" { target *-*-* } }
}

mod m7 {
    pub use ::c::E::*;
    pub type V = ::c::Item;
}
mod m8 {
    pub use ::c::E::*;
    pub const V: ::c::Item = ::c::Item;
}

fn f78() {
    check(m7::V{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m7::V); // { dg-error ".E0423." "" { target *-*-* } }
    check(m8::V{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m8::V); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xf78() {
    check(xm7::V{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm7::V); // { dg-error ".E0423." "" { target *-*-* } }
    check(xm8::V{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm8::V); // { dg-error ".E0277." "" { target *-*-* } }
}

mod m9 {
    pub use ::c::E::*;
    pub type TV = ::c::Item;
}
mod mA {
    pub use ::c::E::*;
    pub const TV: ::c::Item = ::c::Item;
}

fn f9A() {
    check(m9::TV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(m9::TV); // { dg-error ".E0277." "" { target *-*-* } }
    check(mA::TV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(mA::TV); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xf9A() {
    check(xm9::TV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xm9::TV); // { dg-error ".E0277." "" { target *-*-* } }
    check(xmA::TV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xmA::TV); // { dg-error ".E0277." "" { target *-*-* } }
}

mod mB {
    pub use ::c::E::*;
    pub type UV = ::c::Item;
}
mod mC {
    pub use ::c::E::*;
    pub const UV: ::c::Item = ::c::Item;
}

fn fBC() {
    check(mB::UV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(mB::UV); // { dg-error ".E0277." "" { target *-*-* } }
    check(mC::UV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(mC::UV); // { dg-error ".E0277." "" { target *-*-* } }
}
fn xfBC() {
    check(xmB::UV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xmB::UV); // { dg-error ".E0277." "" { target *-*-* } }
    check(xmC::UV{}); // { dg-error ".E0277." "" { target *-*-* } }
    check(xmC::UV); // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}


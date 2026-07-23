// ignore-tidy-linelength

#![feature(associated_type_bounds)]
#![feature(type_alias_impl_trait)]
#![feature(impl_trait_in_bindings)] // { dg-warning "" "" { target *-*-* } }
#![feature(untagged_unions)]

use std::iter;

struct SI1<T: Iterator<Item: Copy, Item: Send>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
struct SI2<T: Iterator<Item: Copy, Item: Copy>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
struct SI3<T: Iterator<Item: 'static, Item: 'static>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
struct SW1<T> where T: Iterator<Item: Copy, Item: Send> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
struct SW2<T> where T: Iterator<Item: Copy, Item: Copy> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
struct SW3<T> where T: Iterator<Item: 'static, Item: 'static> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }

enum EI1<T: Iterator<Item: Copy, Item: Send>> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
enum EI2<T: Iterator<Item: Copy, Item: Copy>> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
enum EI3<T: Iterator<Item: 'static, Item: 'static>> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
enum EW1<T> where T: Iterator<Item: Copy, Item: Send> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
enum EW2<T> where T: Iterator<Item: Copy, Item: Copy> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
enum EW3<T> where T: Iterator<Item: 'static, Item: 'static> { V(T) }
// { dg-error ".E0719." "" { target *-*-* } .-1 }

union UI1<T: Iterator<Item: Copy, Item: Send>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
union UI2<T: Iterator<Item: Copy, Item: Copy>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
union UI3<T: Iterator<Item: 'static, Item: 'static>> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
union UW1<T> where T: Iterator<Item: Copy, Item: Send> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
union UW2<T> where T: Iterator<Item: Copy, Item: Copy> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
union UW3<T> where T: Iterator<Item: 'static, Item: 'static> { f: T }
// { dg-error ".E0719." "" { target *-*-* } .-1 }

fn FI1<T: Iterator<Item: Copy, Item: Send>>() {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FI2<T: Iterator<Item: Copy, Item: Copy>>() {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FI3<T: Iterator<Item: 'static, Item: 'static>>() {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FW1<T>() where T: Iterator<Item: Copy, Item: Send> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FW2<T>() where T: Iterator<Item: Copy, Item: Copy> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FW3<T>() where T: Iterator<Item: 'static, Item: 'static> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }

fn FRPIT1() -> impl Iterator<Item: Copy, Item: Send> { iter::empty() }
fn FRPIT2() -> impl Iterator<Item: Copy, Item: Copy> { iter::empty() }
fn FRPIT3() -> impl Iterator<Item: 'static, Item: 'static> { iter::empty() }
fn FAPIT1(_: impl Iterator<Item: Copy, Item: Send>) {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FAPIT2(_: impl Iterator<Item: Copy, Item: Copy>) {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn FAPIT3(_: impl Iterator<Item: 'static, Item: 'static>) {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }

const CIT1: impl Iterator<Item: Copy, Item: Send> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }
const CIT2: impl Iterator<Item: Copy, Item: Copy> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }
const CIT3: impl Iterator<Item: 'static, Item: 'static> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }
static SIT1: impl Iterator<Item: Copy, Item: Send> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }
static SIT2: impl Iterator<Item: Copy, Item: Copy> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }
static SIT3: impl Iterator<Item: 'static, Item: 'static> = iter::empty();
// { dg-error ".E0719." "" { target *-*-* } .-1 }

fn lit1() { let _: impl Iterator<Item: Copy, Item: Send> = iter::empty(); }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn lit2() { let _: impl Iterator<Item: Copy, Item: Copy> = iter::empty(); }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
fn lit3() { let _: impl Iterator<Item: 'static, Item: 'static> = iter::empty(); }
// { dg-error ".E0719." "" { target *-*-* } .-1 }

type TAI1<T: Iterator<Item: Copy, Item: Send>> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TAI2<T: Iterator<Item: Copy, Item: Copy>> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TAI3<T: Iterator<Item: 'static, Item: 'static>> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TAW1<T> where T: Iterator<Item: Copy, Item: Send> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TAW2<T> where T: Iterator<Item: Copy, Item: Copy> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TAW3<T> where T: Iterator<Item: 'static, Item: 'static> = T;
// { dg-error ".E0719." "" { target *-*-* } .-1 }

type ETAI1<T: Iterator<Item: Copy, Item: Send>> = impl Copy;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type ETAI2<T: Iterator<Item: Copy, Item: Copy>> = impl Copy;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type ETAI3<T: Iterator<Item: 'static, Item: 'static>> = impl Copy;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type ETAI4 = impl Iterator<Item: Copy, Item: Send>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type ETAI5 = impl Iterator<Item: Copy, Item: Copy>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type ETAI6 = impl Iterator<Item: 'static, Item: 'static>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }

trait TRI1<T: Iterator<Item: Copy, Item: Send>> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRI2<T: Iterator<Item: Copy, Item: Copy>> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRI3<T: Iterator<Item: 'static, Item: 'static>> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRS1: Iterator<Item: Copy, Item: Send> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRS2: Iterator<Item: Copy, Item: Copy> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRS3: Iterator<Item: 'static, Item: 'static> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRW1<T> where T: Iterator<Item: Copy, Item: Send> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRW2<T> where T: Iterator<Item: Copy, Item: Copy> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRW3<T> where T: Iterator<Item: 'static, Item: 'static> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRSW1 where Self: Iterator<Item: Copy, Item: Send> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
// { dg-error ".E0719." "" { target *-*-* } .-2 }
trait TRSW2 where Self: Iterator<Item: Copy, Item: Copy> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
// { dg-error ".E0719." "" { target *-*-* } .-2 }
trait TRSW3 where Self: Iterator<Item: 'static, Item: 'static> {}
// { dg-error ".E0719." "" { target *-*-* } .-1 }
// { dg-error ".E0719." "" { target *-*-* } .-2 }
trait TRA1 { type A: Iterator<Item: Copy, Item: Send>; }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRA2 { type A: Iterator<Item: Copy, Item: Copy>; }
// { dg-error ".E0719." "" { target *-*-* } .-1 }
trait TRA3 { type A: Iterator<Item: 'static, Item: 'static>; }
// { dg-error ".E0719." "" { target *-*-* } .-1 }

type TADyn1 = dyn Iterator<Item: Copy, Item: Send>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TADyn2 = Box<dyn Iterator<Item: Copy, Item: Copy>>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }
type TADyn3 = dyn Iterator<Item: 'static, Item: 'static>;
// { dg-error ".E0719." "" { target *-*-* } .-1 }

fn main() {}


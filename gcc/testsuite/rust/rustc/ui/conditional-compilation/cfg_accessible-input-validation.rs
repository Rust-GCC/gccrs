#![feature(cfg_accessible)]

#[cfg_accessible] // { dg-error "" "" { target *-*-* } }
struct S1;

#[cfg_accessible = "value"] // { dg-error "" "" { target *-*-* } }
struct S2;

#[cfg_accessible()] // { dg-error "" "" { target *-*-* } }
struct S3;

#[cfg_accessible(std, core)] // { dg-error "" "" { target *-*-* } }
struct S4;

#[cfg_accessible("std")] // { dg-error "" "" { target *-*-* } }
struct S5;

#[cfg_accessible(std = "value")] // { dg-error "" "" { target *-*-* } }
struct S6;

#[cfg_accessible(std(value))] // { dg-error "" "" { target *-*-* } }
struct S7;

fn main() {}

